```javascript run id=error-first
import { promisify } from "node:util";

function parsePort(text, callback) { // error-first continuation
  setImmediate(() => {
    const n = Number(text);
    if (!Number.isInteger(n) || n < 1 || n > 65535) callback(new Error(`bad port: ${text}`));
    else callback(null, n);
  });
}

parsePort("8080", (err, port) => console.log("callback:", err ? err.message : port));
parsePort("http", (err, port) => console.log("callback:", err ? err.message : port));

const parsePortAsync = promisify(parsePort); // the same function, as a promise
try {
  console.log("await:", await parsePortAsync("443"));
  await parsePortAsync("99999");
} catch (e) {
  console.log("await:", e.message);
}
```

```output for=error-first
callback: 8080
callback: bad port: http
await: 443
await: bad port: 99999
```

```javascript wrong id=called-twice
function root(n, callback) {
  if (n < 0) callback(new Error("negative"));
  callback(null, Math.sqrt(n)); // runs even after the error
}

root(-4, (err, v) => console.log(err ? `error: ${err.message}` : `value: ${v}`));
```

```output for=called-twice
error: negative
value: NaN
```

- Verdict: idiom. Node.js passes the result of every asynchronous operation of its older APIs to a callback, which is a continuation. The convention is error-first: the callback takes an error, or `null`, as its first argument and the value as its second. `util.promisify` turns such a function into one that returns a promise, and `await` then lets the caller write the code in direct style while the engine still runs it as continuations.
- The wrong block calls its continuation twice. After the error, `root` does not return, so it also calls the callback with `Math.sqrt(-4)`, and the caller sees an error and then the value `NaN`. Write `return callback(...)`, so that each path calls the continuation exactly once.
