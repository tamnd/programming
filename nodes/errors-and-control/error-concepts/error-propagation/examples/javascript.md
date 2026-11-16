```javascript run id=propagate-javascript
function parsePort(text) {
  if (!/^[0-9]+$/.test(text)) throw new Error(`not a number: ${text}`);
  return Number(text);
}

function loadConfig(line) {
  const value = line.slice(line.indexOf("=") + 1);
  try {
    return parsePort(value);
  } catch (e) {
    // Add context, and keep the original error as the cause.
    throw new Error(`config line '${line}'`, { cause: e });
  }
}

for (const line of ["port=8080", "port=x"]) {
  try {
    console.log(`port ${loadConfig(line)}`);
  } catch (e) {
    console.log(`error: ${e.message}: ${e.cause.message}`);
  }
}
```

```output for=propagate-javascript
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. A thrown error goes up the call stack to the nearest `catch`. The option `cause` of the `Error` constructor keeps the original error, so that its stack trace stays available to a higher handler.
