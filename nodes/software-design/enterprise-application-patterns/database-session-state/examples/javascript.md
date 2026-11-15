```javascript run id=shop-on-two-servers
// The "database" is a set of tables that every server shares.
// A real system keeps these tables in a database server.
const TIMEOUT = 30; // minutes with no request before a session ends

const db = {
  sessions: new Map(), // id -> { user, lastUsed }
  cartLines: [], // { sessionId, item, qty }, the pending data
  orders: [], // { id, user }
  orderLines: [], // { orderId, item, qty }, the committed business data
};

const hhmm = (t) => `${Math.floor(t / 60)}:${String(t % 60).padStart(2, "0")}`;

function start(sid, user, now) {
  db.sessions.set(sid, { user, lastUsed: now });
}

// Return the user of the session, or null if it is unknown or idle.
function touch(sid, now) {
  const s = db.sessions.get(sid);
  if (s === undefined || now - s.lastUsed > TIMEOUT) return null;
  s.lastUsed = now;
  return s.user;
}

function cart(sid) {
  return db.cartLines
    .filter((l) => l.sessionId === sid)
    .map((l) => `${l.item} ${l.qty}`)
    .join(", ");
}

function add(server, sid, item, now) {
  const user = touch(sid, now);
  const line = db.cartLines.find((l) => l.sessionId === sid && l.item === item);
  if (line) line.qty += 1;
  else db.cartLines.push({ sessionId: sid, item, qty: 1 });
  console.log(`${hhmm(now)} ${user} adds ${item} on server ${server}: cart ${cart(sid)}`);
}

// The pending cart lines become an order. A database does this in one transaction.
function checkout(server, sid, now) {
  const user = touch(sid, now);
  const lines = cart(sid);
  const oid = db.orders.length + 1;
  db.orders.push({ id: oid, user });
  for (const l of db.cartLines.filter((l) => l.sessionId === sid)) {
    db.orderLines.push({ orderId: oid, item: l.item, qty: l.qty });
  }
  db.cartLines = db.cartLines.filter((l) => l.sessionId !== sid);
  console.log(`${hhmm(now)} ${user} checks out on server ${server}: order ${oid} with ${lines}`);
}

function report() {
  const sums = new Map();
  for (const l of db.orderLines) sums.set(l.item, (sums.get(l.item) ?? 0) + l.qty);
  const items = [...sums.keys()].sort();
  console.log("sales:", items.map((i) => `${i} ${sums.get(i)}`).join(", "));
}

function purge(now) {
  const ids = [...db.sessions.keys()]
    .filter((id) => now - db.sessions.get(id).lastUsed > TIMEOUT)
    .sort();
  db.cartLines = db.cartLines.filter((l) => !ids.includes(l.sessionId));
  for (const id of ids) db.sessions.delete(id);
  console.log(`${hhmm(now)} purge sessions idle for more than ${TIMEOUT} minutes: ${ids.join(", ")}`);
}

start("s1", "ann", 600);
start("s2", "bob", 601);
add("a", "s1", "pen", 600);
add("a", "s2", "ink", 601);
add("b", "s1", "pen", 603); // server b reads the rows that server a wrote
add("a", "s1", "ink", 604);
checkout("b", "s1", 625);
report();
purge(640);
console.log(
  `rows left: sessions ${db.sessions.size}, cart lines ${db.cartLines.length}, ` +
    `orders ${db.orders.length}, order lines ${db.orderLines.length}`,
);
```

```output
10:00 ann adds pen on server a: cart pen 1
10:01 bob adds ink on server a: cart ink 1
10:03 ann adds pen on server b: cart pen 2
10:04 ann adds ink on server a: cart pen 2, ink 1
10:25 ann checks out on server b: order 1 with pen 2, ink 1
sales: ink 1, pen 2
10:40 purge sessions idle for more than 30 minutes: s2
rows left: sessions 1, cart lines 0, orders 1, order lines 2
```

- Verdict: library. Node.js has no database in its stable standard library, so a real server uses a database driver. The example keeps the tables in a shared object, so it runs with no package. All the servers use the same object, as they would use the same database.
- `cartLines` is the pending data and `orderLines` is the committed business data. The checkout moves the lines of one session from the first to the second.
