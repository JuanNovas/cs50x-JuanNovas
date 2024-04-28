import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    tables = db.execute("SELECT name FROM sqlite_schema WHERE rootpage > 4 AND tbl_name <> 'history'")
    stocks = []
    total_value = 0
    for i in tables:

        amount = db.execute("SELECT amount FROM (?) WHERE id = (?)", (i["name"]), (session["user_id"]))
        if len(amount) != 0:
            if amount[0]["amount"] > 0:
                db.execute("SELECT * FROM users")
                info = lookup(i["name"])
                total = float(info["price"]) * int(amount[0]["amount"])
                total_value += total
                total = total
                price = info["price"]
                stocks.append({"name":info["name"], "symbol":info["symbol"], "shares":amount[0]["amount"], "price":price, "total":total})
    cash = db.execute("SELECT cash from users where id = (?)", session["user_id"])
    total_value += cash[0]["cash"]

    return render_template("/index.html", stocks=stocks, cash=cash[0]["cash"], total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        wanted = request.form.get("symbol")
        symbol = lookup(wanted)
        amount = request.form.get("shares")
        try:
            amount = int(amount)
        except:
            return apology("invalid number of shares", 400)

        money = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
        money = money[0]["cash"]

        if symbol == None:
            return apology("invalid symbol", 400)

        total_price = float(amount) * float(symbol["price"])

        if money < total_price:
            return apology("not enogh money")
        elif amount < 1:
            return apology("invalid number of stocks", 400)

        money -= total_price
        db.execute("UPDATE users SET cash=(?) WHERE id=(?)", (money), (session["user_id"]))

        #check if table exist
        try:
            check = db.execute("SELECT * FROM (?)", symbol["symbol"])
        except:
            command = "CREATE TABLE IF NOT EXISTS " + (symbol["symbol"]) + " (id INTEGER, amount INTEGER, FOREIGN KEY (id) REFERENCES users (id))"
            db.execute(command)
            command = "INSERT INTO " + (str(symbol["symbol"])) + " (id, amount) VALUES (" + (str(session["user_id"])) + ", " + (str(amount)) + ")"
            db.execute(command)

            db.execute("INSERT INTO history (id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, DATETIME())", (session["user_id"]), (symbol["symbol"]), (amount), (symbol["price"]),)

            return redirect("/")

        n = db.execute("SELECT amount FROM (?) WHERE id = (?)", (symbol["symbol"]), (session["user_id"]))
        if len(n) == 0:
            command = "INSERT INTO " + (symbol["symbol"]) + " (id, amount) VALUES (" + (str(session["user_id"])) + ", " + (str(amount)) + ")"
            db.execute(command)
            db.execute("INSERT INTO history (id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, DATETIME())", (session["user_id"]), (symbol["symbol"]), (amount), (symbol["price"]),)
            return redirect("/")
        else:
            n = n[0]["amount"] + amount
            command = "UPDATE " + (str(symbol["symbol"])) + " SET amount= " + (str(n))
            db.execute(command)
            db.execute("INSERT INTO history (id, symbol, shares, price, transacted) VALUES (?, ?, ?, ?, DATETIME())", (session["user_id"]), (symbol["symbol"]), (amount), (symbol["price"]),)
            return redirect("/")

    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    actions = db.execute("SELECT symbol, shares, price, transacted FROM history WHERE id = ?", session["user_id"])

    return render_template("/history.html", actions=actions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    db.execute("CREATE TABLE IF NOT EXISTs history (id INTGER, symbol TEXT, shares INTEGER, price FLOAT, transacted DATETIME, FOREIGN KEY (id) REFERENCES users (id))")

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

    if request.method == "POST":
        symbol = request.form.get("symbol")
        symbol = lookup(symbol)
        if symbol == None:
            return apology("invalid symbol", 400)
        else:
            return render_template("/quote.html", name=symbol["name"], price=usd(symbol["price"]), symbol=symbol["symbol"])
    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        usersnames = db.execute("SELECT username FROM users")
        users = []
        for name in usersnames:
            users.append(name["username"])
        if request.form.get("username") in users:
            return apology("username already exist", 400)

        elif not request.form.get("username"):
            return apology("must provide username", 400)

        elif not request.form.get("password"):
            return apology("must provide username", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("the passwords must be the same")

        username = request.form.get("username")
        password_hash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", (username), (password_hash))
        user = db.execute("SELECT id FROM users WHERE username = (?)", username)
        session["user_id"] = user[0]["id"]
        return redirect("/")
    else:
        return render_template("/register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        amount_selled = request.form.get("shares")
        if symbol == None or amount_selled == None:
            return apology("incomplete input")
        amount_selled = int(amount_selled)
        symbol = symbol.replace(" ", "")
        info = lookup(symbol)
        if info == None:
            return apology("invalid symbol")
        amount = db.execute("SELECT amount FROM (?) WHERE id = (?)", (info["symbol"]), (session["user_id"]))
        if amount_selled < 0 or amount_selled > int(amount[0]["amount"]):
            return apology("invalid number of stocks")
        #update cash in db
        new_money = float(info["price"]) * int(amount_selled)
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", (session["user_id"]))
        money = new_money + float(cash[0]["cash"])
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)", (money), (session["user_id"]))

        #update amount in db
        amount = int(amount[0]["amount"]) - amount_selled
        command = "UPDATE " + info["symbol"] + " SET amount = " + str(amount) + " WHERE id = " + str(session["user_id"])
        db.execute(command)

        db.execute("INSERT INTO history (id, symbol, shares, price, transacted) VALUES (?, ?, -?, ?, DATETIME())", (session["user_id"]), (info["symbol"]), (amount_selled), (info["price"]))

        return redirect("/")

    else:

        tables = db.execute("SELECT name FROM sqlite_schema WHERE rootpage > 4 AND name <> 'history'")
        symbols = []
        for table in tables:
            x = db.execute("SELECT id FROM (?) WHERE id = (?) AND amount > 0", (table["name"]), (session["user_id"]))
            if len(x) > 0:
                symbols.append(table["name"])
        return render_template("/sell.html", symbols=symbols)

