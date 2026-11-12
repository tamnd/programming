```rust run id=kinds
trait Checkout {
    fn discount(&self, amount: i32) -> i32 {
        amount
    }
    fn total(&self, prices: &[i32]) -> i32 {
        self.discount(prices.iter().sum())
    }
}

struct SaleCheckout;

impl Checkout for SaleCheckout {
    fn discount(&self, amount: i32) -> i32 {
        amount * 9 / 10
    }
}

struct Till {
    discount: Box<dyn Fn(i32) -> i32>,
}

impl Till {
    fn total(&self, prices: &[i32]) -> i32 {
        (self.discount)(prices.iter().sum())
    }
}

fn main() {
    let prices = [40, 60];
    println!("class pattern: {}", SaleCheckout.total(&prices));
    let mut till = Till { discount: Box::new(|amount| amount * 9 / 10) };
    println!("object pattern: {}", till.total(&prices));
    till.discount = Box::new(|amount| amount - 20);
    println!("changed at run time: {}", till.total(&prices));
}
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: idiom. Rust has no inheritance. A trait with a default method takes the place of the base class, and the type overrides one step. The strategy is a boxed closure.
