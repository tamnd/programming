```javascript wrong id=template-literal
import { DatabaseSync } from "node:sqlite";

const db = new DatabaseSync(":memory:");
db.exec("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)");
const insert = db.prepare("INSERT INTO orders VALUES (?, ?, ?)");
insert.run(1, "alice", 12.5);
insert.run(2, "bob", 40.0);

function findOrders(customer) {
  const query = `SELECT id, customer FROM orders WHERE customer = '${customer}'`;
  return db.prepare(query).all();
}

console.log(JSON.stringify(findOrders("alice")));
console.log(JSON.stringify(findOrders("x' OR '1'='1")));
```

```output for=template-literal
[{"id":1,"customer":"alice"}]
[{"id":1,"customer":"alice"},{"id":2,"customer":"bob"}]
```

```javascript run id=placeholder
import { DatabaseSync } from "node:sqlite";

const db = new DatabaseSync(":memory:");
db.exec("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)");
const insert = db.prepare("INSERT INTO orders VALUES (?, ?, ?)");
insert.run(1, "alice", 12.5);
insert.run(2, "bob", 40.0);

const byCustomer = db.prepare("SELECT id, customer FROM orders WHERE customer = ?");

console.log(JSON.stringify(byCustomer.all("alice")));
console.log(JSON.stringify(byCustomer.all("x' OR '1'='1")));

const byName = db.prepare("SELECT id FROM orders WHERE customer = :customer AND total > :min");
console.log(JSON.stringify(byName.all({ customer: "bob", min: 10 })));
```

```output for=placeholder
[{"id":1,"customer":"alice"}]
[]
[{"id":2}]
```

- Verdict: stdlib. Node.js has the module `node:sqlite`. `prepare` compiles the SQL text, and `all`, `get` and `run` bind the arguments to `?` or to named parameters such as `:customer`.
- A template literal is not a parameterised query. `${customer}` puts the text of the input into the SQL text before `prepare` sees it.
- Database clients on npm, such as `pg` and `mysql2`, take the SQL text and an array of values. The placeholder syntax is `$1` for `pg` and `?` for `mysql2`.
- A tagged template, such as the `sql` tag of some query builders, can be safe: the tag function gets the strings and the values apart and binds the values. A plain template literal does not.
