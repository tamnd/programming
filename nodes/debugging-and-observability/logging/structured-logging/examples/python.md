```python run id=records
import json
import logging
import sys


class JsonFormatter(logging.Formatter):
    """Writes each record as one JSON object, with no timestamp."""

    def format(self, record):
        entry = {"level": record.levelname, "logger": record.name, "msg": record.getMessage()}
        entry.update(getattr(record, "fields", {}))
        if record.exc_info:
            entry["error"] = repr(record.exc_info[1])
        return json.dumps(entry)


handler = logging.StreamHandler(sys.stdout)
handler.setFormatter(JsonFormatter())
log = logging.getLogger("shop")
log.addHandler(handler)
log.setLevel(logging.INFO)

log.debug("cart loaded", extra={"fields": {"items": 3}})  # below INFO: dropped
log.info("order placed", extra={"fields": {"order": 1042, "total_cents": 1999}})
log.warning("stock low", extra={"fields": {"sku": "A-17", "left": 2}})
user = 'bob\n{"level": "INFO", "msg": "login ok"}'  # text from a request
log.info("login failed", extra={"fields": {"user": user}})
try:
    1 / 0
except ZeroDivisionError:
    log.exception("price failed", extra={"fields": {"order": 1043}})
print(logging.DEBUG, logging.INFO, logging.WARNING, logging.ERROR, logging.CRITICAL)
```

```output for=records
{"level": "INFO", "logger": "shop", "msg": "order placed", "order": 1042, "total_cents": 1999}
{"level": "WARNING", "logger": "shop", "msg": "stock low", "sku": "A-17", "left": 2}
{"level": "INFO", "logger": "shop", "msg": "login failed", "user": "bob\n{\"level\": \"INFO\", \"msg\": \"login ok\"}"}
{"level": "ERROR", "logger": "shop", "msg": "price failed", "order": 1043, "error": "ZeroDivisionError('division by zero')"}
10 20 30 40 50
```

The default level of the root logger is WARNING. A program that logs at INFO and does not set a level sees nothing.

```python wrong id=default-level
import logging
import sys

logging.basicConfig(stream=sys.stdout, format="%(levelname)s %(message)s")
logging.info("server started")  # below the default level WARNING
logging.warning("disk %d%% full", 91)
```

```output for=default-level
WARNING disk 91% full
```

A plain text format writes the user's text as it is. A newline in the text starts a second line that looks like a real record. The JSON formatter above writes the same text as `\n` inside one string.

```python wrong id=forged
import logging
import sys

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format="%(levelname)s %(message)s")
user = "bob\nINFO login ok user=admin"  # text from a request
logging.info("login failed user=%s", user)
```

```output for=forged
INFO login failed user=bob
INFO login ok user=admin
```

The logging module formats the message only when a handler will write the record. The arguments are still computed before the call. An f-string builds the whole message even when the record is dropped.

```python run id=deferred
import logging
import sys

calls = 0


class Cart:
    def __str__(self):
        global calls
        calls += 1
        return "Cart(3 items)"


logging.basicConfig(stream=sys.stdout, level=logging.INFO, format="%(levelname)s %(message)s")
log = logging.getLogger("shop")
cart = Cart()

log.debug("cart %s", cart)  # dropped before the message is formatted
print("after %-style call:", calls)
log.debug(f"cart {cart}")  # the f-string is built first, then dropped
print("after f-string call:", calls)
if log.isEnabledFor(logging.DEBUG):  # skips the work when DEBUG is off
    log.debug("cart %s", cart)
log.info("cart %s", cart)
print("after info call:", calls)
```

```output for=deferred
after %-style call: 0
after f-string call: 1
INFO cart Cart(3 items)
after info call: 2
```

- Verdict: stdlib. The `logging` module has levels, named loggers in a hierarchy, handlers and formatters.
- The module has no JSON formatter. A subclass of `logging.Formatter` writes each record as JSON, and `extra=` adds fields to the record.
- `log.exception` logs at ERROR and adds the active exception to the record.
- Pass the arguments after a `%s` format string instead of an f-string, so that a dropped record is not formatted.
