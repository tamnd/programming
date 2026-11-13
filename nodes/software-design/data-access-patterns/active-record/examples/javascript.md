```javascript run id=product-record
// A stand-in for the database: one table of rows by id, with an id counter.
// The JavaScript standard library has no database driver.
class Table {
  constructor() {
    this.rows = new Map();
    this.nextId = 1;
  }
}

// The base class of Active Records. A subclass names its table and columns.
class Record {
  constructor(values, id = null) {
    this.id = id;
    for (const column of this.constructor.columns) this[column] = values[column];
  }

  static find(id) {
    const row = this.table.rows.get(id);
    return row ? new this(row, id) : null;
  }

  static all() {
    return [...this.table.rows.keys()].sort((a, b) => a - b).map((id) => this.find(id));
  }

  validate() {} // a subclass throws here when the object must not be saved

  save() {
    this.validate();
    const table = this.constructor.table;
    if (this.id === null) this.id = table.nextId++;
    const row = {};
    for (const column of this.constructor.columns) row[column] = this[column];
    table.rows.set(this.id, row);
  }

  delete() {
    this.constructor.table.rows.delete(this.id);
  }
}

// One row of the products table, with the domain logic of a product.
class Product extends Record {
  static table = new Table();
  static columns = ["name", "price", "stock"];

  validate() {
    if (this.stock < 0) throw new Error(`${this.name}: stock cannot be negative`);
  }

  needsRestock() {
    return this.stock < 5;
  }

  sell(quantity) {
    if (quantity > this.stock) throw new Error(`${this.name}: only ${this.stock} in stock`);
    this.stock -= quantity;
    return quantity * this.price;
  }

  toString() {
    return `${this.id} ${this.name} ${this.stock}`;
  }
}

for (const [name, price, stock] of [["pen", 120, 3], ["ink", 450, 40], ["pad", 300, 0]]) {
  new Product({ name, price, stock }).save();
}

const pen = Product.find(1);
console.log("sold 2 pens for " + pen.sell(2));
pen.save();
const restocked = [];
for (const product of Product.all()) {
  if (product.needsRestock()) {
    product.stock += 20;
    product.save();
    restocked.push(product.name);
  }
}
console.log("restocked: " + restocked.join(", "));
console.log("rows: " + Product.all().join(" | "));
const ink = Product.find(2);
ink.stock -= 45;
try {
  ink.save();
} catch (e) {
  console.log("not saved: " + e.message);
}
console.log("find(9): " + (Product.find(9) ?? "none"));
```

```output
sold 2 pens for 240
restocked: pen, pad
rows: 1 pen 21 | 2 ink 40 | 3 pad 20
not saved: ink: stock cannot be negative
find(9): none
```

- Verdict: idiom. Static methods on the class are the finders, and `this.constructor` lets the base class reach the table and columns of the subclass.
- The standard library has no database driver, so a `Map` of rows stands in for the table.
