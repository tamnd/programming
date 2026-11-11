```javascript run id=port
function lookup(config, key) {
  if (!(key in config)) {
    return { ok: false, error: new Error(`missing key "${key}"`), missing: true };
  }
  return { ok: true, value: config[key] };
}

function parsePort(text) {
  const n = Number(text);
  if (!Number.isInteger(n) || n < 1 || n > 65535) {
    return { ok: false, error: new Error(`invalid port "${text}"`) };
  }
  return { ok: true, value: n };
}

function portSetting(config) {
  const text = lookup(config, "port");
  if (!text.ok) {
    return text;
  }
  const port = parsePort(text.value);
  if (!port.ok) {
    return { ok: false, error: new Error("port setting", { cause: port.error }) };
  }
  return port;
}

for (const config of [{ port: "8080" }, { port: "x" }, {}]) {
  const result = portSetting(config);
  if (result.ok) {
    console.log("port", result.value);
  } else if (result.missing) {
    console.log("use the default 80");
  } else {
    console.log(`error: ${result.error.message}: ${result.error.cause.message}`);
  }
}

const settled = await Promise.allSettled([
  Promise.resolve(1),
  Promise.reject(new Error("timed out")),
]);
console.log(settled.map((s) => s.status).join(", "));
```

```output
port 8080
error: port setting: invalid port "x"
use the default 80
fulfilled, rejected
```

- Verdict: idiom. JavaScript reports errors with exceptions and rejected promises. A function can return an object with an `ok` field, and the caller tests the field.
- The option `cause` of the `Error` constructor keeps the first error inside the second one, as wrapping does in Go.
- `Promise.allSettled` turns each outcome into a value: an object with `status` set to `"fulfilled"` or `"rejected"`. One failed promise does not hide the others.
