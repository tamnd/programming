```javascript run id=recoverable-javascript
function portOrDefault(text, fallback) {
  // A bad fallback is a bug in the caller: stop.
  if (!(fallback >= 1 && fallback <= 65535)) {
    throw new RangeError("fallback out of range");
  }
  // Bad text is an expected error: recover.
  const n = Number(text);
  if (!Number.isInteger(n)) {
    return [fallback, " (not a number)"];
  }
  return [n, ""];
}

for (const text of ["8080", "x"]) {
  const [port, note] = portOrDefault(text, 80);
  console.log(`${text} -> ${port}${note}`);
}
```

```output for=recoverable-javascript
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: absent. JavaScript has one mechanism, exceptions, for both kinds. Here the code returns the expected error as a value and throws for the bug. Nothing in the language marks a thrown `Error` as a bug.
