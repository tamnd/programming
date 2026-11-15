```python run id=mapper-for-each-class
import sqlite3

db = sqlite3.connect(":memory:")
db.row_factory = sqlite3.Row
db.execute("""create table player (id integer primary key, type text not null, name text not null,
              club text, runs integer, wickets integer)""")


class Player:
    def __init__(self, name):
        self.id, self.name = None, name


class Footballer(Player):
    def __init__(self, name, club):
        super().__init__(name)
        self.club = club

    def describe(self):
        return f"Footballer {self.name}, club {self.club}"


class Cricketer(Player):
    def __init__(self, name, runs):
        super().__init__(name)
        self.runs = runs

    def describe(self):
        return f"Cricketer {self.name}, runs {self.runs}"


class Bowler(Cricketer):
    def __init__(self, name, runs, wickets):
        super().__init__(name, runs)
        self.wickets = wickets

    def describe(self):
        return f"Bowler {self.name}, runs {self.runs}, wickets {self.wickets}"


class AbstractPlayerMapper:
    # Each mapper class saves and loads the fields that its domain class declares,
    # and calls the mapper of the superclass for the inherited fields.
    def save(self, p, row, log):
        row["name"] = p.name
        log.append("name (Player)")

    def load(self, p, row):
        p.name = row["name"]

    def insert(self, p):
        row, log = {"type": self.type_code}, []
        self.save(p, row, log)
        cur = db.execute(f"insert into player ({', '.join(row)}) values ({', '.join('?' * len(row))})",
                         tuple(row.values()))
        p.id = cur.lastrowid
        return log

    def find(self, row):
        p = self.create()
        p.id = row["id"]
        self.load(p, row)
        return p


class FootballerMapper(AbstractPlayerMapper):
    type_code = "footballer"

    def create(self):
        return Footballer(None, None)

    def save(self, p, row, log):
        super().save(p, row, log)
        row["club"] = p.club
        log.append("club (Footballer)")

    def load(self, p, row):
        super().load(p, row)
        p.club = row["club"]


class CricketerMapper(AbstractPlayerMapper):
    type_code = "cricketer"

    def create(self):
        return Cricketer(None, None)

    def save(self, p, row, log):
        super().save(p, row, log)
        row["runs"] = p.runs
        log.append("runs (Cricketer)")

    def load(self, p, row):
        super().load(p, row)
        p.runs = row["runs"]


class BowlerMapper(CricketerMapper):
    type_code = "bowler"

    def create(self):
        return Bowler(None, None, None)

    def save(self, p, row, log):
        super().save(p, row, log)
        row["wickets"] = p.wickets
        log.append("wickets (Bowler)")

    def load(self, p, row):
        super().load(p, row)
        p.wickets = row["wickets"]


class PlayerMapper:
    # The mapper for the whole hierarchy: it chooses the concrete mapper and delegates to it.
    def __init__(self):
        self.by_type = {m.type_code: m for m in (FootballerMapper(), CricketerMapper(), BowlerMapper())}
        self.by_class = {Footballer: self.by_type["footballer"], Cricketer: self.by_type["cricketer"],
                         Bowler: self.by_type["bowler"]}

    def insert(self, p):
        mapper = self.by_class[type(p)]
        return mapper, mapper.insert(p)

    def find(self, id):
        row = db.execute("select * from player where id = ?", (id,)).fetchone()
        mapper = self.by_type[row["type"]]
        return mapper, mapper.find(row)

    def find_all(self):
        rows = db.execute("select * from player order by id").fetchall()
        return [self.by_type[row["type"]].find(row) for row in rows]


players = PlayerMapper()
for t, m in players.by_type.items():
    print(f"mapper chain for {t}: {', '.join(c.__name__ for c in type(m).__mro__[:-1])}")
for p in (Footballer("Ann", "Lions"), Cricketer("Bo", 412), Bowler("Cy", 95, 31)):
    mapper, log = players.insert(p)
    print(f"insert {p.name}: {type(mapper).__name__} saved {', '.join(log)}")
mapper, p = players.find(3)
print(f"find 3: {p.describe()} (mapper {type(mapper).__name__})")
print("find all:", " | ".join(p.describe() for p in players.find_all()))
```

```output
mapper chain for footballer: FootballerMapper, AbstractPlayerMapper
mapper chain for cricketer: CricketerMapper, AbstractPlayerMapper
mapper chain for bowler: BowlerMapper, CricketerMapper, AbstractPlayerMapper
insert Ann: FootballerMapper saved name (Player), club (Footballer)
insert Bo: CricketerMapper saved name (Player), runs (Cricketer)
insert Cy: BowlerMapper saved name (Player), runs (Cricketer), wickets (Bowler)
find 3: Bowler Cy, runs 95, wickets 31 (mapper BowlerMapper)
find all: Footballer Ann, club Lions | Cricketer Bo, runs 412 | Bowler Cy, runs 95, wickets 31
```

- Verdict: idiom. Each mapper class overrides `save` and `load` and calls `super()` first, so the fields of each class are mapped once. `AbstractPlayerMapper` holds `insert` and `find`, and each concrete mapper gives `type_code` and `create`.
- `PlayerMapper` keeps two maps, from type code and from class to the concrete mapper. `__mro__` gives the chain of mapper classes that the first lines print.
