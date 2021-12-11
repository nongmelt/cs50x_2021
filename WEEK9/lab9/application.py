import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

QUERIES = [
    "INSERT",
    "EDIT",
    "DELETE"
    ]

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        birthday = request.form.get("birthday")
        cmd = request.form.get("cmd")

        if name and birthday and cmd:
            year, month, day = birthday.split("-")
            if cmd == "INSERT" :
                db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, month, day)
            elif cmd == "EDIT" :
                for row in db.execute("SELECT name FROM birthdays") :
                    if name.lower() == row["name"].lower() :
                        db.execute("UPDATE birthdays SET month = (?), day = (?) WHERE name=(?)", month, day, name)
            elif cmd == "DELETE" :
                for row in db.execute("SELECT * FROM birthdays") :
                    if name.lower() == row["name"].lower() and int(month) == row["month"] and int(day) == row["day"]:
                        db.execute("DELETE FROM birthdays WHERE name=(?) AND month=(?) AND day=(?)", name, month, day)
        return redirect("/")

    else:
        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", birthdays=birthdays, queries=QUERIES)


