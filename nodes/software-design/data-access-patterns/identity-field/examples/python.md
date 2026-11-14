```python run id=insert-then-update
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table person (id integer primary key, name text, email text)")
db.execute("create table person_by_email (email text primary key, name text)")


class Person:
    def __init__(self, name, email, id=None):
        self.id = id  # The Identity Field: the primary key of the row.
        self.name, self.email = name, email

    def show_id(self):
        return "none" if self.id is None else str(self.id)


def save(p):
    if p.id is None:
        cur = db.execute("insert into person (name, email) values (?, ?)", (p.name, p.email))
        p.id = cur.lastrowid
        return 1
    cur = db.execute("update person set name = ?, email = ? where id = ?", (p.name, p.email, p.id))
    return cur.rowcount


def find(id):
    row = db.execute("select name, email, id from person where id = ?", (id,)).fetchone()
    return Person(*row)


ann, bo = Person("Ann", "ann@example.com"), Person("Bo", "bo@example.com")
print(f"new: Ann id={ann.show_id()} | Bo id={bo.show_id()}")
save(ann)
save(bo)
print(f"saved: Ann id={ann.show_id()} | Bo id={bo.show_id()}")

ann.email = "ann@new.example"
rows = save(ann)
row = db.execute("select id, name, email from person where id = ?", (ann.id,)).fetchone()
print(f"update by id: rows {rows} | {row[0]} {row[1]} {row[2]}")

b = find(2)
print(f"find(2): id={b.id} {b.name} {b.email}")

first, second = find(1), find(1)
objects = "same" if first is second else "different"
ids = "equal" if first.id == second.id else "different"
print(f"two loads of row 1: {objects} objects, {ids} ids")

# The same change with a meaningful key: the email is the key.
db.execute("insert into person_by_email values ('cy@example.com', 'Cy')")
cy = Person("Cy", "cy@example.com")
cy.email = "cy@new.example"
cur = db.execute("update person_by_email set name = ? where email = ?", (cy.name, cy.email))
print(f"update by email key: rows {cur.rowcount}")
```

```output
new: Ann id=none | Bo id=none
saved: Ann id=1 | Bo id=2
update by id: rows 1 | 1 Ann ann@new.example
find(2): id=2 Bo bo@example.com
two loads of row 1: different objects, equal ids
update by email key: rows 0
```

- Verdict: idiom. The key field starts as `None`. After the `INSERT`, `cursor.lastrowid` gives the key that SQLite made for the `INTEGER PRIMARY KEY` column, and each `UPDATE` uses it in the `WHERE` clause.
- `cursor.rowcount` shows how many rows an `UPDATE` changed. It is 0 for the table whose key is the email, because the changed email no longer matches the row.
