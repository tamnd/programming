```javascript run id=transfer-and-close
class Account {
  constructor(id, owner, balance) {
    Object.assign(this, { id, owner, balance });
  }
}

class Entry {
  constructor(accountId, amount) {
    Object.assign(this, { accountId, amount });
  }
}

// A stand-in for the database, because Node has no SQL driver in its standard library.
// transaction() keeps a copy of the tables and puts it back when a statement fails.
class Database {
  accounts = new Map([[1, { owner: "ann", balance: 100 }], [2, { owner: "bob", balance: 0 }]]);
  entries = [];

  transaction(work) {
    const saved = [new Map(this.accounts), [...this.entries]];
    try {
      work();
    } catch (e) {
      [this.accounts, this.entries] = saved;
      throw e;
    }
  }

  updateAccount(id, owner, balance) {
    if (balance < 0) throw new Error("CHECK constraint failed: balance >= 0");
    this.accounts.set(id, { owner, balance });
  }
}

const accountMapper = {
  find: (db, id) => new Account(id, db.accounts.get(id).owner, db.accounts.get(id).balance),
  update(db, a) {
    console.log(`  update accounts ${a.id} ${a.owner} ${a.balance}`);
    db.updateAccount(a.id, a.owner, a.balance);
  },
  delete(db, a) {
    console.log(`  delete accounts ${a.id}`);
    db.accounts.delete(a.id);
  },
};

const entryMapper = {
  insert(db, e) {
    console.log(`  insert entries (${e.accountId}, ${e.amount})`);
    db.entries.push({ accountId: e.accountId, amount: e.amount });
  },
};

class UnitOfWork {
  #new = [];
  #dirty = [];
  #removed = [];

  constructor(db, mappers) {
    this.db = db;
    this.mappers = mappers;
  }

  get pending() {
    return `${this.#new.length} new, ${this.#dirty.length} dirty`;
  }

  registerNew(obj) {
    this.#new.push(obj);
  }

  registerDirty(obj) {
    if (!this.#new.includes(obj) && !this.#dirty.includes(obj)) this.#dirty.push(obj);
  }

  registerRemoved(obj) {
    if (this.#new.includes(obj)) {
      this.#new = this.#new.filter((o) => o !== obj); // never written, nothing to delete
      return;
    }
    this.#dirty = this.#dirty.filter((o) => o !== obj); // no update before the delete
    this.#removed.push(obj);
  }

  commit() {
    const mapper = (obj) => this.mappers.get(obj.constructor);
    try {
      this.db.transaction(() => {
        for (const obj of this.#new) mapper(obj).insert(this.db, obj);
        for (const obj of this.#dirty) mapper(obj).update(this.db, obj);
        for (const obj of this.#removed) mapper(obj).delete(this.db, obj);
      });
      console.log("  commit");
    } catch (e) {
      console.log(`  rollback: ${e.message}`);
    } finally {
      this.#new = [];
      this.#dirty = [];
      this.#removed = [];
    }
  }
}

function transfer(uow, source, target, amount) {
  source.balance -= amount;
  target.balance += amount;
  uow.registerDirty(source);
  uow.registerDirty(target);
  uow.registerNew(new Entry(source.id, -amount));
  uow.registerNew(new Entry(target.id, amount));
}

function show(db) {
  console.log("accounts: " + [...db.accounts].sort((a, b) => a[0] - b[0]).map(([id, r]) => `${id} ${r.owner} ${r.balance}`).join(" | "));
  console.log("entries: " + db.entries.map((e) => `${e.accountId} ${e.amount}`).join(" | "));
}

const db = new Database();
const mappers = new Map([[Account, accountMapper], [Entry, entryMapper]]);

let uow = new UnitOfWork(db, mappers);
transfer(uow, accountMapper.find(db, 1), accountMapper.find(db, 2), 30);
console.log(`pending: ${uow.pending}; entries in the table: ${db.entries.length}`);
console.log("commit 1");
uow.commit();

uow = new UnitOfWork(db, mappers);
transfer(uow, accountMapper.find(db, 2), accountMapper.find(db, 1), 50); // bob has only 30
console.log("commit 2");
uow.commit();
show(db);

uow = new UnitOfWork(db, mappers);
const ann = accountMapper.find(db, 1);
const bob = accountMapper.find(db, 2);
transfer(uow, bob, ann, 30); // close bob's account: move the money, then remove it
uow.registerRemoved(bob);
console.log("commit 3");
uow.commit();
show(db);
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

- Verdict: idiom. The lists are private fields (`#new`, `#dirty`, `#removed`), and `includes` compares objects by reference, so two entries with the same fields are two rows.
- The mappers are found by the constructor of the object, in a `Map` from class to mapper.
