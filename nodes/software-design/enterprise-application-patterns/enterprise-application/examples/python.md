```python run id=booking-system
import sqlite3

# Persistent data: the database keeps the flights and the bookings for years.
db = sqlite3.connect(":memory:")
db.executescript("""
create table flight (id integer primary key, city text, seats_left integer);
create table booking (flight_id integer, customer text, sent integer default 0);
insert into flight values (7, 'Oslo', 1);
""")


# Concurrent users: each booking is one transaction, and the update checks the seat count,
# so two users cannot both take the last seat.
def book(flight_id, customer):
    with db:
        cursor = db.execute(
            "update flight set seats_left = seats_left - 1 where id = ? and seats_left > 0",
            (flight_id,),
        )
        if cursor.rowcount == 0:
            return "no seat left"
        db.execute("insert into booking (flight_id, customer) values (?, ?)", (flight_id, customer))
        return "ok"


# Many screens: each screen shows the same data in a different way.
def agent_screen(flight_id):
    city, seats = db.execute("select city, seats_left from flight where id = ?", (flight_id,)).fetchone()
    names = [r[0] for r in db.execute("select customer from booking where flight_id = ?", (flight_id,))]
    return f"agent screen: flight {flight_id} to {city}, seats left {seats}, booked by {', '.join(names)}"


def customer_screen(customer):
    rows = db.execute(
        "select f.id, f.city from booking b join flight f on f.id = b.flight_id where b.customer = ?",
        (customer,),
    ).fetchall()
    return f"customer screen for {customer}: " + "; ".join(f"flight {i} to {c}" for i, c in rows)


# Batch processing and integration: a night job sends new bookings to another system as a file.
def night_batch():
    with db:
        rows = db.execute("select flight_id, customer from booking where sent = 0").fetchall()
        db.execute("update booking set sent = 1 where sent = 0")
    return "\n".join(f"{flight_id};{customer}" for flight_id, customer in rows), len(rows)


for customer in ("Ann", "Bob"):
    print(f"{customer} books flight 7: {book(7, customer)}")
print(agent_screen(7))
print(customer_screen("Ann"))
text, count = night_batch()
print(f"night batch: {count} booking sent to the airline system")
print("airline file:", text)
print("second night batch:", night_batch()[1], "bookings")
```

```output
Ann books flight 7: ok
Bob books flight 7: no seat left
agent screen: flight 7 to Oslo, seats left 0, booked by Ann
customer screen for Ann: flight 7 to Oslo
night batch: 1 booking sent to the airline system
airline file: 7;Ann
second night batch: 0 bookings
```

SQLite stands in for the database server. The `with db:` block is one transaction, and the `update` checks the seat count in the same statement, so a second booking of the last seat changes no row. The two screens read the same tables, and the night batch marks the rows that it sent, so a second run sends nothing.
