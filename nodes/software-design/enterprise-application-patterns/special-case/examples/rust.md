```rust run id=unknown-customer
use std::collections::HashMap;

// Customer is the interface that callers use.
trait Customer {
    fn is_known(&self) -> bool;
    fn bill_line(&self) -> String;
}

struct KnownCustomer {
    id: u32,
    name: String,
    plan: String,
}

impl Customer for KnownCustomer {
    fn is_known(&self) -> bool {
        true
    }

    fn bill_line(&self) -> String {
        format!("customer {}: {}, plan {}", self.id, self.name, self.plan)
    }
}

// The special case: the same interface, with the behavior for a customer with no record.
struct UnknownCustomer {
    id: u32,
}

impl Customer for UnknownCustomer {
    fn is_known(&self) -> bool {
        false
    }

    fn bill_line(&self) -> String {
        format!("customer {}: current resident, plan basic", self.id)
    }
}

fn find_customer(rows: &HashMap<u32, (&str, &str)>, id: u32) -> Box<dyn Customer> {
    match rows.get(&id) {
        Some((name, plan)) => Box::new(KnownCustomer { id, name: name.to_string(), plan: plan.to_string() }),
        None => Box::new(UnknownCustomer { id }), // not an Option that each caller must check
    }
}

fn main() {
    let rows = HashMap::from([(1, ("Ann", "premium")), (2, ("Bob", "basic"))]);
    let customers: Vec<Box<dyn Customer>> = [1, 2, 9].iter().map(|&id| find_customer(&rows, id)).collect();
    for c in &customers {
        println!("{}", c.bill_line());
    }
    println!("known customers: {}", customers.iter().filter(|c| c.is_known()).count());
}
```

```output
customer 1: Ann, plan premium
customer 2: Bob, plan basic
customer 9: current resident, plan basic
known customers: 2
```

- Verdict: idiom. `Customer` is a trait, and `UnknownCustomer` is a second type that implements it. The finder returns `Box<dyn Customer>`.
- Rust has no null references. Without the special case, the finder would return `Option`, and each caller would have to handle `None`.
