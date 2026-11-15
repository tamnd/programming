```python run id=graph-in-one-column
import json
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table customer (id integer primary key, name text not null, departments text not null)")


class Department:
    def __init__(self, name, children=()):
        self.name, self.children = name, list(children)

    def to_dict(self):
        return {"name": self.name, "children": [c.to_dict() for c in self.children]}

    @staticmethod
    def from_dict(d):
        return Department(d["name"], [Department.from_dict(c) for c in d["children"]])


def save(id, name, departments):
    # The whole graph of departments is written as one value in one column.
    lob = json.dumps([d.to_dict() for d in departments], separators=(",", ":"))
    db.execute("insert or replace into customer values (?, ?, ?)", (id, name, lob))
    return len(lob.encode())


def load(id):
    name, lob = db.execute("select name, departments from customer where id = ?", (id,)).fetchone()
    return name, [Department.from_dict(d) for d in json.loads(lob)]


def paths(departments, prefix=""):
    for d in departments:
        yield prefix + d.name
        yield from paths(d.children, prefix + d.name + "/")


def ancestors(departments, target, above=()):
    for d in departments:
        if d.name == target:
            return list(reversed(above))
        found = ancestors(d.children, target, above + (d.name,))
        if found is not None:
            return found
    return None


tree = [Department("Sales", [Department("Presales"), Department("Key accounts")]), Department("Support")]
size = save(1, "Acme", tree)
print(f"saved Acme: {size} bytes in one column")
print("column:", db.execute("select departments from customer").fetchone()[0])
name, loaded = load(1)
print(f"loaded {name}:", " | ".join(paths(loaded)))
print("ancestors of Presales:", ", ".join(ancestors(loaded, "Presales")))
loaded[1].children.append(Department("Field"))
size = save(1, name, loaded)
print(f"add Support/Field: {size} bytes written again")
print("rows in customer:", db.execute("select count(*) from customer").fetchone()[0])
```

```output
saved Acme: 136 bytes in one column
column: [{"name":"Sales","children":[{"name":"Presales","children":[]},{"name":"Key accounts","children":[]}]},{"name":"Support","children":[]}]
loaded Acme: Sales | Sales/Presales | Sales/Key accounts | Support
ancestors of Presales: Sales
add Support/Field: 166 bytes written again
rows in customer: 1
```

- Verdict: idiom. `json.dumps` with compact separators writes the whole tree into the `departments` column, and `json.loads` makes it again on load. `to_dict` and `from_dict` turn the classes into plain dictionaries and lists.
- SQLAlchemy and Django do the same step in a column type: `JSON` or `PickleType` in SQLAlchemy, and `JSONField` in Django.
