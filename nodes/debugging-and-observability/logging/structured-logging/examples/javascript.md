```javascript run id=records
const LEVELS = { debug: 10, info: 20, warn: 30, error: 40 };

// Returns a logger that writes one JSON object per line, with no timestamp.
function createLogger(minLevel, fields = {}) {
  const log = (level, msg, extra = {}) => {
    if (LEVELS[level] < LEVELS[minLevel]) {
      return;
    }
    console.log(JSON.stringify({ level, msg, ...fields, ...extra }));
  };
  return {
    debug: (msg, extra) => log("debug", msg, extra),
    info: (msg, extra) => log("info", msg, extra),
    warn: (msg, extra) => log("warn", msg, extra),
    error: (msg, extra) => log("error", msg, extra),
    child: (more) => createLogger(minLevel, { ...fields, ...more }),
  };
}

const log = createLogger("info");
log.debug("cart loaded", { items: 3 }); // below info: dropped
log.info("order placed", { order: 1042, totalCents: 1999 });
const req = log.child({ request: "r-7" });
req.warn("stock low", { sku: "A-17", left: 2 });
req.info("login failed", { user: "bob\n{\"level\":\"info\",\"msg\":\"login ok\"}" });
function price(cents) {
  if (cents <= 0) {
    throw new RangeError("price must be positive");
  }
  return cents;
}

try {
  price(-5);
} catch (err) {
  req.error("bad cart", { error: err.name, detail: err.message });
}
```

```output for=records
{"level":"info","msg":"order placed","order":1042,"totalCents":1999}
{"level":"warn","msg":"stock low","request":"r-7","sku":"A-17","left":2}
{"level":"info","msg":"login failed","request":"r-7","user":"bob\n{\"level\":\"info\",\"msg\":\"login ok\"}"}
{"level":"error","msg":"bad cart","request":"r-7","error":"RangeError","detail":"price must be positive"}
```

`JSON.stringify` writes only the own enumerable properties of an object. The `name`, `message` and `stack` of an `Error` are not enumerable, so an error becomes `{}`.

```javascript wrong id=error-object
function price(cents) {
  if (cents <= 0) {
    throw new RangeError("price must be positive");
  }
  return cents;
}

try {
  price(-5);
} catch (err) {
  console.log(JSON.stringify({ level: "error", msg: "bad cart", error: err }));
}
```

```output for=error-object
{"level":"error","msg":"bad cart","error":{}}
```

- Verdict: idiom. `console.log`, `console.warn` and `console.error` write text. A logger with levels and fields is a few lines over `JSON.stringify`.
- `JSON.stringify` escapes the newline in the user's text, so the text stays in one record.
- Copy the `name` and `message` of an error into fields. An `Error` object serializes as `{}`.
