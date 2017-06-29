from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

import datetime
from helpers import *

global id;
# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# custom filter for calculating grand total
app.jinja_env.filters["gt"] = gt


# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    u_row = db.execute("SELECT cash FROM users where id = :uid", uid = id)
    history_rows = db.execute("SELECT * FROM history where userid = :userid", userid = id)
    
    #stocks name lists
    ml=[]
    for row in history_rows:
        ml.append(row["stock"])
    ml = set(ml)
    
    
    #number of shares list sl; ml2 is companies with net share 0
    sl =[]
    ml2=[]
    
    for company in ml:
        n_shares = db.execute("SELECT sum(shares) FROM history where userid = :userid and stock = :sv", userid = id, sv = company )
        if int(n_shares[0]["sum(shares)"]) == 0:
            ml2.append(company)
        else:
            sl.append(n_shares[0]["sum(shares)"])
    
    #deleting companies that have net 0 shares
    for company in ml2:
        ml.remove(company)
        
    
    #stock value lists
    splist = []
    for company in ml:
       info = lookup(company)
       splist.append(info["price"])
      
    #fdict 1 is ml: shares 
    #fdict 2 is ml: stock prices
    fdict1 = dict(zip(ml, sl))
    fdict2 = dict(zip(ml, splist))
   
    
    #return render_template("check.html",value = fdict1)
    return render_template("index.html", cash = u_row[0]['cash'], shares = fdict1, sp = fdict2 )
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    
    a = db.execute("SELECT * FROM users where id = :uid", uid = id)
    cash  = a[0]["cash"]
    
    if request.method == "GET":
        return render_template("buy.html", cash = cash)
   
    if request.method == "POST":
        
        #check if both fields are filled
        if request.form.get("stock") == ""   or request.form.get("n_shares")=="":
            return apology("Enter stock AND shares")
        
        #check if input symbol is valid
        stock_info = lookup(request.form.get("stock"))
        if stock_info == None:
            return apology("Stock symbol's faulty")
        
        #if shares to buy is positive number
        elif int(request.form.get("n_shares")) < 1 :
            return apology("Number of shares must be greater than 0")
            
        #check if user can afford it
        to_buy = int(request.form.get("n_shares")) * stock_info["price"]
        
        if to_buy > cash :
            return apology("Not sufficient balance")
        else:
            db.execute("INSERT INTO history (userid, stock,shares) VALUES(:userid, :stock, :shares)", userid = id, stock = (request.form.get("stock")).upper(), shares = int(request.form.get("n_shares"))) 
            db.execute("UPDATE users SET cash  = :cash where id = :uid", cash = cash - to_buy, uid = id)
            return redirect(url_for("index"))
        

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    history_rows = db.execute("SELECT * FROM history where userid = :userid", userid = id)
    #return render_template("check.html", value = history_rows)
    
    #ml is list of company stocks
    ml=[]
    for row in history_rows:
        ml.append(row["stock"])
    ml = set(ml)
    
    #splist is list of stock prices
    splist = []
    for company in ml:
       info = lookup(company)
       splist.append(info["price"])
       
    f_dict = dict(zip(ml, splist))
    return render_template("history.html",  rows = history_rows, prices = f_dict )
 

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        global id 
        id = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
        
    if request.method == "POST":
        q_info = lookup(request.form["toquote"])
        if q_info == None:
            return apology("Enter Company's symbol")
        return render_template("quoted.html", q_info = q_info)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    
    if request.method == "POST":
        if request.form["username"] == "" or request.form["password"] == "" or request.form["repassword"] == "":
            return apology("Provide username and passwords")
        
        # ensure username doesnt exist
        urow = db.execute("SELECT * FROM users WHERE username = :uname", uname = request.form["username"])
        if len(urow) != 0:
            return apology("This username already exists!")
        
        elif request.form["password"] == request.form["repassword"] :
            db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form["username"], hash = pwd_context.encrypt(request.form["password"]))
            
            row = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = row[0]["id"]
           # redirect user to home page
            return redirect(url_for("index"))
           
        else :
            return apology("passwords do not match")
    

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        return render_template("sell.html")
        
    if request.method == "POST":
       
        #check if both fields are filled
        if request.form.get("stock") == ""   or request.form.get("n_shares")=="":
            return apology("Enter stock AND shares")
        
        
        #check if the stock symbol is valid
        stock_info = lookup(request.form.get("stock"))
        if stock_info == None:
            return apology("Stock symbol's faulty")
    
        if  int(request.form.get("n_shares")) < 1 :
            return apology("shares should be a posiitve number")
            
        #checkinf if stock has enough shares 
        n_shares = db.execute("SELECT sum(shares) FROM history where userid = :userid and stock = :sv", userid = id, sv = (request.form.get("stock")).upper() )
        if n_shares == None or int(request.form.get("n_shares")) > int(n_shares[0]["sum(shares)"]):
            return apology("You dont have enough shares of the stock")
        
        #calculate current balance & amount to add
        sellprice = int(request.form.get("n_shares")) * stock_info["price"]
        a = db.execute("SELECT * FROM users where id = :uid", uid = id)
        cash  = a[0]["cash"]
        
        #update cash; update stocks; update shares
        db.execute("UPDATE users SET cash = :cash where id = :uid", cash = cash + sellprice, uid = id)
        db.execute("INSERT INTO history (userid, stock, shares) VALUES(:userid, :stock, :shares)", userid = id, stock = (request.form.get("stock")).upper(), shares = (int(request.form.get("n_shares"))) * -1)
        #return render_template("check.html",value = (int(request.form.get("n_shares"))) * -1)
       
        return redirect(url_for("index"))
        
@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "GET":
            return render_template("addcash.html")
    
    if request.method == "POST":
        if request.form.get("cash") == "" or int(request.form.get("cash")) < 0:
            return apology("Cash should be positive")
                
        else:
                
            a = db.execute("SELECT * FROM users where id = :uid", uid = id)
            cash  = a[0]["cash"]
            db.execute("UPDATE users SET cash = :cashv where id = :uid", cashv = cash + int(request.form.get("cash")), uid = id)
            return redirect(url_for("index"))
