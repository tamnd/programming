```javascript run id=sessions-on-two-servers
// One server's sessions, kept in its memory as serialized text, with the time of their last use.
class SessionStore {
  static TIMEOUT = 30; // minutes with no request, then the session ends

  constructor(name) {
    this.name = name;
    this.sessions = new Map(); // session ID -> [serialized state, minute of last use]
    this.count = 0;
  }

  newId() {
    // A real server makes a random ID with a secure generator, such as crypto.randomBytes(32).
    this.count++;
    return `${this.name}${this.count}`;
  }

  load(sid, now) {
    const entry = this.sessions.get(sid);
    if (entry === undefined || now - entry[1] > SessionStore.TIMEOUT) return null;
    return deserialize(entry[0]);
  }

  save(sid, cart, now) {
    this.sessions.set(sid, [serialize(cart), now]);
  }

  sweep(now) {
    const idle = [...this.sessions].filter(([, [, last]]) => now - last > SessionStore.TIMEOUT).map(([sid]) => sid).sort();
    for (const sid of idle) this.sessions.delete(sid);
    return idle;
  }
}

function serialize(cart) {
  return [...cart].map(([item, qty]) => `${item}:${qty}`).join(",");
}

function deserialize(text) {
  const cart = new Map();
  for (const part of text.split(",").filter(Boolean)) {
    const [item, qty] = part.split(":");
    cart.set(item, Number(qty));
  }
  return cart;
}

const show = (cart) => [...cart].map(([item, qty]) => `${item} ${qty}`).join(", ");
const clock = (minute) => `10:${String(minute).padStart(2, "0")}`;

// One request. The client sends only its session ID; the state is on the server.
function add(server, sid, who, item, now) {
  let cart = sid ? server.load(sid, now) : null;
  let status;
  if (cart === null) {
    status = sid ? `session ${sid} is unknown here, ` : "";
    sid = server.newId();
    cart = new Map();
    status += `new session ${sid}`;
  } else {
    status = `session ${sid}`;
  }
  cart.set(item, (cart.get(item) ?? 0) + 1);
  server.save(sid, cart, now);
  console.log(`${clock(now)} ${who} adds ${item} on server ${server.name}: ${status}, cart ${show(cart)}`);
  return sid;
}

const a = new SessionStore("a");
const b = new SessionStore("b");
const ann = add(a, null, "ann", "pen", 0);
add(a, null, "bob", "ink", 1);
add(a, ann, "ann", "pen", 2);
add(b, ann, "ann", "ink", 3); // a load balancer sends this request to the other server
add(a, ann, "ann", "ink", 30);
console.log("10:40 server a removes idle sessions:", a.sweep(40).join(", "));
for (const [sid, [text, last]] of [...a.sessions].sort()) {
  console.log(`server a keeps ${sid} = '${text}', last used at ${clock(last)}`);
}
```

```output
10:00 ann adds pen on server a: new session a1, cart pen 1
10:01 bob adds ink on server a: new session a2, cart ink 1
10:02 ann adds pen on server a: session a1, cart pen 2
10:03 ann adds ink on server b: session a1 is unknown here, new session b1, cart ink 1
10:30 ann adds ink on server a: session a1, cart pen 2, ink 1
10:40 server a removes idle sessions: a2
server a keeps a1 = 'pen:2,ink:1', last used at 10:30
```

- Verdict: idiom. The store is a `Map` in the memory of one Node.js process. Node.js serves all the requests of the process on one thread, so two requests do not change the map at the same time.
- Each server has its own store, so a request that goes to server `b` does not find the session of server `a`.
