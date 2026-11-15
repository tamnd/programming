```python run id=one-table-for-hierarchy
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("""create table player (id integer primary key, type text not null, name text,
              club text, runs integer, wickets integer)""")


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


# The discriminator: one value for each concrete class, kept in the type column.
TYPE_OF = {Footballer: "footballer", Cricketer: "cricketer", Bowler: "bowler"}
SUBTYPES = {"cricketer": ["cricketer", "bowler"]}


def insert(p):
    # Each class writes its own fields; the columns of the other classes stay null.
    sql = "insert into player (type, name, club, runs, wickets) values (?, ?, ?, ?, ?)"
    values = (TYPE_OF[type(p)], p.name, getattr(p, "club", None),
              getattr(p, "runs", None), getattr(p, "wickets", None))
    p.id = db.execute(sql, values).lastrowid


def load(row):
    id, kind, name, club, runs, wickets = row
    if kind == "footballer":
        return Footballer(name, club, id)
    if kind == "cricketer":
        return Cricketer(name, runs, id)
    return Bowler(name, runs, wickets, id)


def find_all(kind=None):
    if kind is None:
        rows = db.execute("select * from player order by id")
    else:
        kinds = SUBTYPES.get(kind, [kind])
        marks = ", ".join("?" for _ in kinds)
        rows = db.execute(f"select * from player where type in ({marks}) order by id", kinds)
    return [load(r) for r in rows]


def show(v):
    return "null" if v is None else str(v)


insert(Footballer("Ann", "Lions"))
insert(Cricketer("Bo", 412))
insert(Bowler("Cy", 95, 31))
for row in db.execute("select * from player order by id"):
    print("row:", " ".join(show(v) for v in row))
for p in find_all():
    print("loaded:", p.describe())
print("cricketers:", ", ".join(p.name for p in find_all("cricketer")))
nulls = db.execute("""select sum((club is null) + (runs is null) + (wickets is null)),
                      3 * count(*) from player""").fetchone()
print(f"null cells in subclass columns: {nulls[0]} of {nulls[1]}")
```

```output
row: 1 footballer Ann Lions null null
row: 2 cricketer Bo null 412 null
row: 3 bowler Cy null 95 31
loaded: Footballer Ann, club Lions
loaded: Cricketer Bo, runs 412
loaded: Bowler Cy, runs 95, wickets 31
cricketers: Bo, Cy
null cells in subclass columns: 5 of 9
```

- Verdict: idiom. `TYPE_OF` maps each concrete class to its code in the `type` column, and `load` reads the code to choose the class. `getattr(p, "club", None)` gives null for a field that the class does not have.
- `find_all("cricketer")` selects the codes of `Cricketer` and its subclass `Bowler`, as a polymorphic query does.
