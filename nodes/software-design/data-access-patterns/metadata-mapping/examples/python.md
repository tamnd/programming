```python run id=generic-mapper-from-metadata
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table player (id integer primary key, name text, nation text);
create table club (id integer primary key, title text, city text);
""")


class Player:
    def __init__(self, id=None, name=None, country=None):
        self.id, self.name, self.country = id, name, country

    def __repr__(self):
        return f"Player(id={self.id}, name={self.name}, country={self.country})"


class Club:
    def __init__(self, id=None, name=None, city=None):
        self.id, self.name, self.city = id, name, city

    def __repr__(self):
        return f"Club(id={self.id}, name={self.name}, city={self.city})"


class DataMap:
    # The metadata: the class, the table and the pairs of field and column.
    def __init__(self, cls, table, columns):
        self.cls, self.table, self.columns = cls, table, columns

    def describe(self):
        pairs = ", ".join(f"{f} -> {c}" for f, c in self.columns)
        return f"{self.cls.__name__} -> table {self.table}: {pairs}"


MAPS = {
    Player: DataMap(Player, "player", [("id", "id"), ("name", "name"), ("country", "nation")]),
    Club: DataMap(Club, "club", [("id", "id"), ("name", "title"), ("city", "city")]),
}


class Mapper:
    # Generic code: it reads the metadata and uses reflection on the fields.
    def insert(self, obj):
        m = MAPS[type(obj)]
        cols = ", ".join(c for _, c in m.columns)
        marks = ", ".join("?" for _ in m.columns)
        sql = f"insert into {m.table} ({cols}) values ({marks})"
        db.execute(sql, [getattr(obj, f) for f, _ in m.columns])
        return sql

    def find(self, cls, id):
        m = MAPS[cls]
        sql = f"select {', '.join(c for _, c in m.columns)} from {m.table} where id = ?"
        row = db.execute(sql, (id,)).fetchone()
        obj = cls()
        for (f, _), value in zip(m.columns, row):
            setattr(obj, f, value)
        return obj

    def update(self, obj):
        m = MAPS[type(obj)]
        sets = ", ".join(f"{c} = ?" for f, c in m.columns if f != "id")
        sql = f"update {m.table} set {sets} where id = ?"
        db.execute(sql, [getattr(obj, f) for f, _ in m.columns if f != "id"] + [obj.id])
        return sql


mapper = Mapper()
for m in MAPS.values():
    print("map", m.describe())
print("insert:", mapper.insert(Player(1, "Ann", "NZ")))
print("insert:", mapper.insert(Club(1, "Lions", "Leeds")))
p = mapper.find(Player, 1)
print("find player 1:", p)
print("find club 1:", mapper.find(Club, 1))
p.country = "AU"
print("update:", mapper.update(p))
for table in ("player", "club"):
    for row in db.execute(f"select * from {table} order by id"):
        print(f"row in {table}:", " ".join(str(v) for v in row))
```

```output
map Player -> table player: id -> id, name -> name, country -> nation
map Club -> table club: id -> id, name -> title, city -> city
insert: insert into player (id, name, nation) values (?, ?, ?)
insert: insert into club (id, title, city) values (?, ?, ?)
find player 1: Player(id=1, name=Ann, country=NZ)
find club 1: Club(id=1, name=Lions, city=Leeds)
update: update player set name = ?, nation = ? where id = ?
row in player: 1 Ann AU
row in club: 1 Lions Leeds
```

- Verdict: idiom. `DataMap` holds the class, the table and the pairs of field and column, and `Mapper` uses `getattr` and `setattr` to move each value. The SQL text comes from the metadata, and SQLite runs it.
- SQLAlchemy and Django keep the same kind of metadata in a `Mapper` and in `Model._meta`, and build the SQL from it.
