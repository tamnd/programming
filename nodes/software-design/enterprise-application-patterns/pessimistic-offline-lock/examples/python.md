```python run id=lock-table-with-timeout
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table app_lock (resource text primary key, owner text, taken integer)")


class LockError(Exception):
    pass


# The lock manager keeps the locks in a table, so that they last across requests.
# The primary key makes the insert fail when another session holds the lock.
class LockManager:
    TIMEOUT = 30  # minutes: a lock that is older than this has been abandoned

    def acquire(self, resource, owner, now):
        with db:  # one system transaction for each step
            db.execute("delete from app_lock where resource = ? and taken <= ?", (resource, now - self.TIMEOUT))
            try:
                db.execute("insert into app_lock values (?, ?, ?)", (resource, owner, now))
            except sqlite3.IntegrityError:
                (holder,) = db.execute("select owner from app_lock where resource = ?", (resource,)).fetchone()
                if holder != owner:
                    raise LockError(f"{resource} is locked by {holder}") from None

    def release_all(self, owner):
        with db:
            db.execute("delete from app_lock where owner = ?", (owner,))


locks = LockManager()


def clock(minute):
    return f"10:{minute:02d}"


def open_for_edit(minute, who, resource):
    try:
        locks.acquire(resource, who, minute)
        print(f"{clock(minute)} {who} opens {resource} for edit: lock granted")
    except LockError as e:
        print(f"{clock(minute)} {who} opens {resource} for edit: refused: {e}")


def close(minute, who):
    locks.release_all(who)
    print(f"{clock(minute)} {who} saves and closes: locks released")


open_for_edit(0, "ann", "customer 1")
open_for_edit(5, "bob", "customer 1")
close(12, "ann")
open_for_edit(15, "bob", "customer 1")
open_for_edit(20, "bob", "order 9")
# Bob closes the browser and never comes back.
open_for_edit(40, "carol", "customer 1")
open_for_edit(50, "carol", "customer 1")
rows = db.execute("select resource, owner from app_lock order by resource").fetchall()
print("locks held:", ", ".join(f"{r} by {o}" for r, o in rows))
```

```output
10:00 ann opens customer 1 for edit: lock granted
10:05 bob opens customer 1 for edit: refused: customer 1 is locked by ann
10:12 ann saves and closes: locks released
10:15 bob opens customer 1 for edit: lock granted
10:20 bob opens order 9 for edit: lock granted
10:40 carol opens customer 1 for edit: refused: customer 1 is locked by bob
10:50 carol opens customer 1 for edit: lock granted
locks held: customer 1 by carol, order 9 by bob
```

- Verdict: idiom. The lock table is a table in the database, so the locks last across requests and across the processes of the application. The primary key on `resource` makes the `insert` fail when another session holds the lock, so the check and the take are one step.
- Each call of `acquire` first deletes a lock that is older than the timeout, in the same system transaction. This frees the locks of sessions that were abandoned.
