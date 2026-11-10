```javascript wrong id=client-flag
// The server side. Maps stand for the session store and the database.
const sessions = new Map([["s-ana", "ana"], ["s-chi", "chi"]]);
const users = new Map([["ana", { role: "admin" }], ["chi", { role: "member" }]]);
const apps = new Map([["chat", { enabled: true }]]);

// POST /apps/:name/disable
function disableApp(cookie, body) {
  const user = sessions.get(cookie);
  if (user === undefined) return "401 login required";
  // The browser sends isAdmin, so the server trusts it.
  if (body.isAdmin !== true) return "403 admins only";
  apps.get(body.app).enabled = false;
  return `200 ${body.app} disabled by ${user}`;
}

// The browser side. It shows the button only to admins and adds the flag.
function pageFor(user) {
  const isAdmin = users.get(user).role === "admin";
  return {
    showDisableButton: isAdmin,
    click: (cookie, app) => disableApp(cookie, { app, isAdmin }),
  };
}

console.log("chi sees the button:", pageFor("chi").showDisableButton);
console.log("chi clicks:", pageFor("chi").click("s-chi", "chat"));
// The classic demonstration: chi sends the request without the page.
console.log("chi direct:", disableApp("s-chi", { app: "chat", isAdmin: true }));
console.log(apps.get("chat"));
```

```output for=client-flag
chi sees the button: false
chi clicks: 403 admins only
chi direct: 200 chat disabled by chi
{ enabled: false }
```

The page hides the button from `chi`, and the click sends `isAdmin: false`. The server trusts the flag in the body. When `chi` sends the request without the page, with `isAdmin: true`, the server disables the app.

```javascript run id=server-decides
// The server side. Maps stand for the session store and the database.
const sessions = new Map([["s-ana", "ana"], ["s-chi", "chi"]]);
const users = new Map([["ana", { role: "admin" }], ["chi", { role: "member" }]]);
const apps = new Map([["chat", { enabled: true }]]);

// The server decides from its own data. It ignores any role in the request.
function isAdmin(user) {
  return users.get(user)?.role === "admin";
}

// POST /apps/:name/disable
function disableApp(cookie, body) {
  const user = sessions.get(cookie);
  if (user === undefined) return "401 login required";
  if (!isAdmin(user)) return "403 admins only";
  const app = apps.get(body.app);
  if (app === undefined) return "404 no such app";
  app.enabled = false;
  return `200 ${body.app} disabled by ${user}`;
}

// The browser side can still hide the button. That is only for the user.
function pageFor(user) {
  return {
    showDisableButton: isAdmin(user),
    click: (cookie, app) => disableApp(cookie, { app }),
  };
}

console.log("chi sees the button:", pageFor("chi").showDisableButton);
console.log("chi direct:", disableApp("s-chi", { app: "chat", isAdmin: true }));
console.log(apps.get("chat"));
console.log("ana clicks:", pageFor("ana").click("s-ana", "chat"));
console.log(apps.get("chat"));
```

```output for=server-decides
chi sees the button: false
chi direct: 403 admins only
{ enabled: true }
ana clicks: 200 chat disabled by ana
{ enabled: false }
```

The server finds the user from the session and the role from its own data. The `isAdmin` field in the request has no effect. The page still hides the button, but that only helps the user.

- Verdict: idiom. Node.js has no authorization layer in its standard library. The rule is the same in every framework: the server decides from data that it controls.
- A JSON Web Token can carry a role, but only when the server verifies the signature and the algorithm before it reads the claims. An unverified token is a value that the client controls.
- Code in the browser is the client, even when it comes from the same repository as the server.
