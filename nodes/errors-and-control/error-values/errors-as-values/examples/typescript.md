```typescript run id=port
type Result<T, E> = { ok: true; value: T } | { ok: false; error: E };

type ConfigError =
  | { kind: "missing"; key: string }
  | { kind: "invalid"; text: string };

function lookup(config: Record<string, string>, key: string): Result<string, ConfigError> {
  const value = config[key];
  if (value === undefined) {
    return { ok: false, error: { kind: "missing", key } };
  }
  return { ok: true, value };
}

function portSetting(config: Record<string, string>): Result<number, ConfigError> {
  const text = lookup(config, "port");
  if (!text.ok) {
    return text;
  }
  const n = Number(text.value);
  if (!Number.isInteger(n) || n < 1 || n > 65535) {
    return { ok: false, error: { kind: "invalid", text: text.value } };
  }
  return { ok: true, value: n };
}

function describe(error: ConfigError): string {
  switch (error.kind) {
    case "missing":
      return `no ${error.key}, use the default 80`;
    case "invalid":
      return `invalid port "${error.text}"`;
  }
}

for (const config of [{ port: "8080" }, { port: "x" }, {}]) {
  const result = portSetting(config);
  console.log(result.ok ? `port ${result.value}` : describe(result.error));
}
```

```output
port 8080
invalid port "x"
no port, use the default 80
```

- Verdict: idiom. The language has no result type, but a union of object types with a literal field, a discriminated union, gives one.
- After the test `if (!text.ok)`, the checker narrows `text` to the `ok: true` member, so `text.value` is allowed. Without the test, `tsc --strict` rejects the access to `value`.
- The `switch` in `describe` covers every `kind`. When a new kind is added, `tsc` reports that the function can end without a return value.
- The types are erased at run time. The program runs as the JavaScript that remains.
