```javascript run id=tax-service-stub
// The TaxService interface: rateBp(region) gives the rate in basis points; 650 is 6.5%.

// The real service. Here it stands for a remote call that is not reachable.
class RemoteTaxService {
  rateBp(region) {
    throw new Error("the tax service is not reachable");
  }
}

// Runs locally, fast and in memory, with canned rates.
class TaxServiceStub {
  static RATES = new Map([
    ["north", 650],
    ["south", 0],
  ]);
  calls = 0;

  rateBp(region) {
    this.calls++;
    return TaxServiceStub.RATES.get(region) ?? 500; // a default rate for the other regions
  }
}

class BillingService {
  constructor(taxes) {
    this.taxes = taxes;
  }

  totalCents(amountCents, region) {
    return amountCents + Math.floor((amountCents * this.taxes.rateBp(region)) / 10000);
  }
}

const money = (c) => `${Math.floor(c / 100)}.${String(c % 100).padStart(2, "0")}`;

try {
  new BillingService(new RemoteTaxService()).totalCents(10000, "north");
} catch (e) {
  console.log("real service: error:", e.message);
}

const stub = new TaxServiceStub();
const billing = new BillingService(stub);
for (const region of ["north", "south", "east"]) {
  console.log(`${region}: 100.00 with tax is ${money(billing.totalCents(10000, region))}`);
}
console.log("the stub got", stub.calls, "calls");
```

```output
real service: error: the tax service is not reachable
north: 100.00 with tax is 106.50
south: 100.00 with tax is 100.00
east: 100.00 with tax is 105.00
the stub got 3 calls
```

- Verdict: idiom. Any object with a `rateBp` method can be the tax service, so the stub needs no common class. `??` gives the default rate only when the region is not in the map, and keeps the rate 0 of the south.
