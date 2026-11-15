```python run id=table-per-concrete-class
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table footballer (id integer primary key, name text not null, club text not null);
create table cricketer (id integer primary key, name text not null, runs integer not null);
create table bowler (id integer primary key, name text not null, runs integer not null,
                     wickets integer not null);
create table key_source (next_id integer not null);
insert into key_source values (1);
""")


class Player:
    def __init__(self, name, id=None):
        self.id, self.name = id, name


class Footballer(Player):
    def __init__(self, name, club, id=None):
        super().__init__(name, id)
        self.club = club

    def describe(self):
        return f"Footballer {self.name}, club {self.club}"


class Cricketer(Player):
    def __init__(self, name, runs, id=None):
        super().__init__(name, id)
        self.runs = runs

    def describe(self):
        return f"Cricketer {self.name}, runs {self.runs}"


class Bowler(Cricketer):
    def __init__(self, name, runs, wickets, id=None):
        super().__init__(name, runs, id)
        self.wickets = wickets

    def describe(self):
        return f"Bowler {self.name}, runs {self.runs}, wickets {self.wickets}"


def next_key():
    # One key source for all the tables, so that a key is unique in the whole hierarchy.
    (id,) = db.execute("select next_id from key_source").fetchone()
    db.execute("update key_source set next_id = next_id + 1")
    return id


def insert(p):
    # Each concrete class has one table with all its fields, the inherited ones too.
    p.id = next_key()
    if isinstance(p, Bowler):
        db.execute("insert into bowler values (?, ?, ?, ?)", (p.id, p.name, p.runs, p.wickets))
    elif isinstance(p, Cricketer):
        db.execute("insert into cricketer values (?, ?, ?)", (p.id, p.name, p.runs))
    else:
        db.execute("insert into footballer values (?, ?, ?)", (p.id, p.name, p.club))


def find(id):
    # A key alone does not say which table holds the row, so the mapper tries each table.
    searched = []
    for table, make in (("footballer", Footballer), ("cricketer", Cricketer), ("bowler", Bowler)):
        searched.append(table)
        row = db.execute(f"select * from {table} where id = ?", (id,)).fetchone()
        if row:
            return make(*row[1:], id=row[0]), searched


def show(table):
    rows = db.execute(f"select * from {table} order by id")
    return " | ".join(" ".join(str(v) for v in row) for row in rows)


for p in (Footballer("Ann", "Lions"), Cricketer("Bo", 412), Bowler("Cy", 95, 31)):
    insert(p)
for table in ("footballer", "cricketer", "bowler"):
    print(f"{table}: {show(table)}")
p, searched = find(3)
print(f"find 3: {p.describe()} (tables searched: {', '.join(searched)})")
everyone = db.execute("""select id, name, 'footballer' from footballer
                         union all select id, name, 'cricketer' from cricketer
                         union all select id, name, 'bowler' from bowler order by id""")
print("all players:", " | ".join(f"{i} {n} {t}" for i, n, t in everyone))
cricketers = db.execute("""select name from cricketer
                           union all select name from bowler order by name""")
print("cricketers:", ", ".join(n for (n,) in cricketers))
print("next key:", db.execute("select next_id from key_source").fetchone()[0])
```

```output
footballer: 1 Ann Lions
cricketer: 2 Bo 412
bowler: 3 Cy 95 31
find 3: Bowler Cy, runs 95, wickets 31 (tables searched: footballer, cricketer, bowler)
all players: 1 Ann footballer | 2 Bo cricketer | 3 Cy bowler
cricketers: Bo, Cy
next key: 4
```

- Verdict: idiom. Each concrete class has a SQLite table with all its columns, and each column is `not null`. `next_key` reads and increments the one row of `key_source`, so the three tables never share a key.
- The query for all players is a `union all` of the three tables with the table name as a literal type column, as SQLAlchemy builds it with `polymorphic_union`.
