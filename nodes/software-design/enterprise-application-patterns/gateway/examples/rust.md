```rust run id=shipping-gateway
use std::collections::HashMap;

// The foreign API of a shipping service. Its names and types do not fit our code:
// the weight is in grams, the flags are a string, and the result is a map
// with a return code and the amount as a string of cents.
fn foreign_quote(weight_grams: u64, dest: &str, svc_flags: &str) -> HashMap<&'static str, String> {
    println!("  connection: quote(weight_grams={}, dest={}, svc_flags={})", weight_grams, dest, svc_flags);
    let mut r = HashMap::new();
    if weight_grams > 30000 {
        r.insert("rc", "17".to_string());
        r.insert("msg", "too heavy".to_string());
    } else {
        r.insert("rc", "0".to_string());
        r.insert("amt", (500 + weight_grams / 100 * 10).to_string());
        r.insert("cur", "EUR".to_string());
    }
    r
}

type Connection = Box<dyn Fn(u64, &str, &str) -> HashMap<&'static str, String>>;

// Our interface to the shipping service, in the terms of our code.
struct ShippingGateway {
    connection: Connection, // the call to the foreign API; a test can replace it
}

impl ShippingGateway {
    // Returns the cost in cents and the currency, or the reason for no service.
    fn standard_cost(&self, kilograms: f64, country: &str) -> Result<(u64, String), String> {
        let reply = (self.connection)((kilograms * 1000.0).round() as u64, country, "STD");
        if reply["rc"] != "0" {
            return Err(format!("code {}: {}", reply["rc"], reply["msg"]));
        }
        let cents = reply["amt"].parse::<u64>().map_err(|e| e.to_string())?;
        Ok((cents, reply["cur"].clone()))
    }
}

fn show(gateway: &ShippingGateway, kilograms: f64, country: &str) {
    match gateway.standard_cost(kilograms, country) {
        Ok((cents, currency)) => println!(
            "parcel of {:.1} kg to {} costs {}.{:02} {}",
            kilograms, country, cents / 100, cents % 100, currency
        ),
        Err(e) => println!("parcel of {:.1} kg to {}: no service ({})", kilograms, country, e),
    }
}

fn main() {
    let gateway = ShippingGateway { connection: Box::new(foreign_quote) };
    show(&gateway, 1.2, "DE");
    show(&gateway, 31.0, "DE");

    // A test gives the gateway a connection that returns canned data.
    let canned = ShippingGateway {
        connection: Box::new(|_, _, _| {
            let mut r = HashMap::new();
            r.insert("rc", "0".to_string());
            r.insert("amt", "999".to_string());
            r.insert("cur", "EUR".to_string());
            r
        }),
    };
    show(&canned, 2.0, "FR");
}
```

```output
  connection: quote(weight_grams=1200, dest=DE, svc_flags=STD)
parcel of 1.2 kg to DE costs 6.20 EUR
  connection: quote(weight_grams=31000, dest=DE, svc_flags=STD)
parcel of 31.0 kg to DE: no service (code 17: too heavy)
parcel of 2.0 kg to FR costs 9.99 EUR
```

- Verdict: idiom. The connection is a boxed closure, so the program can give a different connection to each gateway. `standard_cost` returns a `Result`, with the reason as the error.
- The `?` operator returns the error of `parse` if the amount is not a number.
