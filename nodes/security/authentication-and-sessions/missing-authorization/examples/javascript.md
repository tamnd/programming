```javascript run id=central-policy
const sessions = new Map([["session-alice", "alice"], ["session-carol", "carol"]]);
const roles = new Map([["alice", "customer"], ["bob", "customer"], ["carol", "admin"]]);
// The policy lists what each role may do. Anything not listed is denied.
const policy = new Map([
  ["customer", new Set(["invoice:read"])],
  ["admin", new Set(["invoice:read", "user:delete"])],
]);
const invoices = [
  { id: 1, owner: "alice", total: 120 },
  { id: 2, owner: "bob", total: 75 },
];

const routes = new Map();

function route(name, { permission, handler }) {
  if (typeof permission !== "string") {
    throw new Error(`route ${name} has no permission`);
  }
  routes.set(name, { permission, handler });
}

// Every request passes through this one check.
function dispatch(name, session, ...args) {
  const entry = routes.get(name);
  if (!entry) return [404, "not found"];
  const user = sessions.get(session);
  if (user === undefined) return [401, "log in first"];
  if (!(policy.get(roles.get(user))?.has(entry.permission) ?? false)) {
    return [403, "forbidden"];
  }
  return entry.handler(user, ...args);
}

route("getInvoice", {
  permission: "invoice:read",
  handler(user, id) {
    // The lookup includes the owner, as WHERE id = ? AND owner = ? does.
    const invoice = invoices.find((i) => i.id === id && i.owner === user);
    return invoice ? [200, invoice] : [404, "not found"];
  },
});

route("deleteUser", {
  permission: "user:delete",
  handler: (user, name) => [200, `deleted ${name}`],
});

console.log(dispatch("getInvoice", "session-alice", 1));
console.log(dispatch("getInvoice", "session-alice", 2));
console.log(dispatch("deleteUser", "session-alice", "bob"));
console.log(dispatch("deleteUser", "session-carol", "bob"));
console.log(dispatch("getInvoice", "none", 1));

try {
  route("exportAll", { handler: () => [200, invoices] });
} catch (error) {
  console.log("refused:", error.message);
}
```

```output for=central-policy
[ 200, { id: 1, owner: 'alice', total: 120 } ]
[ 404, 'not found' ]
[ 403, 'forbidden' ]
[ 200, 'deleted bob' ]
[ 401, 'log in first' ]
refused: route exportAll has no permission
```

`route` throws when a route has no permission, and `dispatch` checks the session and the policy before any handler runs. The invoice lookup matches the id and the owner together.

- Verdict: stdlib. Node.js has no authorization layer. Web frameworks add the same check as middleware.
- The expression `policy.get(roles.get(user))?.has(entry.permission) ?? false` denies by default. A user with no role, or a role with no entry, gives `false`.
- A check in browser code only changes what the page shows. The server must repeat every check.
