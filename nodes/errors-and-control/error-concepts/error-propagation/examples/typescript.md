```typescript run id=propagate-typescript
function parsePort(text: string): number {
  if (!/^[0-9]+$/.test(text)) throw new Error(`not a number: ${text}`);
  return Number(text);
}

function loadConfig(line: string): number {
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
    const err = e as Error;
    const cause = err.cause as Error;
    console.log(`error: ${err.message}: ${cause.message}`);
  }
}
```

```output for=propagate-typescript
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. As in JavaScript, an error goes up the stack to the nearest `catch`. The caught value has the type `unknown`, so the code gives it a type before it reads `message` and `cause`. A function type does not show which errors the function can throw.
