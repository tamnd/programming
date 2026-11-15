```javascript run id=one-script-per-request
// The tables of the database, kept in memory for this example.
const db = {
  customer: new Map([[1, { name: "Ann", tier: "gold" }], [2, { name: "Bob", tier: "regular" }]]),
  product: new Map([[1, { name: "lamp", priceCents: 2500 }], [2, { name: "desk", priceCents: 7500 }], [3, { name: "pen", priceCents: 500 }]]),
  orders: new Map(),
  orderLine: [],
  nextOrderId: 1,
};

class ScriptError extends Error {}

const cents = (n) => `${Math.floor(n / 100)}.${String(n % 100).padStart(2, "0")}`;

// Transaction script 1: place an order.
function placeOrder(customerId, items) {
  const customer = db.customer.get(customerId);
  if (!customer) throw new ScriptError(`no customer ${customerId}`);
  // Check everything first, so that a failed script writes nothing.
  for (const [productId, qty] of items) {
    if (qty <= 0) throw new ScriptError(`bad quantity ${qty}`);
    if (!db.product.has(productId)) throw new ScriptError(`no product ${productId}`);
  }
  const orderId = db.nextOrderId++;
  db.orders.set(orderId, { customerId });
  for (const [productId, qty] of items) db.orderLine.push({ orderId, productId, qty });
  return [orderId, customer.name];
}

// Transaction script 2: show the total of an order.
function orderTotal(orderId) {
  const order = db.orders.get(orderId);
  if (!order) throw new ScriptError(`no order ${orderId}`);
  const tier = db.customer.get(order.customerId).tier;
  let subtotal = 0;
  for (const line of db.orderLine) {
    if (line.orderId === orderId) subtotal += line.qty * db.product.get(line.productId).priceCents;
  }
  let total = subtotal;
  if (tier === "gold" && subtotal >= 10000) total = subtotal - Math.floor(subtotal / 10);
  return [subtotal, total];
}

for (const [customerId, items] of [[1, [[1, 2], [2, 1]]], [2, [[1, 1], [3, 3]]]]) {
  const [orderId, name] = placeOrder(customerId, items);
  console.log(`placed order ${orderId} for ${name}`);
}
for (const orderId of [1, 2]) {
  const [subtotal, total] = orderTotal(orderId);
  console.log(`order ${orderId}: subtotal ${cents(subtotal)}, total ${cents(total)}`);
}
for (const [customerId, items] of [[1, [[9, 1]]], [2, [[1, 0]]]]) {
  try {
    placeOrder(customerId, items);
  } catch (e) {
    if (!(e instanceof ScriptError)) throw e;
    console.log("error:", e.message);
  }
}
console.log("orders in the database:", db.orders.size);
```

```output
placed order 1 for Ann
placed order 2 for Bob
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
error: no product 9
error: bad quantity 0
orders in the database: 2
```

- Verdict: idiom. Each script is a function. The example has no database, so `placeOrder` checks all the items before it writes, and a failed script leaves the tables as they were. The Python file, with SQLite, gets the same result from a database transaction.
