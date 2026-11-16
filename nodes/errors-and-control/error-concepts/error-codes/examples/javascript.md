```javascript run id=codes-javascript
const OK = 0, EMPTY = 1, NOT_A_NUMBER = 2, OUT_OF_RANGE = 3;
const NAMES = ["ok", "empty", "not a number", "out of range"];

// Returns [code, port]. The port is valid only when the code is OK.
function parsePort(text) {
  if (text === "") return [EMPTY, 0];
  if (!/^[0-9]+$/.test(text)) return [NOT_A_NUMBER, 0];
  const port = Number(text);
  if (port > 65535) return [OUT_OF_RANGE, 0];
  return [OK, port];
}

for (const text of ["8080", "", "x", "99999"]) {
  const [code, port] = parsePort(text);
  if (code !== OK) {
    console.log(`'${text}' -> error code ${code} (${NAMES[code]})`);
  } else {
    console.log(`'${text}' -> port ${port}`);
  }
}
```

```output for=codes-javascript
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. JavaScript reports errors with exceptions. Node.js adds a code to many of its errors: `error.code` is "a string label that identifies the kind of error", such as `ENOENT` for a file that does not exist.
