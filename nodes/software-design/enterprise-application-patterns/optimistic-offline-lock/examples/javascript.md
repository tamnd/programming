```javascript run id=two-sessions-one-customer
class ConflictError extends Error {}

// The table, kept in memory. Each row has a version number.
const table = new Map([[1, { name: "Acme Ltd", credit: 500, version: 1 }]]);

// Each session reads the record in one system transaction and saves it in a later one.
// The version that it read goes with its copy of the data.
function load(id) {
  return { id, ...table.get(id) };
}

function save(copy) {
  // The check and the update must be one step, like UPDATE ... WHERE version = ? in SQL.
  const row = table.get(copy.id);
  if (row.version !== copy.version) throw new ConflictError(`customer ${copy.id} was changed by another session`);
  table.set(copy.id, { name: copy.name, credit: copy.credit, version: row.version + 1 });
  copy.version++;
}

const show = (c) => `${c.name}, credit ${c.credit}, version ${c.version}`;

function trySave(who, copy, what) {
  try {
    save(copy);
    console.log(`${who} saves ${what}: ok, version ${copy.version}`);
    return true;
  } catch (e) {
    if (!(e instanceof ConflictError)) throw e;
    console.log(`${who} saves ${what}: conflict: ${e.message}`);
    return false;
  }
}

let ann = load(1);
console.log("ann loads customer 1:", show(ann));
const bob = load(1);
console.log("bob loads customer 1:", show(bob));

bob.credit = 800;
trySave("bob", bob, "credit 800");

ann.name = "Acme Group";
if (!trySave("ann", ann, "name Acme Group")) {
  ann = load(1); // the session reloads, shows the new data, and the user does the edit again
  console.log("ann reloads:", show(ann));
  ann.name = "Acme Group";
  trySave("ann", ann, "name Acme Group");
}

console.log("final:", show(load(1)));
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

- Verdict: idiom. The table is a `Map` in memory, and `save` checks the version and writes the row with no `await` between the two steps, so no other code can run between them. With a database, the same check is an `UPDATE` with the version in the `WHERE` clause.
- The session's copy is a new object made with the spread syntax, so a change to the copy does not change the table.
