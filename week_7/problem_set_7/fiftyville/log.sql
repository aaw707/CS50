-- Keep a log of any SQL queries you execute as you solve the mystery.

-- check the crime_scene_reports table with the crime date and location
SELECT *
FROM crime_scene_reports
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND street = 'Chamberlin Street';
-- one record is returned
-- id: 295
-- description: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

-- check the interviews table on that day where the transcript mentions courthouse
SELECT *
FROM interviews
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND transcript LIKE '%courthouse%';
/*
id | name | year | month | day | transcript
161 | Ruth | 2020 | 7 | 28 | Sometime within ten minutes of the theft,
I saw the thief get into a car in the courthouse parking lot and drive away.
If you have security footage from the courthouse parking lot, you might want
to look for cars that left the parking lot in that time frame.

162 | Eugene | 2020 | 7 | 28 | I don't know the thief's name, but it was
someone I recognized. Earlier this morning, before I arrived at the courthouse,
I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.

163 | Raymond | 2020 | 7 | 28 | As the thief was leaving the courthouse,
they called someone who talked to them for less than a minute. In the call,
I heard the thief say that they were planning to take the earliest flight out
of Fiftyville tomorrow. The thief then asked the person on the other end of
the phone to purchase the flight ticket.
*/

-- check the security footage from the courthouse in 10:15-10:25am
SELECT *
FROM courthouse_security_logs
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND hour = 10
 AND minute BETWEEN 15 AND 25
 AND activity = 'exit';
/*
id | year | month | day | hour | minute | activity | license_plate
260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
that's a lot of records.. we need to narrow this down
*/

-- check the ATM transaction
SELECT *
FROM atm_transactions
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND atm_location LIKE '%Fifer%'
 AND transaction_type = 'withdraw';

/*
id | account_number | year | month | day | atm_location | transaction_type | amount
246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35
*/

-- check the fiftyville airport for the airport_id
SELECT *
FROM airports
WHERE full_name LIKE '%Fiftyville%';
/*
id | abbreviation | full_name | city
8 | CSF | Fiftyville Regional Airport | Fiftyville
*/

-- check the first flight out tomorrow
SELECT *
FROM flights
WHERE origin_airport_id = 8
 AND year = 2020
 AND month = 7
 AND day = 29
ORDER BY hour, minute
LIMIT 1;
/*
id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
*/
-- destination airport id: 4
-- find the destination
SELECT *
FROM airports
WHERE id = 4;
/*
id | abbreviation | full_name | city
4 | LHR | Heathrow Airport | London
*/

-- check the passengers of this flight
SELECT *
FROM passengers
WHERE flight_id = 36;
/*
flight_id | passport_number | seat
36 | 7214083635 | 2A
36 | 1695452385 | 3B
36 | 5773159633 | 4A
36 | 1540955065 | 5C
36 | 8294398571 | 6C
36 | 1988161715 | 6D
36 | 9878712108 | 7A
36 | 8496433585 | 7B
*/
-- too many.. need narrow it down

-- check the phone call
SELECT *
FROM phone_calls
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND duration < 60;
/*
id | caller | receiver | year | month | day | duration
221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7 | 28 | 55
281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7 | 28 | 54
*/

-- so, for the thief, we have the phone number, the passport number, and the license plate in 3 different queries

WITH phone_numbers AS (
SELECT *
FROM phone_calls
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND duration < 60
),
passport_numbers AS (
SELECT *
FROM passengers
WHERE flight_id = 36
),
license_plates AS (
SELECT *
FROM courthouse_security_logs
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND hour = 10
 AND minute BETWEEN 15 AND 25
 AND activity = 'exit'
),
transactions AS (
SELECT *
FROM atm_transactions
WHERE year = 2020
 AND month = 7
 AND day = 28
 AND atm_location LIKE '%Fifer%'
 AND transaction_type = 'withdraw'
)
SELECT people.id, name, phone_number, people.passport_number, people.license_plate, receiver, flight_id, people.passport_number, seat
FROM people
JOIN phone_numbers
ON people.phone_number = phone_numbers.caller
JOIN passport_numbers
ON people.passport_number = passport_numbers.passport_number
JOIN license_plates
ON people.license_plate = license_plates.license_plate
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN transactions
ON bank_accounts.account_number = transactions.account_number;
/*
id | name | phone_number | passport_number | license_plate | receiver | flight_id | passport_number | seat
686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X | (375) 555-8161 | 36 | 5773159633 | 4A
*/

-- THIS IS THE THEIF!

-- find the name of the accomplice
-- receiver number:  (375) 555-8161
SELECT *
FROM people
WHERE phone_number = '(375) 555-8161';
/*
id | name | phone_number | passport_number | license_plate
864400 | Berthold | (375) 555-8161 |  | 4V16VO0
*/