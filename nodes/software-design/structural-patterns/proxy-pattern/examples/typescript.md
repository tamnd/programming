```typescript run id=report
interface Report {
  render(): string;
}

class DiskReport implements Report {
  private readonly orders: number;
  constructor() {
    console.log("load: read the report from disk");
    this.orders = 42;
  }
  render(): string {
    return `report: ${this.orders} orders`;
  }
}

// A virtual proxy: it has the same interface and makes the real object on first use.
class LazyReport implements Report {
  private real: Report | null = null;
  private readonly factory: () => Report;
  constructor(factory: () => Report) {
    this.factory = factory;
  }
  render(): string {
    this.real ??= this.factory();
    return this.real.render();
  }
}

// A protection proxy: it checks the role before it forwards.
class GuardedReport implements Report {
  private readonly inner: Report;
  private readonly role: string;
  constructor(inner: Report, role: string) {
    this.inner = inner;
    this.role = role;
  }
  render(): string {
    return this.role === "admin" ? this.inner.render() : "access denied";
  }
}

const report: Report = new LazyReport(() => new DiskReport());
console.log("proxy made");
console.log(report.render());
console.log(report.render());
for (const role of ["guest", "admin"]) {
  console.log(`${role}: ${new GuardedReport(report, role).render()}`);
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: idiom. Each proxy is a class that implements the same interface as the real object. The client holds a `Report` and cannot tell which class it has.
- A static proxy is checked by `tsc`. A method added to `Report` makes each proxy class that lacks it a type error. A built-in `Proxy`, as in the JavaScript example, forwards the method with no change, but `tsc` cannot check its traps.
- `??=` makes the real object only when `real` is `null`, so the second call reuses it.
