```python wrong id=traceback-to-client
import logging
import os
import sys
import traceback

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format="LOG %(message)s")
log = logging.getLogger("shop")

# A placeholder. A real service reads this from a secret store.
DATABASE_URL = "postgresql://shop:PLACEHOLDER-PASSWORD@db.internal:5432/shop"


def connect(url):
    raise ConnectionError(f"could not connect to {url}")


def get_order(order_id):
    connect(DATABASE_URL)


def handle(headers, order_id):
    log.info("GET /orders/%s headers=%s", order_id, headers)
    try:
        return 200, get_order(order_id)
    except Exception:
        return 500, traceback.format_exc()  # the client gets the traceback


status, body = handle({"Authorization": "Bearer PLACEHOLDER-TOKEN"}, 7)
print(status)
# The traceback shows the full path of this file. The demo shortens it, so
# that the output is the same on every machine.
print(body.replace(os.path.dirname(os.path.abspath(__file__)), "<app>"))
```

```output for=traceback-to-client
LOG GET /orders/7 headers={'Authorization': 'Bearer PLACEHOLDER-TOKEN'}
500
Traceback (most recent call last):
  File "<app>/main.py", line 24, in handle
    return 200, get_order(order_id)
                ~~~~~~~~~^^^^^^^^^^
  File "<app>/main.py", line 18, in get_order
    connect(DATABASE_URL)
    ~~~~~~~^^^^^^^^^^^^^^
  File "<app>/main.py", line 14, in connect
    raise ConnectionError(f"could not connect to {url}")
ConnectionError: could not connect to postgresql://shop:PLACEHOLDER-PASSWORD@db.internal:5432/shop
```

The log line holds the bearer token, and the client gets the traceback with the connection string.

```python run id=generic-error
import io
import itertools
import json
import logging
import os


class Secret:
    """Holds a secret. Its repr and str never show the value."""

    def __init__(self, value):
        self._value = value

    def reveal(self):
        return self._value

    def __repr__(self):
        return "Secret('***')"

    __str__ = __repr__


# The server log. In the demo it is a buffer, so the program can print it.
server_log = io.StringIO()
handler = logging.StreamHandler(server_log)
handler.setFormatter(logging.Formatter("LOG %(levelname)s %(message)s"))
log = logging.getLogger("shop")
log.addHandler(handler)
log.setLevel(logging.INFO)
log.propagate = False

# Placeholders. A real service reads these from a secret store.
DB_HOST = "db.internal:5432"
DB_PASSWORD = Secret("PLACEHOLDER-PASSWORD")
INCIDENTS = itertools.count(1001)  # a real service uses a random id


def connect(host, password):
    raise ConnectionError(f"could not connect to {host} with password {password}")


def get_order(order_id):
    connect(DB_HOST, DB_PASSWORD)


def handle(headers, order_id):
    # Log an allow list of fields, never the whole set of headers.
    log.info("GET /orders/%s auth=%s", order_id, "present" if "Authorization" in headers else "absent")
    try:
        return 200, get_order(order_id)
    except Exception:
        incident = f"E{next(INCIDENTS)}"
        log.exception("incident %s", incident)  # the details stay on the server
        return 500, json.dumps({"error": "internal error", "incident": incident})


status, body = handle({"Authorization": "Bearer PLACEHOLDER-TOKEN"}, 7)
print("client gets:", status, body)
print("server log:")
print(server_log.getvalue().replace(os.path.dirname(os.path.abspath(__file__)), "<app>"), end="")
```

```output for=generic-error
client gets: 500 {"error": "internal error", "incident": "E1001"}
server log:
LOG INFO GET /orders/7 auth=present
LOG ERROR incident E1001
Traceback (most recent call last):
  File "<app>/main.py", line 50, in handle
    return 200, get_order(order_id)
                ~~~~~~~~~^^^^^^^^^^
  File "<app>/main.py", line 43, in get_order
    connect(DB_HOST, DB_PASSWORD)
    ~~~~~~~^^^^^^^^^^^^^^^^^^^^^^
  File "<app>/main.py", line 39, in connect
    raise ConnectionError(f"could not connect to {host} with password {password}")
ConnectionError: could not connect to db.internal:5432 with password Secret('***')
```

The client gets a generic message and an incident id. The server log has the traceback, and the password shows as `Secret('***')`.

```python run id=steps
import hmac

EXPECTED = "7f3a9c"  # a placeholder token


def early_exit_equal(a, b):
    """Compares like a plain loop: it stops at the first difference."""
    steps = 0
    if len(a) != len(b):
        return False, steps
    for x, y in zip(a, b):
        steps += 1
        if x != y:
            return False, steps
    return True, steps


for guess in ["000000", "700000", "7f0000", "7f3000", "7f3a9c"]:
    equal, steps = early_exit_equal(guess, EXPECTED)
    print(guess, equal, "steps:", steps)

print(hmac.compare_digest(b"7f0000", EXPECTED.encode()))
print(hmac.compare_digest(b"7f3a9c", EXPECTED.encode()))
```

```output for=steps
000000 False steps: 1
700000 False steps: 2
7f0000 False steps: 3
7f3000 False steps: 4
7f3a9c True steps: 6
False
True
```

The early-exit comparison takes one more step for each correct character at the start. `hmac.compare_digest` gives the same answer without that difference.

- Verdict: stdlib. `logging`, `traceback`, `json` and `hmac` cover the pattern. `secrets` makes tokens and incident ids in real code.
- `logging.Logger.exception` writes the traceback to the log at the level `ERROR`. Send it to the server log, never to the response.
- A wrapper class with its own `__repr__` and `__str__` stops the value from showing in f-strings, logs and tracebacks. It does not stop code that calls `reveal()` and prints the result.
- `hmac.compare_digest` needs two values of the same type: two `bytes` or two ASCII `str`. Different lengths can still be observed, but not the contents.
- Web frameworks have a debug mode that shows tracebacks in the browser. Turn it off in production.
