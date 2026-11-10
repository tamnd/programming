```javascript wrong id=whole-record
// A map stands for the database. Each function stands for a request handler.
const users = new Map([
  ["ana", { name: "ana", email: "ana@example.com", passwordHash: "PLACEHOLDER-HASH", resetToken: "PLACEHOLDER-TOKEN" }],
]);

// GET /api/users/:name
function getUser(name) {
  const user = users.get(name);
  if (user === undefined) return [404, "{}"];
  return [200, JSON.stringify(user)]; // every field goes to the client
}

// GET /api/orders/:id
function getOrder(id) {
  try {
    throw new Error("connect ECONNREFUSED db.internal:5432 (user shop, database orders)");
  } catch (err) {
    return [500, JSON.stringify({ error: err.message })]; // the internal error goes too
  }
}

console.log(...getUser("ana"));
console.log(...getOrder(7));
```

```output for=whole-record
200 {"name":"ana","email":"ana@example.com","passwordHash":"PLACEHOLDER-HASH","resetToken":"PLACEHOLDER-TOKEN"}
500 {"error":"connect ECONNREFUSED db.internal:5432 (user shop, database orders)"}
```

`JSON.stringify(user)` sends every field of the record, including the password hash and the reset token. The error handler sends the internal error message, with the host name, the user and the database.

```javascript run id=allow-list
const { randomUUID } = require("node:crypto");

// A map stands for the database. Each function stands for a request handler.
const users = new Map([
  ["ana", { name: "ana", email: "ana@example.com", passwordHash: "PLACEHOLDER-HASH", resetToken: "PLACEHOLDER-TOKEN" }],
]);
const serverLog = [];

// The response lists the fields that the client may see. New fields stay out.
function publicUser(user) {
  return { name: user.name, email: user.email };
}

// GET /api/users/:name
function getUser(name) {
  const user = users.get(name);
  if (user === undefined) return [404, "{}"];
  return [200, JSON.stringify(publicUser(user))];
}

// GET /api/orders/:id
function getOrder(id) {
  try {
    throw new Error("connect ECONNREFUSED db.internal:5432 (user shop, database orders)");
  } catch (err) {
    const incident = randomUUID();
    serverLog.push({ incident, message: err.message }); // the details stay on the server
    return [500, JSON.stringify({ error: "internal error", incident })];
  }
}

console.log(...getUser("ana"));
const [status, body] = getOrder(7);
console.log(status, Object.keys(JSON.parse(body)));
console.log("server log:", serverLog[0].message);
```

```output for=allow-list
200 {"name":"ana","email":"ana@example.com"}
500 [ 'error', 'incident' ]
server log: connect ECONNREFUSED db.internal:5432 (user shop, database orders)
```

`publicUser` copies only the fields that the client may see. A field that someone adds to the record later stays on the server. The error handler logs the message and sends a generic error with an incident id.

- Verdict: stdlib. `JSON.stringify` and `crypto.randomUUID` are enough.
- `JSON.stringify` also calls a `toJSON` method if the object has one. A class can use it to control its own output, but an explicit copy at the response is easier to review.
- Do not send `err.stack` or `err.message` of an unexpected error to the client. Add an error handler that sends a fixed message for every unexpected error.
- The demo prints the keys of the error response, because the incident id is random.
