```javascript run id=report-locator
class Locator {
  static #services = new Map();

  static register(name, service) {
    Locator.#services.set(name, service);
  }

  static get(name) {
    if (!Locator.#services.has(name)) throw new Error(`no service: ${name}`);
    return Locator.#services.get(name);
  }
}

class Report {
  print(total) {
    const format = Locator.get("format");
    return format(total);
  }
}

class TaxReport {
  print(total) {
    const tax = Locator.get("tax"); // nobody registered this service
    return Locator.get("format")(total + tax(total));
  }
}

// Configuration: the program puts the services into the locator.
Locator.register("format", (n) => `total: ${n}`);

console.log(new Report().print(12));
const taxReport = new TaxReport(); // building it does not fail
try {
  console.log(taxReport.print(12));
} catch (e) {
  console.log(`error: ${e.message}`);
}
```

```output
total: 12
error: no service: tax
```
