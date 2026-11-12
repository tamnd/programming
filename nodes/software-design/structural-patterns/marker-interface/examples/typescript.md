```typescript run id=redacting-logger
const sensitive: unique symbol = Symbol("sensitive");

interface Sensitive {
  readonly [sensitive]: true;
}

class Username {
  readonly value: string;
  constructor(value: string) {
    this.value = value;
  }
}

class Password implements Sensitive {
  readonly [sensitive] = true as const;
  readonly value: string;
  constructor(value: string) {
    this.value = value;
  }
}

class ApiKey implements Sensitive {
  readonly [sensitive] = true as const;
  readonly value: string;
  constructor(value: string) {
    this.value = value;
  }
}

function log(label: string, item: Username | Password | ApiKey): void {
  const shown = sensitive in item ? "[redacted]" : item.value;
  console.log(`${label}: ${shown}`);
}

log("user", new Username("ann"));
log("password", new Password("hunter2"));
log("api key", new ApiKey("k-123"));
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. An empty interface would match every object, because TypeScript types are structural. The brand property with a `unique symbol` key makes the interface match only the classes that declare it.
