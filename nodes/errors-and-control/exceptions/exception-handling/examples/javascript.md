```javascript run id=ports
class ConfigError extends Error {}

function parsePort(text) {
  let port;
  try {
    port = Number(BigInt(text));
  } catch (err) {
    throw new ConfigError(`not a number: ${text}`, { cause: err });
  }
  if (port < 1 || port > 65535) {
    throw new ConfigError(`out of range: ${port}`);
  }
  return port;
}

let checked = 0;
for (const text of ["8080", "70000", "abc"]) {
  try {
    console.log("port", parsePort(text));
  } catch (e) {
    if (!(e instanceof ConfigError)) throw e;
    const cause = e.cause ? e.cause.constructor.name : "none";
    console.log(`error: ${e.message} (cause: ${cause})`);
  } finally {
    checked++;
  }
}
console.log("checked", checked);
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: SyntaxError)
checked 3
```

- Verdict: native. `throw` takes any value, and `try` with `catch` and `finally` handles it.
- A `catch` clause has no type test. The handler tests the value with `instanceof` and throws it again when it does not match.
- `Number("abc")` gives `NaN` and does not throw. `BigInt("abc")` throws a `SyntaxError`, so the function uses it to find text that is not an integer.
- The `cause` option of the `Error` constructor stores the original error.
- An `async` function returns a promise. Its exception reaches a `catch` only through `await` (see Pitfalls on the concept page).
