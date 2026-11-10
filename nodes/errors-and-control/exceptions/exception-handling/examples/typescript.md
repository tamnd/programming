```typescript run id=ports
class ConfigError extends Error {
  constructor(message: string, options?: ErrorOptions) {
    super(message, options);
    this.name = "ConfigError";
  }
}

function parsePort(text: string): number {
  let port: number;
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
  } catch (e: unknown) {
    if (!(e instanceof ConfigError)) throw e;
    const cause = e.cause instanceof Error ? e.cause.name : "none";
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

- Verdict: native. The statements are those of JavaScript, and the type checker adds no rule about exceptions.
- A function type does not say which exceptions the function can throw. The checker cannot tell the caller that `parsePort` can fail.
- Under `strict`, the variable of a `catch` clause has the type `unknown`. The code must narrow it, here with `instanceof`, before it reads `message` or `cause`.
- The only annotation allowed on a `catch` variable is `unknown` or `any`.
