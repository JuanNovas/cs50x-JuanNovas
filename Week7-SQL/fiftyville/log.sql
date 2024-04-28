-- Keep a log of any SQL queries you execute as you solve the mystery.
.schema -- to see the tables and the values
SELECT * FROM crime_scene_reports; -- to see what happened
SELECT * FROM crime_scene_reports WHERE description LIKE "%duck%"; -- to see what happend with the duck
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%"; -- to see the interviews of the day of the crime
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25; -- to see the bakery security log to check the information of Ruth
SELECT * FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street"; -- to see the transactions at Legget Streat the day of the crime
SELECT * FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60; -- to see the calss of the day of the crime
SELECT * FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street"); -- to get ids


SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
AND passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute ASC LIMIT 1))
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25); -- to get the robber of the duck by checking all the information we have

SELECT city FROM airports WHERE id IN
(SELECT destination_airport_id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id IN
(SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute ASC LIMIT 1); -- to get the city the criminals scape to

SELECT name FROM people WHERE phone_number IN
(SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller IN
(SELECT phone_number FROM people WHERE name IN
(SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND phone_number IN
(SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
AND passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute ASC LIMIT 1))
AND license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25)))); -- to get the name of the acomplice, reusing the command use to get the name of the robber
