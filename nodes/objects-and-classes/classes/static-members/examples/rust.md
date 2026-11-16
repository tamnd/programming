```rust run id=associated-functions-and-consts
use std::sync::atomic::{AtomicU32, Ordering};

static CREATED: AtomicU32 = AtomicU32::new(0); // module-level: Rust has no static fields

struct Order {
    id: u32,
    amount: u32,
}

impl Order {
    const TAX_RATE: f64 = 0.2; // an associated constant

    fn new(amount: u32) -> Order { // an associated function: no self
        let id = CREATED.fetch_add(1, Ordering::Relaxed) + 1;
        Order { id, amount }
    }

    fn from_text(text: &str) -> Option<Order> {
        text.parse().ok().map(Order::new)
    }

    fn created() -> u32 {
        CREATED.load(Ordering::Relaxed)
    }

    fn total(&self) -> f64 { // a method: it has self
        self.amount as f64 * (1.0 + Self::TAX_RATE)
    }
}

fn main() {
    let a = Order::new(100);
    let b = Order::from_text("50").unwrap();
    println!("{} {} {}", a.id, b.id, Order::created());
    println!("{} {}", a.total(), Order::TAX_RATE);
}
```

```output for=associated-functions-and-consts
1 2 2
120 0.2
```

- Verdict: native. An `impl` block holds associated functions with no `self`, called as `Order::new`, and associated constants, but no associated variables.
- A changing value shared by all orders is a `static` item at the module level. A `static` must be safe to share between threads, so the counter is an `AtomicU32`; a `static mut` would need `unsafe` code at each use.
