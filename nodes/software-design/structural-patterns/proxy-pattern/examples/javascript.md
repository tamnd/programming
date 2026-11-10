```javascript run id=report
class Report {
  constructor() {
    console.log("load: read the report from disk");
    this.orders = 42;
  }
  render() {
    return `report: ${this.orders} orders`;
  }
}

// A virtual proxy: a built-in Proxy whose get trap makes the real object on first use.
function lazy(factory) {
  let real = null;
  return new Proxy(
    {},
    {
      get(_target, name) {
        real ??= factory();
        const value = Reflect.get(real, name);
        return typeof value === "function" ? value.bind(real) : value;
      },
    },
  );
}

// A protection proxy: the get trap replaces render for a role that may not read.
function guarded(report, role) {
  return new Proxy(report, {
    get(target, name, receiver) {
      if (name === "render" && role !== "admin") {
        return () => "access denied";
      }
      return Reflect.get(target, name, receiver);
    },
  });
}

const report = lazy(() => new Report());
console.log("proxy made");
console.log(report.render());
console.log(report.render());
for (const role of ["guest", "admin"]) {
  console.log(`${role}: ${guarded(report, role).render()}`);
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

- Verdict: native. `Proxy` is part of the language. A handler object holds traps, such as `get`, that the engine calls for operations on the proxy. An operation without a trap goes to the target.
- `Reflect.get` does the default lookup, so a trap can change one case and forward the rest.
- The lazy proxy binds each method to the real object. Without `bind`, `this` in `render` would be the proxy, which is a different object. A class with private `#fields` fails in that case, and the node page shows the error.
