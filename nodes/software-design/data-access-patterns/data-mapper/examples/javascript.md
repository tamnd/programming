```javascript run id=product-mapper
// A domain object. It has no storage code and no idea that a database exists.
class Product {
  constructor(name, price, stock, tags = [], id = null) {
    Object.assign(this, { name, price, stock, tags, id });
  }

  sell(quantity) {
    if (quantity > this.stock) throw new Error(`${this.name}: only ${this.stock} in stock`);
    this.stock -= quantity;
    return quantity * this.price;
  }

  toString() {
    return `${this.id} ${this.name} ${this.price} ${this.stock} [${this.tags.join(", ")}]`;
  }
}

// A stand-in for the database, because the standard library has no driver.
// Two tables with their own column names: products and product_tags.
const db = { products: new Map(), productTags: [], nextId: 1 };

// The mapper moves products between objects and the two tables.
class ProductMapper {
  constructor(db) {
    this.db = db;
  }

  find(id) {
    const row = this.db.products.get(id);
    if (!row) return null;
    const tags = this.db.productTags.filter((t) => t.productId === id).map((t) => t.tag).sort();
    return new Product(row.title, row.priceCents, row.qty, tags, id);
  }

  insert(p) {
    p.id = this.db.nextId++;
    this.update(p);
  }

  update(p) {
    this.db.products.set(p.id, { title: p.name, priceCents: p.price, qty: p.stock });
    this.deleteTags(p.id);
    for (const tag of p.tags) this.db.productTags.push({ productId: p.id, tag });
  }

  delete(p) {
    this.deleteTags(p.id);
    this.db.products.delete(p.id);
  }

  deleteTags(id) {
    this.db.productTags = this.db.productTags.filter((t) => t.productId !== id);
  }
}

const mapper = new ProductMapper(db);
mapper.insert(new Product("pen", 120, 3, ["office"]));
const ink = new Product("ink", 450, 40, ["office"]);
mapper.insert(ink);

const pen = mapper.find(1);
console.log("sold: " + pen.sell(2));
pen.tags.push("sale");
mapper.update(pen);
console.log("object: " + mapper.find(1));
const rows = [...db.products].sort((a, b) => a[0] - b[0]).map(([id, r]) => `${id} ${r.title} ${r.priceCents} ${r.qty}`);
console.log("products: " + rows.join(" | "));
const tagRows = db.productTags.map((t) => `${t.productId} ${t.tag}`).sort();
console.log("product_tags: " + tagRows.join(" | "));
mapper.delete(ink);
console.log(`after delete: ${db.products.size} product, ${db.productTags.length} tags`);
console.log("find(2): " + (mapper.find(2) ?? "none"));
```

```output
sold: 240
object: 1 pen 120 1 [office, sale]
products: 1 pen 120 1 | 2 ink 450 40
product_tags: 1 office | 1 sale | 2 office
after delete: 1 product, 2 tags
find(2): none
```

- Verdict: idiom. `Product` is a plain class, and `ProductMapper` copies its fields to and from the stored rows.
- The standard library has no database driver, so an object with a `Map` and an array stands in for the two tables.
