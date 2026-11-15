```python run id=two-sessions-one-customer
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, credit integer, version integer);
insert into customer values (1, 'Acme Ltd', 500, 1);
""")


class ConflictError(Exception):
    pass


# Each session reads the record in one system transaction and saves it in a later one.
# The version that it read goes with its copy of the data.
def load(customer_id):
    name, credit, version = db.execute(
        "select name, credit, version from customer where id = ?", (customer_id,)).fetchone()
    return {"id": customer_id, "name": name, "credit": credit, "version": version}


def save(copy):
    with db:  # the check and the update run in one system transaction
        cur = db.execute(
            "update customer set name = ?, credit = ?, version = version + 1 where id = ? and version = ?",
            (copy["name"], copy["credit"], copy["id"], copy["version"]))
        if cur.rowcount == 0:
            raise ConflictError(f"customer {copy['id']} was changed by another session")
    copy["version"] += 1


def show(c):
    return f"{c['name']}, credit {c['credit']}, version {c['version']}"


def try_save(who, copy, what):
    try:
        save(copy)
        print(f"{who} saves {what}: ok, version {copy['version']}")
        return True
    except ConflictError as e:
        print(f"{who} saves {what}: conflict: {e}")
        return False


ann = load(1)
print("ann loads customer 1:", show(ann))
bob = load(1)
print("bob loads customer 1:", show(bob))

bob["credit"] = 800
try_save("bob", bob, "credit 800")

ann["name"] = "Acme Group"
if not try_save("ann", ann, "name Acme Group"):
    ann = load(1)  # the session reloads, shows the new data, and the user does the edit again
    print("ann reloads:", show(ann))
    ann["name"] = "Acme Group"
    try_save("ann", ann, "name Acme Group")

print("final:", show(load(1)))
```

```output
ann loads customer 1: Acme Ltd, credit 500, version 1
bob loads customer 1: Acme Ltd, credit 500, version 1
bob saves credit 800: ok, version 2
ann saves name Acme Group: conflict: customer 1 was changed by another session
ann reloads: Acme Ltd, credit 800, version 2
ann saves name Acme Group: ok, version 3
final: Acme Group, credit 800, version 3
```

- Verdict: idiom. The version is a column of the table. `save` runs one `UPDATE` with `version = ?` in its `WHERE` clause, inside `with db:`, so the check and the write are one step. When `rowcount` is 0, another session changed the row, and `save` raises `ConflictError`.
- The session keeps the version with its copy of the data between the requests. In a web application, the version goes into a hidden field of the form.
