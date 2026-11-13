```javascript run id=product-rows
// A stand-in for one database table: rows by id and an id counter.
class Table {
  constructor() {
    this.rows = new Map();
    this.nextId = 1;
  }
}

// A gateway to one row: the columns are fields, and the row saves itself.
class ProductRow {
  constructor(table, name, price, stock, id = null) {
    this.table = table;
    Object.assign(this, { id, name, price, stock });
  }

  record() {
    return { id: this.id, name: this.name, price: this.price, stock: this.stock };
  }

  insert() {
    this.id = this.table.nextId++;
    this.table.rows.set(this.id, this.record());
    return this.id;
  }

  update() {
    this.table.rows.set(this.id, this.record());
  }

  delete() {
    this.table.rows.delete(this.id);
  }

  toString() {
    return `${this.id} ${this.name} ${this.price} ${this.stock}`;
  }
}

// Reads rows and makes one ProductRow for each of them.
class ProductFinder {
  constructor(table) {
    this.table = table;
  }

  load(r) {
    return new ProductRow(this.table, r.name, r.price, r.stock, r.id);
  }

  find(id) {
    const r = this.table.rows.get(id);
    return r ? this.load(r) : null;
  }

  findAll() {
    return [...this.table.rows.values()].sort((a, b) => a.id - b.id).map((r) => this.load(r));
  }

  findLowStock(limit) {
    return this.findAll().filter((p) => p.stock < limit);
  }
}

function restock(finder, limit, amount) {
  const done = [];
  for (const product of finder.findLowStock(limit)) {
    const before = product.stock;
    product.stock += amount;
    product.update();
    done.push(`${product.name} ${before} -> ${product.stock}`);
  }
  return done;
}

const table = new Table();
new ProductRow(table, "pen", 120, 3).insert();
const ink = new ProductRow(table, "ink", 450, 40).insert();
new ProductRow(table, "pad", 300, 0).insert();
const finder = new ProductFinder(table);
console.log("restocked: " + restock(finder, 5, 20).join("; "));
finder.find(ink).delete();
console.log("rows: " + finder.findAll().join(" | "));
console.log("find(9): " + (finder.find(9) ?? "none"));
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. `Object.assign` copies the columns into fields, and `record()` copies them back out for the table.
- The standard library has no database driver, so a `Map` of rows stands in for the table.
