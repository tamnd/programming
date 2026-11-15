```python run id=table-per-class
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
pragma foreign_keys = on;
create table player (id integer primary key, name text not null);
create table footballer (id integer primary key references player(id), club text not null);
create table cricketer (id integer primary key references player(id), runs integer not null);
create table bowler (id integer primary key references cricketer(id), wickets integer not null);
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


def insert(p):
    # One row in the table of each class on the path from the root, all with the same key.
    p.id = db.execute("insert into player (name) values (?)", (p.name,)).lastrowid
    if isinstance(p, Footballer):
        db.execute("insert into footballer values (?, ?)", (p.id, p.club))
    if isinstance(p, Cricketer):
        db.execute("insert into cricketer values (?, ?)", (p.id, p.runs))
    if isinstance(p, Bowler):
        db.execute("insert into bowler values (?, ?)", (p.id, p.wickets))


def find(id):
    # The root row has no type, so the mapper looks for the key in the tables of the subclasses.
    (name,) = db.execute("select name from player where id = ?", (id,)).fetchone()
    tables = ["player"]
    footballer = db.execute("select club from footballer where id = ?", (id,)).fetchone()
    if footballer:
        return Footballer(name, footballer[0], id), tables + ["footballer"]
    (runs,) = db.execute("select runs from cricketer where id = ?", (id,)).fetchone()
    bowler = db.execute("select wickets from bowler where id = ?", (id,)).fetchone()
    if bowler:
        return Bowler(name, runs, bowler[0], id), tables + ["cricketer", "bowler"]
    return Cricketer(name, runs, id), tables + ["cricketer"]


def delete(p):
    count = 0
    for table in ("bowler", "cricketer", "footballer", "player"):  # Children first.
        count += db.execute(f"delete from {table} where id = ?", (p.id,)).rowcount
    return count


def show(table):
    rows = db.execute(f"select * from {table} order by id")
    return " | ".join(" ".join(str(v) for v in row) for row in rows)


ann, bo, cy = Footballer("Ann", "Lions"), Cricketer("Bo", 412), Bowler("Cy", 95, 31)
for p in (ann, bo, cy):
    insert(p)
for table in ("player", "footballer", "cricketer", "bowler"):
    print(f"{table}: {show(table)}")
for id in (1, 2, 3):
    p, tables = find(id)
    print(f"loaded: {p.describe()} (tables: {', '.join(tables)})")
sql = "select p.name from player p join cricketer c on c.id = p.id order by p.id"
print("cricketers:", ", ".join(n for (n,) in db.execute(sql)))
print("delete Cy: rows deleted", delete(cy))
```

```output
player: 1 Ann | 2 Bo | 3 Cy
footballer: 1 Lions
cricketer: 2 412 | 3 95
bowler: 3 31
loaded: Footballer Ann, club Lions (tables: player, footballer)
loaded: Cricketer Bo, runs 412 (tables: player, cricketer)
loaded: Bowler Cy, runs 95, wickets 31 (tables: player, cricketer, bowler)
cricketers: Bo, Cy
delete Cy: rows deleted 3
```

- Verdict: idiom. Each table has the key of the player row as its primary key and as a foreign key to the table above, and `pragma foreign_keys = on` makes SQLite check it. `insert` writes one row for each class on the path, with `isinstance` tests.
- The root table has no type column, so `find` looks for the key in the footballer and bowler tables to choose the class.
