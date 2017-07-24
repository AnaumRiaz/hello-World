import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue
from bs4 import BeautifulSoup
import urllib.request
from pathlib import Path
import smtplib
from smtplib import SMTPException
from cs50 import SQL
from helpers import lookup
import requests

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""
    #getting from the form the value of geo; also args takes arguments from the http link. we want geo parameter of link
    name = request.args.get("geo")
    nam = request.args.get("pname")
    
    art = []
    
    if name == "":
        raise RuntimeError('geo is empty')
    if nam == "":
        raise RuntimeError('pname is empty')
        
    art = lookup(name)
    
    nam = nam.replace(" ","+")
    
    url = 'https://www.google.com/search?q=' + nam + '+temperature'
    req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
    html = urllib.request.urlopen(req).read()
    soup = BeautifulSoup(html)
    badges = soup.body.find('span', attrs={'class': 'wob_t'})
    
    url2 = 'https://www.google.com/search?q=' + nam + '+time'
    req2 = urllib.request.Request(url2, headers={'User-Agent': 'Mozilla/5.0'})
    html2 =  urllib.request.urlopen(req2).read()
    soup2 = BeautifulSoup(html2)
    badges2 = soup2.body.find('div', attrs={'class': '_rkc _Peb'})
    
    #print(badges.getText())
    if badges == None:
        val = "Not Available"
    else:
        val = badges.getText()
        
    if badges2 == None:
        val2 = "Not Available"
    else:
        val2 = badges2.getText()
    
    my = {"Temperature": val}
    my2= {"Time" : val2}
   
    if not ("Temperature" in art[-2]):
        art.append(my)
    if not ("Time" in art[-1]):
        art.append(my2)
        
    return jsonify(art)

@app.route("/search")
def search():
    """Search for places that match query."""
    place = request.args.get("q")
    q = "%" + place  + "%"
    qa = db.execute("SELECT * FROM places WHERE postal_code LIKE :q  or place_name LIKE :p or admin_name1 LIKE :d", q=q, p=q,d=q)
    return jsonify(qa)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")
   
    if not request.args.get("mode"):
         raise RuntimeError("missing mode")
         
    mode = request.args.get("mode");
    
    if mode == '1':
        place = "places"
    else:
        place = "attractions"
        
    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM :place
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            place = place, sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM :place
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            place = place, sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
    
    

@app.route("/att")
def att():
    """Search for places that match query."""
    pn = request.args.get("pname")
    place = pn.replace(" ", "+")
    fn = pn.replace(" ", "_")
    filename = fn + '.jpeg'
    
    
    link1 = "https://en.wikipedia.org/wiki/" + fn
    link2 = "https://www.google.com/search?q=" + place + "&tbm=isch"
    statu = db.execute("SELECT status from attractions where place_name = :pn", pn = pn)     
 
    
    tosend= [{"link1": link1, "link2": link2, "fn":filename, "mode":statu[0]['status'] }]
   
    
    #search if filename exists
    my_file = Path("static/pics/" + filename)
    if my_file.is_file():
         return jsonify(tosend)
    else:
        url = 'https://www.google.com/search?q='+ place +'+image'
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        html = urllib.request.urlopen(req).read()
        soup = BeautifulSoup(html)
        l = soup.findAll('img')[0].get('src')
        imagefile = open("static/pics/" + filename, 'wb')
        imagefile.write(urllib.request.urlopen(l).read())
        imagefile.close()
        return jsonify(tosend)
   
   
   
@app.route("/saveodelete")
def saveodelete():
    pn = request.args.get("place")
    #place = pn.replace(" ", "+")
    statu = db.execute("SELECT status from attractions where place_name = :pn", pn = pn)
    if statu[0]['status'] == 'saved':
        #deleting that status
        db.execute("UPDATE attractions SET status=null where place_name =:pn", pn = pn)
        nstatus="Deleted"
    else:
        #save it 
        db.execute("UPDATE attractions SET status='saved' where place_name =:pn", pn = pn)
        nstatus= "Saved"
        
    tosend = [{"newstatus": nstatus }]
    return jsonify(tosend)
    
@app.route("/sendemail")
def sendemail():
    
    e_add = request.args.get("address");
    loclist= ['LIST']
    datatosend =  db.execute("SELECT place_name from attractions where status='saved'")
    for entry in datatosend:
        loclist.append(entry['place_name'])
    s = "\n";

    listatt=(s.join( loclist ))
    requests.post(
        "https://api.mailgun.net/v3/sandboxcef5afe406cd4056b8850aef73b94fa2.mailgun.org/messages",
        auth=("api", "key-2c5c3df20099a0324fa6db0ea5c06af9"),
        data={"from": "Mailgun Sandbox <postmaster@sandboxcef5afe406cd4056b8850aef73b94fa2.mailgun.org>",
             "to": e_add,
            "subject": "List of attractions",
           "text": listatt})
    tosend=[{"status":listatt}]
    return jsonify(tosend)
              
              
              
              