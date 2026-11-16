```javascript run id=error-javascript
function parseInteger(text) {
  const n = Number(text);
  if (!Number.isInteger(n)) {
    throw new Error(`not an integer: ${text}`);
  }
  return n;
}

for (const text of ["42", "x42"]) {
  try {
    console.log(`${text} -> ${parseInteger(text)}`);
  } catch (e) {
    console.log(`${text} -> error (${e.name})`);
  }
}
```

```output for=error-javascript
42 -> 42
x42 -> error (Error)
```

- Verdict: native. JavaScript reports errors with exceptions: a `throw` statement and `try...catch`. `Number` does not throw for a bad string; it gives `NaN`. So the function tests the result and throws an `Error` object itself.
