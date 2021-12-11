import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ? ORDER BY symbol_shares ASC", user_id)
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    profit_shares = user_cash

    for port in portfolio :
        share = lookup(port["symbol_shares"])
        port["market_price"] = usd(share["price"])
        port["total"] = usd(port["available_shares"]*share["price"])
        profit_shares += port["available_shares"]*share["price"]

    return render_template("index.html", portfolio=portfolio, user_cash=usd(user_cash), profit_shares=usd(profit_shares))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST" :
        time = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
        user_id = session["user_id"]

        symbol = request.form.get("symbol")
        if not symbol :
            return apology("missing symbol", 400)
        shares = request.form.get("shares")
        if not shares :
            return apology("missing shares", 400)
        shares = int(shares)

        stock = lookup(symbol)
        if not stock :
            return apology("invalid symbol", 400)


        user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
        user_port = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol_shares = ?", user_id, stock["symbol"])

        if user_cash >= stock["price"]*shares :

            db.execute("INSERT INTO transactions (user_id, symbol, name, price, status, shares,  time) VALUES (?, ?, ?, ?, ?, ?, ?)"
                , user_id, stock["symbol"], stock["name"], stock["price"], "BUY", shares, time)

            if not user_port :
                db.execute("INSERT INTO portfolio (user_id, symbol_shares, name_shares, available_shares) VALUES (?, ?, ?, ?)"
                , user_id, stock["symbol"], stock["name"],shares)
            else :
                available_shares = user_port["available_shares"] + shares
                db.execute("UPDATE portfolio SET available_shares = ? WHERE user_id = ? AND symbol = ?"
                , available_shares, user_id, stock["symbol"])

            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash - stock["price"]*shares, user_id)
        else :
            return apology("can't afford", 400)
        flash("Bought")
        return redirect("/")
    else :
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    records = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY time", user_id)
    for record in records :
        record["price"] = usd(record["price"])

    return render_template("history.html",records=records)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("You were successfully logged in")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/forget", methods=["GET", "POST"])
def forget():
    if request.method == "POST" :
        if not request.form.get("username"):
            return apology("must provide username", 403)

        user = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(user) != 1 :
            return apology("Couldn't find your username. Please register", 403)

        session["user_id"] = user[0]["id"]

        return redirect("/newpassword")
    else :
        return render_template("forget.html")

@app.route("/newpassword", methods=["GET", "POST"])
def newpassword():
    if request.method == "POST" :

        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not password:
            return apology("must provide password", 403)
        elif not confirmation :
            return apology("must provide password confirmation", 403)

        user_id = session["user_id"]
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(password), user_id)

        session.clear()

        flash("Password has been changed")
        return redirect("/login")
    else :
        return render_template("newpassword.html")

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    flash("You were logged out")
    return redirect("/")

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST" :
        symbol = request.form.get("symbol")
        if not symbol :
            return render_template("quote.html")

        quote = lookup(symbol)
        if not quote :
            return apology("invalid symbol", 400)

        quote["price"] = usd(quote["price"])
        return render_template("quoted.html", quote=quote)
    else :
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("must provide username", 403)
        elif not password:
            return apology("must provide password", 403)
        elif not confirmation :
            return apology("must provide password confirmation", 403)
        elif password != confirmation :
            return apology("those passwords didn’t match. Try again.", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        user_id = db.execute("SELECT id FROM users WHERE username = ?", username)[0]
        session["user_id"] = user_id

        flash("You were succesfully registered")
        return redirect("/")
    else :
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "POST" :
        time = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

        symbol = request.form.get("symbol")
        if not symbol :
            return apology("missing symbol", 400)
        shares = request.form.get("shares")
        if not shares :
            return apology("missing shares", 400)
        shares = int(shares)

        stock = lookup(symbol)
        if not stock :
            return apology("invalid symbol", 400)

        user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
        user_portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol_shares = ?", user_id, stock["symbol"])[0]

        if user_portfolio["available_shares"] < shares :
            return apology("too many shares", 400)

        db.execute("INSERT INTO transactions (user_id, symbol, name, price, status, shares,  time) VALUES (?, ?, ?, ?, ?, ?, ?)"
            , user_id, stock["symbol"], stock["name"], stock["price"], "SELL", shares, time)

        available_shares = user_portfolio["available_shares"] - shares
        if available_shares == 0 :
            db.execute("DELETE portfolio WHERE user_id = ? AND symbol_shares = ?"
            , user_id, stock["symbol"])
        else :
            db.execute("UPDATE portfolio SET available_shares = ? WHERE user_id = ? AND symbol_shares = ?"
                , available_shares, user_id, stock["symbol"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + stock["price"]*shares, user_id)

        flash("Sold")
        return redirect("/")
    else :
        portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", user_id)
        return render_template("sell.html", portfolio=portfolio)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
