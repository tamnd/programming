```javascript wrong id=admin-route
const http = require("node:http");
const { randomBytes } = require("node:crypto");

const TOKEN = randomBytes(16).toString("hex");
const settings = { signups: "open" };

const server = http.createServer((req, res) => {
  if (req.url === "/settings") {
    if (req.headers.authorization !== `Bearer ${TOKEN}`) {
      res.writeHead(401).end("login required");
      return;
    }
    res.end(JSON.stringify(settings));
  } else if (req.url === "/admin/close-signups" && req.method === "POST") {
    // The admin page that shows this button needs a login.
    // The endpoint behind the button does not check.
    settings.signups = "closed";
    res.end("signups closed");
  } else {
    res.writeHead(404).end("not found");
  }
});

function call(method, path) {
  const { port } = server.address();
  return new Promise((resolve, reject) => {
    const req = http.request({ host: "127.0.0.1", port, method, path }, (res) => {
      let body = "";
      res.on("data", (chunk) => (body += chunk));
      res.on("end", () => resolve(`${method} ${path} ${res.statusCode} ${body}`));
    });
    req.on("error", reject);
    req.end();
  });
}

server.listen(0, "127.0.0.1", async () => {
  console.log(await call("GET", "/settings"));
  console.log(await call("POST", "/admin/close-signups"));
  console.log(settings);
  server.close();
});
```

```output for=admin-route
GET /settings 401 login required
POST /admin/close-signups 200 signups closed
{ signups: 'closed' }
```

The admin page that shows the button needs a login, so the developer thought the action was protected. The server answers the request to the endpoint itself, and that endpoint makes no check.

```javascript run id=route-table
const http = require("node:http");
const { randomBytes, timingSafeEqual } = require("node:crypto");

const TOKEN = randomBytes(16).toString("hex");
const settings = { signups: "open" };

// Each route says if it is public. The default is that it is not.
const routes = new Map([
  ["GET /health", { handler: (req, res) => res.end("ok"), public: true }],
  ["GET /settings", { handler: (req, res) => res.end(JSON.stringify(settings)) }],
  [
    "POST /admin/close-signups",
    {
      handler: (req, res) => {
        settings.signups = "closed";
        res.end("signups closed");
      },
    },
  ],
]);

function authenticated(req) {
  const sent = Buffer.from(req.headers.authorization ?? "");
  const expected = Buffer.from(`Bearer ${TOKEN}`);
  return sent.length === expected.length && timingSafeEqual(sent, expected);
}

// The only request handler. No route runs before this check.
const server = http.createServer((req, res) => {
  const route = routes.get(`${req.method} ${req.url}`);
  if (route === undefined) {
    res.writeHead(404).end("not found");
  } else if (!route.public && !authenticated(req)) {
    res.writeHead(401).end("login required");
  } else {
    route.handler(req, res);
  }
});

function call(method, path, auth) {
  const { port } = server.address();
  const headers = auth ? { authorization: auth } : {};
  return new Promise((resolve, reject) => {
    const req = http.request({ host: "127.0.0.1", port, method, path, headers }, (res) => {
      let body = "";
      res.on("data", (chunk) => (body += chunk));
      res.on("end", () => resolve(`${method} ${path} ${res.statusCode} ${body}`));
    });
    req.on("error", reject);
    req.end();
  });
}

server.listen(0, "127.0.0.1", async () => {
  console.log(await call("GET", "/health"));
  console.log(await call("POST", "/admin/close-signups"));
  console.log(settings);
  console.log(await call("POST", "/admin/close-signups", `Bearer ${TOKEN}`));
  console.log(settings);
  server.close();
});
```

```output for=route-table
GET /health 200 ok
POST /admin/close-signups 401 login required
{ signups: 'open' }
POST /admin/close-signups 200 signups closed
{ signups: 'closed' }
```

The server has one request handler. It finds the route in a table and checks the token before any route runs, unless the route says `public: true`.

- Verdict: stdlib. `node:http` runs the server and the client, and `crypto.timingSafeEqual` compares the token. It throws when the lengths differ, so the code checks the length first.
- Express and similar frameworks put the same check in a middleware that runs before the routes. Register it before the routes, not on each route, so that a new route gets it without any change.
- The demonstration uses `http.request` to 127.0.0.1, so no proxy or network is involved.
