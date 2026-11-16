```javascript run id=throw-helper
// throw is a statement, so a helper function lets a throw sit inside an expression.
function fail(msg) {
  throw new Error(msg);
}

function port(s) {
  const n = Number(s);
  return Number.isInteger(n) ? n : fail("bad port: " + s);
}

console.log(port("8080"));
try {
  port("x");
} catch (e) {
  console.log(e.message);
}

const config = {};
try {
  const host = config.host ?? fail("no host");
  console.log(host);
} catch (e) {
  console.log(e.message);
}
```

```output for=throw-helper
8080
bad port: x
no host
```

- Verdict: absent. JavaScript has no static types, so it has no bottom type. `throw` is a statement and cannot be an operand of `?:` or `??`, so code calls a helper such as `fail` that always throws. In TypeScript, the result type of such a helper is `never`.
