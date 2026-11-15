```rust run id=tax-service-stub
use std::cell::Cell;

// The rate in basis points: 650 is 6.5%.
trait TaxService {
    fn rate_bp(&self, region: &str) -> Result<i64, String>;
}

// The real service. Here it stands for a remote call that is not reachable.
struct RemoteTaxService;

impl TaxService for RemoteTaxService {
    fn rate_bp(&self, _region: &str) -> Result<i64, String> {
        Err("the tax service is not reachable".to_string())
    }
}

// Runs locally, fast and in memory, with canned rates.
#[derive(Default)]
struct TaxServiceStub {
    calls: Cell<u32>,
}

impl TaxService for TaxServiceStub {
    fn rate_bp(&self, region: &str) -> Result<i64, String> {
        self.calls.set(self.calls.get() + 1);
        Ok(match region {
            "north" => 650,
            "south" => 0,
            _ => 500, // a default rate for the other regions
        })
    }
}

struct BillingService<'a> {
    taxes: &'a dyn TaxService,
}

impl BillingService<'_> {
    fn total_cents(&self, amount: i64, region: &str) -> Result<i64, String> {
        Ok(amount + amount * self.taxes.rate_bp(region)? / 10000)
    }
}

fn money(c: i64) -> String {
    format!("{}.{:02}", c / 100, c % 100)
}

fn main() {
    if let Err(e) = (BillingService { taxes: &RemoteTaxService }).total_cents(10000, "north") {
        println!("real service: error: {}", e);
    }

    let stub = TaxServiceStub::default();
    let billing = BillingService { taxes: &stub };
    for region in ["north", "south", "east"] {
        println!("{}: 100.00 with tax is {}", region, money(billing.total_cents(10000, region).unwrap()));
    }
    println!("the stub got {} calls", stub.calls.get());
}
```

```output
real service: error: the tax service is not reachable
north: 100.00 with tax is 106.50
south: 100.00 with tax is 100.00
east: 100.00 with tax is 105.00
the stub got 3 calls
```

- Verdict: idiom. `TaxService` is a trait, and `BillingService` keeps a `&dyn TaxService`. The stub counts its calls in a `Cell`, because the trait method takes `&self`.
