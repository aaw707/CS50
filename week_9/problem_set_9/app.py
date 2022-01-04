import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # check purchases table to find out which stocks does the user own
    stocks_owned = db.execute('''
                                WITH bought AS (
                                    SELECT symbol, SUM(shares) AS bought_shares
                                    FROM transactions
                                    WHERE user_id = ?
                                     AND action = "buy"
                                    GROUP BY symbol
                                ),
                                sold AS (
                                    SELECT symbol, SUM(shares) AS sold_shares
                                    FROM transactions
                                    WHERE user_id = ?
                                     AND action = "sell"
                                    GROUP BY symbol
                                ),
                                combined AS (
                                    SELECT bought.symbol AS symbol, bought_shares, IFNULL(sold_shares, 0) AS sold_shares
                                    FROM bought
                                    LEFT JOIN sold
                                    ON bought.symbol = sold.symbol
                                )
                                SELECT symbol, (bought_shares - sold_shares) AS shares
                                FROM combined
                                WHERE bought_shares - sold_shares != 0;
                            ''',
                              session["user_id"], session["user_id"])
    #print("stocks owned", stocks_owned)

    # calculate the cash_balance
    cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # assign the grand_total
    grand_total = cash_balance

    # for each stock, find out the current price, calculate the total value (price * shares), and add it to the grand total
    for stock in stocks_owned:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total_value"] = stock["price"] * stock["shares"]
        grand_total += stock["total_value"]

    # get the user name
    user_name = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]["username"]

    # render stocks owned to the page
    return render_template("index.html", user_name=user_name, stocks_owned=stocks_owned, cash_balance=cash_balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get the symbol from user input
        symbol = request.form.get("symbol")

        # if the input is blank
        if not symbol:
            return apology("must provide symbol", 400)

        # get the response from lookup
        response = lookup(symbol)

        # if the symbol doesn't exist, as per the return value of lookup
        if not response:
            return apology("symbol doesn't exist", 400)

        # check the price of the stock
        price = response["price"]

        # get the shares from user input
        shares = request.form.get("shares")

        # convert the input shares from string to int and float
        try:
            int_shares = int(shares)
            float_shares = float(shares)
        # shares is not a number
        except:
            return apology("shares must be a positive integer", 400)

        # if shares is an integer and shares > 0
        if int_shares == float_shares and int_shares > 0:

            # check how much cash this user has
            cash = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])

            # if the user can't afford the transaction, return an apology
            if int_shares * price > cash:
                return apology("not enough cash to complete the purchase", 400)

            # if the user can afford the transaction
            else:
                # insert the record into transactions table
                db.execute("INSERT INTO transactions (user_id, symbol, shares, price, action) VALUES (?, ?, ?, ?, ?)",
                           session["user_id"], symbol, int_shares, price, "buy")

                # update the cash balance in users table
                db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - int_shares * price, session["user_id"])

                # get the current balance
                current_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

                # show notice
                return render_template("bought.html", symbol=symbol, shares=int_shares, total_price=price * int_shares, current_balance=current_balance)

        # if shares is a number but not a positive integer, return an apology
        else:
            return apology("shares must be a positive integer", 400)

    # user reach route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get the username
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]["username"]

    # get the transaction history
    transaction_history = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", username=username, transaction_history=transaction_history)


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
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # user reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("quote.html")

    # user reached route via POST (as by submitting a form via POST)
    else:
        symbol = request.form.get("symbol")
        response = lookup(symbol)

        # invalid symbol
        if not response:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", name=response["name"], price=response["price"], symbol=response["symbol"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # Ensure password matches confirmation
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password must match with confirmation", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't already exist
        if len(rows) != 0:
            return apology("username already exists", 400)

        # store the new user into users
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Redirect user to login
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # get the stocks currently owned by this user
    stocks_owned = db.execute('''
                                WITH bought AS (
                                    SELECT symbol, SUM(shares) AS bought_shares
                                    FROM transactions
                                    WHERE user_id = ?
                                    AND action = "buy"
                                    GROUP BY symbol
                                ),
                                sold AS (
                                    SELECT symbol, SUM(shares) AS sold_shares
                                    FROM transactions
                                    WHERE user_id = ?
                                    AND action = "sell"
                                    GROUP BY symbol
                                ),
                                combined AS (
                                    SELECT bought.symbol AS symbol, bought_shares, IFNULL(sold_shares, 0) AS sold_shares
                                    FROM bought
                                    LEFT JOIN sold
                                    ON bought.symbol = sold.symbol
                                )
                                SELECT symbol, (bought_shares - sold_shares) AS shares
                                FROM combined
                            ''',
                              session["user_id"], session["user_id"])

    # a list of owned stock symbols
    stock_symbols = []
    for stock in stocks_owned:
        stock_symbols.append(stock["symbol"])

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get the symbol of the stock to be sold
        symbol = request.form.get("symbol")

        # if user fails to select a stock
        if not symbol:
            return apology("no stock was selected", 400)

        # if user doesn't own any of that stock
        if symbol not in stock_symbols:
            return apology("this stock is not currently owned", 400)

        # get the number of shares to be sold
        shares_to_sell = request.form.get("shares")

        # convert the input shares from string to int and float
        try:
            int_shares_to_sell = int(shares_to_sell)
            float_shares_to_sell = float(shares_to_sell)
        # shares is not a number
        except:
            return apology("shares must be a positive integer", 400)

        # if shares input is not a positive integer
        if int_shares_to_sell != float_shares_to_sell or int_shares_to_sell < 0:
            return apology("shares must be a positive integer", 400)

        # get the number of shares currently owned
        for stock in stocks_owned:
            if stock["symbol"] == symbol:
                shares_owned = stock["shares"]
                break

        # if shares input is larger than the number of shares owned by user
        if int_shares_to_sell > shares_owned:
            return apology("you don't own that many shares to sell", 400)

        # if inputs are valid, proceed with the selling
        else:

            # get the current price
            price = lookup(symbol)["price"]

            # insert the record into transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, action) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol, int_shares_to_sell, price, "sell")

            # update the cash balance in users table
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", int_shares_to_sell * price, session["user_id"])

            # get the current balance
            current_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

            # show notice
            return render_template("sold.html", symbol=symbol, total_price=price * int_shares_to_sell, shares=int_shares_to_sell, current_balance=current_balance)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", stock_symbols=stock_symbols)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add more cash to the account"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get the amount of cash to be added
        amount = request.form.get("amount")

        # convert the amount to a float
        try:
            amount = float(amount)
        # if the amount is not a number
        except:
            return apology("amount must be a number", 400)

        # check if the amount is positive
        if amount <= 0:
            return apology("amount must be a positive number", 400)

        # if input is valid, proceed
        # add to the cash balance
        db.execute("UPDATE users SET cash = cash + ?", amount)

        # return to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("add_cash.html")