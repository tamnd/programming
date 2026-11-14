```python run id=transfer-and-close
import sqlite3
from dataclasses import dataclass


@dataclass
class Account:
    id: int
    owner: str
    balance: int


@dataclass
class Entry:
    account_id: int
    amount: int


class AccountMapper:
    def find(self, db, id):
        return Account(*db.execute("SELECT id, owner, balance FROM accounts WHERE id = ?", (id,)).fetchone())

    def update(self, db, a):
        print(f"  update accounts {a.id} {a.owner} {a.balance}")
        db.execute("UPDATE accounts SET owner = ?, balance = ? WHERE id = ?", (a.owner, a.balance, a.id))

    def delete(self, db, a):
        print(f"  delete accounts {a.id}")
        db.execute("DELETE FROM accounts WHERE id = ?", (a.id,))


class EntryMapper:
    def insert(self, db, e):
        print(f"  insert entries ({e.account_id}, {e.amount})")
        db.execute("INSERT INTO entries (account_id, amount) VALUES (?, ?)", (e.account_id, e.amount))


class UnitOfWork:
    def __init__(self, db, mappers):
        self.db, self.mappers = db, mappers
        self.new, self.dirty, self.removed = [], [], []

    @staticmethod
    def _has(objs, obj):
        return any(o is obj for o in objs)

    def register_new(self, obj):
        self.new.append(obj)

    def register_dirty(self, obj):
        if not self._has(self.new, obj) and not self._has(self.dirty, obj):
            self.dirty.append(obj)

    def register_removed(self, obj):
        if self._has(self.new, obj):  # never written, so there is nothing to delete
            self.new = [o for o in self.new if o is not obj]
            return
        self.dirty = [o for o in self.dirty if o is not obj]  # no UPDATE before the DELETE
        self.removed.append(obj)

    def commit(self):
        try:
            with self.db:  # one transaction: COMMIT at the end, ROLLBACK if a statement fails
                for obj in self.new:
                    self.mappers[type(obj)].insert(self.db, obj)
                for obj in self.dirty:
                    self.mappers[type(obj)].update(self.db, obj)
                for obj in self.removed:
                    self.mappers[type(obj)].delete(self.db, obj)
            print("  commit")
        except sqlite3.IntegrityError as e:
            print("  rollback:", e)
        finally:
            self.new, self.dirty, self.removed = [], [], []


def transfer(uow, source, target, amount):
    source.balance -= amount
    target.balance += amount
    uow.register_dirty(source)
    uow.register_dirty(target)
    uow.register_new(Entry(source.id, -amount))
    uow.register_new(Entry(target.id, amount))


def show(db):
    print("accounts:", " | ".join(f"{i} {o} {b}" for i, o, b in db.execute("SELECT * FROM accounts ORDER BY id")))
    print("entries:", " | ".join(f"{a} {m}" for a, m in db.execute("SELECT account_id, amount FROM entries ORDER BY id")))


db = sqlite3.connect(":memory:")
db.executescript("""
CREATE TABLE accounts (id INTEGER PRIMARY KEY, owner TEXT, balance INTEGER CHECK (balance >= 0));
CREATE TABLE entries (id INTEGER PRIMARY KEY, account_id INTEGER, amount INTEGER);
INSERT INTO accounts VALUES (1, 'ann', 100), (2, 'bob', 0);
""")
accounts = AccountMapper()
mappers = {Account: accounts, Entry: EntryMapper()}

# Each business transaction loads fresh objects and has its own unit of work.
uow = UnitOfWork(db, mappers)
transfer(uow, accounts.find(db, 1), accounts.find(db, 2), 30)
print("pending:", len(uow.new), "new,", len(uow.dirty), "dirty; entries in the table:", db.execute("SELECT count(*) FROM entries").fetchone()[0])
print("commit 1")
uow.commit()

uow = UnitOfWork(db, mappers)
transfer(uow, accounts.find(db, 2), accounts.find(db, 1), 50)  # bob has only 30
print("commit 2")
uow.commit()
show(db)

uow = UnitOfWork(db, mappers)
ann, bob = accounts.find(db, 1), accounts.find(db, 2)
transfer(uow, bob, ann, 30)  # close bob's account: move the money, then remove it
uow.register_removed(bob)
print("commit 3")
uow.commit()
show(db)
```

```output
pending: 2 new, 2 dirty; entries in the table: 0
commit 1
  insert entries (1, -30)
  insert entries (2, 30)
  update accounts 1 ann 70
  update accounts 2 bob 30
  commit
commit 2
  insert entries (2, -50)
  insert entries (1, 50)
  update accounts 2 bob -20
  rollback: CHECK constraint failed: balance >= 0
accounts: 1 ann 70 | 2 bob 30
entries: 1 -30 | 2 30
commit 3
  insert entries (2, -30)
  insert entries (1, 30)
  update accounts 1 ann 100
  delete accounts 2
  commit
accounts: 1 ann 100
entries: 1 -30 | 2 30 | 2 -30 | 1 30
```

- Verdict: idiom. `with self.db:` makes the commit one SQLite transaction: it commits when the block ends and rolls back when a statement raises an exception.
- `register_removed` drops a pending `UPDATE` of the same object, so commit 3 writes a `DELETE` and no `UPDATE` for account 2. The lists compare objects with `is`, because two `Entry` objects with the same fields are equal under `==` but are two rows.
