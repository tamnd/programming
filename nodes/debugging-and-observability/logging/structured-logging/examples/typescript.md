```typescript run id=records
type Level = "debug" | "info" | "warn" | "error";
type Fields = Record<string, string | number | boolean>;

const rank: Record<Level, number> = { debug: 10, info: 20, warn: 30, error: 40 };

class Logger {
  private readonly minLevel: Level;
  private readonly fields: Fields;

  constructor(minLevel: Level, fields: Fields = {}) {
    this.minLevel = minLevel;
    this.fields = fields;
  }

  log(level: Level, msg: string, extra: Fields = {}): void {
    if (rank[level] < rank[this.minLevel]) {
      return;
    }
    console.log(JSON.stringify({ level, msg, ...this.fields, ...extra }));
  }

  child(more: Fields): Logger {
    return new Logger(this.minLevel, { ...this.fields, ...more });
  }
}

const log = new Logger("info");
log.log("debug", "cart loaded", { items: 3 }); // below info: dropped
log.log("info", "order placed", { order: 1042, totalCents: 1999 });
const req = log.child({ request: "r-7" });
req.log("warn", "stock low", { sku: "A-17", left: 2 });
req.log("error", "payment refused", { order: 1043, retry: false });
```

```output for=records
{"level":"info","msg":"order placed","order":1042,"totalCents":1999}
{"level":"warn","msg":"stock low","request":"r-7","sku":"A-17","left":2}
{"level":"error","msg":"payment refused","request":"r-7","order":1043,"retry":false}
```

- Verdict: idiom. The program is the JavaScript logger with types.
- The union type `Level` lets the compiler reject a level name that does not exist.
- `Fields` allows only strings, numbers and booleans as values, so an `Error` object cannot go into a record by mistake.
