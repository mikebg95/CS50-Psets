from __future__ import print_function

import os
import sys
import datetime
import time

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

    id = session["user_id"]

    user = db.execute("SELECT * FROM users WHERE id=:id", id=id)
    cash = int(user[0]["cash"])

    stocks_info = db.execute("SELECT * FROM stocks WHERE stock_id=:id", id=id)

    total_money = cash
    final_stocks = []

    for stock in stocks_info:
        if (stock["stock"] != None):
            symbol = stock["stock"]
            name = lookup(symbol)["name"]
            shares = stock["amount"]
            price = lookup(symbol)["price"]
            total = shares * price

            total_money += total

            final_stock = {"symbol": symbol, "name": name, "shares": shares, "price": price, "total": total}
            final_stocks.append(final_stock)

    total_money = "%0.2f" % total_money

    return render_template("index.html", cash=cash, stocks=final_stocks, total=total_money)
    # return apology("TODO")


# SOMETHING GOES WRONG WHEN BUYING STOCK ALREADY OWNED: TOTAL DOESNT ADD UP!!!

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")
    else:
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("stock doesn't exist")

        price = stock["price"]
        symbol = stock["symbol"]

        shares = int(request.form.get("shares"))

        id = session["user_id"]

        user = db.execute("SELECT * FROM users WHERE id=:id", id=id)
        cash = user[0]["cash"]

        # if not enough cash -> works!
        if price * shares > cash:
            return apology("not enough cash")

        # if enough cash
        else:
            # print("")
            # print("")
            # print("cash before: " + str(cash))
            # print("price: " + str(price))
            # print("number of shares: " + str(shares))
            # print("price x shares: " + str(price * shares))
            # print("cash after: " + str(cash - price*shares))
            # print("")
            # print("")

            timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')

            db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash-price*shares, id=id)
            db.execute("INSERT INTO history (history_id, stock, shares, price, timestamp) VALUES (:id, :stock, :shares, :price, :timestamp)", id=id, stock=symbol, shares=shares, price=price, timestamp=timestamp)

            stock_user = db.execute("SELECT * FROM stocks WHERE stock_id=:id", id=id)

            # if user doesnt own any stocks yet
            if not stock_user:
                db.execute("INSERT INTO stocks (stock_id, stock, amount) VALUES (:id, :stock, :amount)", id=id, stock=symbol, amount=shares)
                # db.execute("UPDATE stocks SET stock=:stock, amount=:amount WHERE stock_id=:id", stock=symbol, amount=shares, id=id)

            # if user already owns stocks
            else:
                this_stock = db.execute("SELECT * FROM stocks WHERE stock_id=:id AND stock=:stock", id=id, stock=symbol)

                # if user doesnt own this stock yet
                if not this_stock:
                    db.execute("INSERT INTO stocks (stock_id, stock, amount) VALUES (:id, :stock, :amount)", id=id, stock=symbol, amount=shares)

                # if user already owns this stock
                else:
                    new_amount = int(stock_user[0]["amount"]) + shares
                    db.execute("UPDATE stocks SET amount=:new_amount WHERE stock_id=:id AND stock=:stock", new_amount=new_amount, id=id, stock=symbol)

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    id = session["user_id"]

    user_history = db.execute("SELECT * FROM history WHERE history_id=:id", id=id)

    print("")
    print("")
    for transaction in user_history:
        print(transaction)
        print("")
    print("")
    print("")

    return render_template("history.html", history=user_history)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

    if request.method == "GET":
        return render_template("quote.html")
    else:
        smbl = request.form.get("symbol")
        stock = lookup(smbl)

        if not stock:
            return apology("stock doesn't exist")

        return render_template("quote2.html", name=stock["name"], symbol=stock["symbol"], price=stock["price"])



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # if user entered page through GET request, show register template
    if request.method == "GET":
        return render_template("register.html")

    # if user entered page through POST request
    else:

        # ensure user inserted username
        if not request.form.get("username"):
            return apology("please insert username", 400)

        # ensure user inserted password
        elif not request.form.get("password"):
            return apology("please insert password", 400)

        # ensure passwords match
        elif request.form.get("password") != request.form.get("password_check"):
            return apology("passwords do not match", 400)

        # save username and (hashed) password in variables
        username = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))

        # insert user only if username isnt taken
        try:
            result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)
        except:
            return apology("username taken", 400)

        # db.execute("INSERT INTO stocks (stock_id) VALUES (:id)", id=result)

        # remember user that has logged in
        session["user_id"] = result



        # show message to confirm registration and go to homepage
        flash("Registered!")
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    id = session["user_id"]

    if request.method == "GET":

        stocks = []
        user_stocks = db.execute("SELECT * FROM stocks WHERE stock_id=:id", id=id)

        for stock in user_stocks:
            stocks.append(str(stock["stock"]))

        return render_template("sell.html", stocks=stocks)
    else:
        stock = request.form.get("symbol")
        sold_shares = int(request.form.get("shares"))

        current_shares = int(db.execute("SELECT * FROM stocks WHERE stock_id=:id AND stock=:stock", id=id, stock=stock)[0]["amount"])

        new_shares = current_shares - sold_shares

        if new_shares < 0:
            return apology("not enough shares")

        if new_shares == 0:
            db.execute("DELETE FROM stocks WHERE stock_id=:id AND stock=:stock", id=id, stock=stock)
        else:
            db.execute("UPDATE stocks SET amount=:amount WHERE stock_id=:id AND stock=:stock", amount=new_shares, id=id, stock=stock)

        old_cash = int(db.execute("SELECT * FROM users WHERE id=:id", id=id)[0]["cash"])
        stock_price = lookup(stock)["price"]
        new_cash = old_cash + stock_price * sold_shares
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=new_cash, id=id)

        timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')

        db.execute("INSERT INTO history (history_id, stock, shares, price, timestamp) VALUES (:id, :stock, :shares, :price, :timestamp)", id=id, stock=stock, shares=sold_shares*-1, price=stock_price, timestamp=timestamp)


        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
