```javascript run id=lock-table-with-timeout
class LockError extends Error {}

// The lock manager keeps the locks in a table, so that they last across requests.
// In a real application, the table is in the database and the resource is its primary key.
class LockManager {
  static TIMEOUT = 30; // minutes: a lock that is older than this has been abandoned
  #table = new Map(); // resource -> { owner, taken }

  acquire(resource, owner, now) {
    const lock = this.#table.get(resource);
    if (lock && lock.taken <= now - LockManager.TIMEOUT) this.#table.delete(resource);
    const holder = this.#table.get(resource);
    if (!holder) this.#table.set(resource, { owner, taken: now });
    else if (holder.owner !== owner) throw new LockError(`${resource} is locked by ${holder.owner}`);
  }

  releaseAll(owner) {
    for (const [resource, lock] of this.#table) if (lock.owner === owner) this.#table.delete(resource);
  }

  held() {
    return [...this.#table].sort(([a], [b]) => a.localeCompare(b)).map(([r, l]) => `${r} by ${l.owner}`);
  }
}

const locks = new LockManager();
const clock = (minute) => `10:${String(minute).padStart(2, "0")}`;

function openForEdit(minute, who, resource) {
  try {
    locks.acquire(resource, who, minute);
    console.log(`${clock(minute)} ${who} opens ${resource} for edit: lock granted`);
  } catch (e) {
    if (!(e instanceof LockError)) throw e;
    console.log(`${clock(minute)} ${who} opens ${resource} for edit: refused: ${e.message}`);
  }
}

function close(minute, who) {
  locks.releaseAll(who);
  console.log(`${clock(minute)} ${who} saves and closes: locks released`);
}

openForEdit(0, "ann", "customer 1");
openForEdit(5, "bob", "customer 1");
close(12, "ann");
openForEdit(15, "bob", "customer 1");
openForEdit(20, "bob", "order 9");
// Bob closes the browser and never comes back.
openForEdit(40, "carol", "customer 1");
openForEdit(50, "carol", "customer 1");
console.log("locks held:", locks.held().join(", "));
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

- Verdict: idiom. The lock table is a `Map` in a private field of the lock manager. `acquire` has no `await` between the check and the take, so no other code can run between the two steps. With a database, the lock table would have the resource as its primary key.
- `releaseAll` removes all the locks of one owner when the business transaction ends, for example when the user saves or cancels.
