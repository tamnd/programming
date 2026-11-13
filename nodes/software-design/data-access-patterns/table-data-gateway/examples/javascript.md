```javascript run id=products-gateway
// A stand-in for one database table: rows by id and an id counter.
class Table {
  constructor() {
    this.rows = new Map();
    this.nextId = 1;
  }
}

// The gateway holds every operation on the products table.
// It returns plain row records, never domain objects.
class ProductGateway {
  constructor(table) {
    this.table = table;
  }

  find(id) {
    const row = this.table.rows.get(id);
    return row ? { ...row } : null;
  }

  findAll() {
    return [...this.table.rows.values()].sort((a, b) => a.id - b.id).map((r) => ({ ...r }));
  }

  findLowStock(limit) {
    return this.findAll().filter((r) => r.stock < limit);
  }

  insert(name, price, stock) {
    const id = this.table.nextId++;
    this.table.rows.set(id, { id, name, price, stock });
    return id;
  }

  updateStock(id, stock) {
    const row = this.table.rows.get(id);
    if (row) row.stock = stock;
  }

  delete(id) {
    this.table.rows.delete(id);
  }
}

// A transaction script: the rule is here, the data access is in the gateway.
function restock(products, limit, amount) {
  const done = [];
  for (const row of products.findLowStock(limit)) {
    products.updateStock(row.id, row.stock + amount);
    done.push(`${row.name} ${row.stock} -> ${row.stock + amount}`);
  }
  return done;
}

const products = new ProductGateway(new Table());
products.insert("pen", 120, 3);
const ink = products.insert("ink", 450, 40);
products.insert("pad", 300, 0);
console.log("restocked: " + restock(products, 5, 20).join("; "));
products.delete(ink);
console.log("rows: " + products.findAll().map((r) => `${r.id} ${r.name} ${r.price} ${r.stock}`).join(" | "));
console.log("find(9): " + (products.find(9) ?? "none"));
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. A class with one method for each operation on the table. The standard library has no database driver, so a `Map` of rows stands in for the table.
- The finders return copies made with spread syntax, so a caller cannot change the table except through a gateway method.
