```javascript run id=cookie-cart
const crypto = require("node:crypto");

const SECRET = "server secret, never sent to the client";

function sign(data) {
  return crypto.createHmac("sha256", SECRET).update(data).digest("hex");
}

// Serializes the session state into the value of a cookie: the data, a bar and the signature.
function encode(cart) {
  const data = [...cart].map(([item, qty]) => `${item}:${qty}`).join(",");
  return data + "|" + sign(data);
}

// Checks the signature and gives back the state. A changed cookie gives an error.
function decode(cookie) {
  const bar = cookie.lastIndexOf("|");
  const data = cookie.slice(0, bar);
  const tag = Buffer.from(cookie.slice(bar + 1));
  const expected = Buffer.from(sign(data));
  if (tag.length !== expected.length || !crypto.timingSafeEqual(tag, expected)) {
    throw new Error("bad signature");
  }
  const cart = new Map();
  for (const part of data.split(",").filter(Boolean)) {
    const [item, qty] = part.split(":");
    cart.set(item, Number(qty));
  }
  return cart;
}

// One request. The server keeps nothing between requests: the state comes in and goes out in the cookie.
function handleAdd(cookie, item) {
  const cart = cookie ? decode(cookie) : new Map();
  cart.set(item, (cart.get(item) ?? 0) + 1);
  return encode(cart);
}

function show(cookie) {
  const bar = cookie.lastIndexOf("|");
  return `${cookie.slice(0, bar)}|${cookie.slice(bar + 1, bar + 9)}... (${cookie.length} bytes)`;
}

let cookie = null;
for (const item of ["pen", "pen", "ink"]) {
  cookie = handleAdd(cookie, item);
  console.log(`add ${item}: cookie ${show(cookie)}`);
}

const tampered = cookie.replace("ink:1", "ink:9");
try {
  handleAdd(tampered, "pen");
} catch (e) {
  console.log("a client changes ink:1 to ink:9:", e.message);
}

console.log("cart:", [...decode(cookie)].map(([item, qty]) => `${item} ${qty}`).join(", "));
```

```output
add pen: cookie pen:1|2c06beca... (70 bytes)
add pen: cookie pen:2|bdba40d9... (70 bytes)
add ink: cookie pen:2,ink:1|8bdcc52b... (76 bytes)
a client changes ink:1 to ink:9: bad signature
cart: pen 2, ink 1
```

- Verdict: stdlib. `crypto.createHmac("sha256", SECRET)` from `node:crypto` signs the data, and `crypto.timingSafeEqual` compares the signatures. `timingSafeEqual` needs two buffers of the same length, so the code checks the lengths first.
- The cart is a `Map`, so the items keep the order in which they were added.
