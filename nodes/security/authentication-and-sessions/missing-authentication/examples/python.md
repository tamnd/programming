```python wrong id=debug-port
import secrets
import threading
import urllib.error
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

TOKEN = secrets.token_hex(16)  # made at start; the operators get it
SETTINGS = {"maintenance": False}


class Handler(BaseHTTPRequestHandler):
    def reply(self, status, text):
        body = text.encode()
        self.send_response(status)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass


class AppHandler(Handler):
    def do_GET(self):
        if self.path == "/orders":
            if self.headers.get("Authorization") != "Bearer " + TOKEN:
                return self.reply(401, "login required")
            return self.reply(200, "3 orders")
        self.reply(404, "not found")


class DebugHandler(Handler):
    # A second port for the operators. Nobody else knows it, so it has no login.
    def do_POST(self):
        if self.path == "/maintenance":
            SETTINGS["maintenance"] = True
            return self.reply(200, "maintenance mode on")
        self.reply(404, "not found")


def start(handler):
    server = ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server


def call(method, server, path):
    url = f"http://127.0.0.1:{server.server_port}{path}"
    opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))
    try:
        with opener.open(urllib.request.Request(url, method=method)) as response:
            return response.status, response.read().decode()
    except urllib.error.HTTPError as error:
        return error.code, error.read().decode()


app = start(AppHandler)
debug = start(DebugHandler)
print("app   GET /orders      ", call("GET", app, "/orders"))
print("debug POST /maintenance", call("POST", debug, "/maintenance"))
print(SETTINGS)
```

```output for=debug-port
app   GET /orders       (401, 'login required')
debug POST /maintenance (200, 'maintenance mode on')
{'maintenance': True}
```

The second port has no check, so a caller with no token turns on maintenance mode.

```python run id=gated
import hmac
import secrets
import threading
import urllib.error
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

TOKEN = secrets.token_hex(16)  # made at start; the operators get it
SETTINGS = {"maintenance": False}


class GatedHandler(BaseHTTPRequestHandler):
    # Each subclass lists its routes: (method, path) -> (method name, public).
    ROUTES = {}

    def dispatch(self, method):
        route = self.ROUTES.get((method, self.path))
        if route is None:
            return self.reply(404, "not found")
        name, public = route
        if not public and not self.authenticated():
            return self.reply(401, "login required")
        getattr(self, name)()

    def authenticated(self):
        sent = self.headers.get("Authorization", "")
        return hmac.compare_digest(sent.encode(), ("Bearer " + TOKEN).encode())

    def do_GET(self):
        self.dispatch("GET")

    def do_POST(self):
        self.dispatch("POST")

    def reply(self, status, text):
        body = text.encode()
        self.send_response(status)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass


class AppHandler(GatedHandler):
    ROUTES = {("GET", "/health"): ("health", True), ("GET", "/orders"): ("orders", False)}

    def health(self):
        self.reply(200, "ok")

    def orders(self):
        self.reply(200, "3 orders")


class DebugHandler(GatedHandler):
    ROUTES = {("POST", "/maintenance"): ("maintenance", False)}

    def maintenance(self):
        SETTINGS["maintenance"] = True
        self.reply(200, "maintenance mode on")


def start(handler):
    server = ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server


def call(method, server, path, token=None):
    url = f"http://127.0.0.1:{server.server_port}{path}"
    headers = {"Authorization": "Bearer " + token} if token else {}
    opener = urllib.request.build_opener(urllib.request.ProxyHandler({}))
    try:
        with opener.open(urllib.request.Request(url, method=method, headers=headers)) as response:
            return response.status, response.read().decode()
    except urllib.error.HTTPError as error:
        return error.code, error.read().decode()


app = start(AppHandler)
debug = start(DebugHandler)
print("app   GET /health      ", call("GET", app, "/health"))
print("app   GET /orders      ", call("GET", app, "/orders"))
print("debug POST /maintenance", call("POST", debug, "/maintenance"))
print(SETTINGS)
print("debug POST /maintenance", call("POST", debug, "/maintenance", TOKEN))
print(SETTINGS)
```

```output for=gated
app   GET /health       (200, 'ok')
app   GET /orders       (401, 'login required')
debug POST /maintenance (401, 'login required')
{'maintenance': False}
debug POST /maintenance (200, 'maintenance mode on')
{'maintenance': True}
```

Every handler class inherits the check from `GatedHandler`. A route is public only when its entry says `True`.

- Verdict: stdlib. `http.server` and `urllib.request` run both servers and the client. `hmac.compare_digest` compares the token in constant time, and `secrets.token_hex` makes it.
- The Python documentation says that `http.server` is not recommended for production. A real service uses the authentication of its framework, with the same rule: every route is protected unless the code says otherwise.
- `urllib.request.ProxyHandler({})` turns off proxies from the environment, so the requests go straight to 127.0.0.1.
- The token is made at start for the demonstration. A real service gets it from a secret store, never from the source code. See [[hard-coded-credentials]].
