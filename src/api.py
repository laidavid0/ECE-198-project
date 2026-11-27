from flask import Flask, request
import sqlite3

app = Flask(__name__)
DB = "profiles.db"

conn = sqlite3.connect(DB)
conn.execute("""
    CREATE TABLE IF NOT EXISTS profiles(
        id INTEGER PRIMARY KEY,
        max_range INTEGER,
        min_range INTEGER
    )
""")
conn.commit()
conn.close()


# add a profile, given the max range and min range
@app.route("/add_profile")
def add_profile():
    min_range = request.args.get("min")
    max_range = request.args.get("max")

    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    cur.execute("INSERT INTO profiles (max_range, min_range) VALUES (?, ?)", (max_range, min_range))
    conn.commit()
    conn.close()


# remove a profile, given its id
@app.route("/remove_profile")
def remove_profile():
    profile_id = request.args.get("id")

    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    cur.execute("DELETE FROM profiles WHERE id = ?", (profile_id,))
    conn.commit()
    conn.close()
    return "removed"


# update a profile, given its index, the max range and min range
@app.route("/update_profile")
def update_profile():
    index = request.args.get("index")
    min_range = request.args.get("min")
    max_range = request.args.get("max")

    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    cur.execute("UPDATE profiles SET max_range = ? WHERE id = ?", (max_range, index))
    cur.execute("UPDATE profiles SET min_range = ? WHERE id = ?", (min_range, index))
    conn.commit()
    conn.close()
    return "updated"


# return the number of profiles
@app.route("/output")
def output():
    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    cur.execute("SELECT COUNT(*) FROM profiles")
    count = cur.fetchone()[0]
    conn.close()
    return str(count)


# return the min range and max range, given the index
@app.route("/max_min_range")
def max_min_range():
    index = request.args.get("index")

    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    cur.execute("SELECT * FROM profiles WHERE id = ?", (index,))
    row = cur.fetchone()
    conn.close()

    if row is None:
        return "not found"
    return str(row[2]) + "," + str(row[1])


app.run(host="0.0.0.0", port=5000)