-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Begin with looking at crime_scene_reports on July 28, 2020 on Chamberlin Street
SELECT * FROM crime_scene_reports
WHERE street = "Chamberlin Street"
AND month = 7
AND day = 28;

-- According to the description, the crime was took place at courthouse, and there were 3 witnesses
-- -> look at courthouse_security_logs and interviews on the same date around 10.15am
SELECT * FROM courthouse_security_logs
WHERE month = 7
AND day = 28;

-- assume the theft driving a car?
-- -> check the license_plate entrance before 10.15am and exit after that?
-- -> too much assumption so looking at interviews instead
-- looking at interviews
SELECT * FROM interviews
WHERE month = 7
AND day = 28;

-- scope court
SELECT * FROM interviews
WHERE month = 7
AND day = 28
AND transcript LIKE "%courthouse%";
-- 10.15 - 10.25am exit at courthouse's parking lots
-- ATM withdraw Fifer Street
-- call someone less than a min to buy a ticket for earliest flight tmr

SELECT people.name FROM people
-- 10.15 - 10.25am exit at courthouse's parking lots
WHERE people.license_plate IN (
    SELECT license_plate FROM courthouse_security_logs
    WHERE month = 7
    AND day = 28
    AND hour = 10
    AND minute <= 25
    AND activity = "exit"
)
-- ATM withdraw Fifer Street
AND people.id IN (
    SELECT people.id FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    WHERE bank_accounts.account_number IN (
        SELECT account_number FROM atm_transactions
        WHERE month = 7
        AND day = 28
        AND atm_location = "Fifer Street"
        AND transaction_type = "withdraw"
        )
    )
-- call someone less than a min
AND people.phone_number IN (
    SELECT people.phone_number FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    WHERE phone_calls.caller IN (
        SELECT caller FROM phone_calls
        WHERE month = 7
        AND day = 28
        AND duration <= 60
        )
    )
-- buy a ticket for earliest flight tmr
AND people.passport_number IN (
    SELECT passport_number FROM passengers
    WHERE flight_id IN (
        SELECT id FROM flights
        WHERE origin_airport_id = (
            SELECT id FROM airports WHERE city = "Fiftyville")
        AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
        )
    );
-- Thief : Ernest

SELECT city FROM airports
WHERE id = (
    SELECT destination_airport_id FROM flights
    WHERE origin_airport_id = (
        SELECT id FROM airports
        WHERE city = "Fiftyville"
        )
    AND month = 7 AND day = 29
    ORDER BY hour, minute DESC
    LIMIT 1
    );
-- ESC-to City :London

SELECT people.name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE phone_calls.caller = (
    SELECT people.phone_number FROM people
    WHERE people.name = "Ernest"
    )
AND phone_calls.month = 7
AND phone_calls.day = 28
AND duration <= 60;

-- Accomplice : Berthold