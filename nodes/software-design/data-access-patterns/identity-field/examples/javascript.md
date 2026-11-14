```javascript run id=insert-then-update
// A stand-in for the database: a table keyed by id, and a counter like AUTOINCREMENT.
const personTable = new Map();
let lastRowId = 0;
// A second table keyed by a meaningful value, the email.
const personByEmail = new Map([["cy@example.com", { name: "Cy" }]]);

class Person {
  constructor(name, email, id = null) {
    this.id = id; // The Identity Field: the primary key of the row.
    this.name = name;
    this.email = email;
  }

  showId() {
    return this.id === null ? "none" : String(this.id);
  }
}

function save(p) {
  if (p.id === null) {
    lastRowId += 1;
    personTable.set(lastRowId, { name: p.name, email: p.email });
    p.id = lastRowId;
    return 1;
  }
  if (!personTable.has(p.id)) return 0;
  personTable.set(p.id, { name: p.name, email: p.email });
  return 1;
}

function find(id) {
  const row = personTable.get(id);
  return new Person(row.name, row.email, id);
}

const ann = new Person("Ann", "ann@example.com");
const bo = new Person("Bo", "bo@example.com");
console.log(`new: Ann id=${ann.showId()} | Bo id=${bo.showId()}`);
save(ann);
save(bo);
console.log(`saved: Ann id=${ann.showId()} | Bo id=${bo.showId()}`);

ann.email = "ann@new.example";
const rows = save(ann);
const row = personTable.get(ann.id);
console.log(`update by id: rows ${rows} | ${ann.id} ${row.name} ${row.email}`);

const b = find(2);
console.log(`find(2): id=${b.id} ${b.name} ${b.email}`);

const first = find(1);
const second = find(1);
const objects = first === second ? "same" : "different";
const ids = first.id === second.id ? "equal" : "different";
console.log(`two loads of row 1: ${objects} objects, ${ids} ids`);

// The same change with a meaningful key: the email is the key.
const cy = new Person("Cy", "cy@example.com");
cy.email = "cy@new.example";
let updated = 0;
if (personByEmail.has(cy.email)) {
  personByEmail.set(cy.email, { name: cy.name });
  updated = 1;
}
console.log(`update by email key: rows ${updated}`);
```

```output
new: Ann id=none | Bo id=none
saved: Ann id=1 | Bo id=2
update by id: rows 1 | 1 Ann ann@new.example
find(2): id=2 Bo bo@example.com
two loads of row 1: different objects, equal ids
update by email key: rows 0
```

- Verdict: idiom. The key field starts as `null`, and the save function sets it from a counter that stands for the generated key. `===` on two loaded objects compares references, so the two loads of row 1 are different objects with equal keys.
- With a database driver, the new key comes from the result of the insert, for example from an `INSERT ... RETURNING id` statement.
