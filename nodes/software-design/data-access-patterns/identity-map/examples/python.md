```python run id=one-object-per-row
import gc
import sqlite3
import weakref


class Person:
    def __init__(self, id, name, city):
        self.id, self.name, self.city = id, name, city


class PersonMapper:
    def __init__(self, db):
        self.db = db
        # The map does not keep objects alive. When the program drops the last
        # reference to an object, its entry goes away.
        self.identity_map = weakref.WeakValueDictionary()
        self.selects = 0

    def find(self, id):
        person = self.identity_map.get(id)
        if person is not None:
            return person
        self.selects += 1
        row = self.db.execute("SELECT id, name, city FROM people WHERE id = ?", (id,)).fetchone()
        return None if row is None else self._load(row)

    def find_by_city(self, city):
        self.selects += 1
        rows = self.db.execute("SELECT id, name, city FROM people WHERE city = ? ORDER BY id", (city,))
        return [self._load(row) for row in rows]

    def _load(self, row):
        person = self.identity_map.get(row[0])
        if person is None:
            person = Person(*row)
            self.identity_map[row[0]] = person
        return person


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, city TEXT)")
db.executemany("INSERT INTO people VALUES (?, ?, ?)", [(1, "Ann", "Oslo"), (2, "Bo", "Oslo"), (3, "Cy", "Bergen")])

mapper = PersonMapper(db)
a, b = mapper.find(1), mapper.find(1)
print("a is b:", a is b, "| selects:", mapper.selects)
oslo = mapper.find_by_city("Oslo")
print("Oslo:", ", ".join(p.name for p in oslo), "| oslo[0] is a:", oslo[0] is a, "| selects:", mapper.selects)
a.city = "Rome"
print("b.city:", b.city)
print("Oslo again:", ", ".join(f"{p.name} in {p.city}" for p in mapper.find_by_city("Oslo")))
print("another session: same object:", PersonMapper(db).find(1) is a)

print("keys in the map:", sorted(mapper.identity_map.keys()))
del a, b, oslo  # the program no longer uses these objects
gc.collect()
print("keys in the map:", sorted(mapper.identity_map.keys()))
print("find(1) again:", mapper.find(1).city, "| selects:", mapper.selects)
```

```output
a is b: True | selects: 1
Oslo: Ann, Bo | oslo[0] is a: True | selects: 2
b.city: Rome
Oslo again: Ann in Rome, Bo in Oslo
another session: same object: False
keys in the map: [1, 2]
keys in the map: []
find(1) again: Oslo | selects: 4
```

- Verdict: idiom. A `weakref.WeakValueDictionary` does not keep its values alive, so the map holds only the objects that the program still uses. SQLAlchemy keeps the objects of a Session in the same way.
- After `del`, the map is empty, and the next `find(1)` runs a query and returns a new object with the city from the table. The change to `Rome` was never saved, so it is gone. SQLAlchemy keeps a strong reference to an object that has pending changes for this reason.
