```rust run id=checkout
struct Item {
    price: u32, // in cents
    quantity: u32,
}

// The static form: the strategy is a type parameter, fixed at compile time.
struct Checkout<F: Fn(&[Item]) -> u32> {
    pricing: F,
}

impl<F: Fn(&[Item]) -> u32> Checkout<F> {
    fn total(&self, items: &[Item]) -> u32 {
        (self.pricing)(items)
    }
}

fn regular(items: &[Item]) -> u32 {
    items.iter().map(|i| i.price * i.quantity).sum()
}

fn ten_percent_off(items: &[Item]) -> u32 {
    regular(items) * 90 / 100
}

// 50 cents off each unit of a line with 5 or more units.
fn bulk(items: &[Item]) -> u32 {
    items
        .iter()
        .map(|i| {
            let price = if i.quantity >= 5 { i.price - 50 } else { i.price };
            price * i.quantity
        })
        .sum()
}

fn main() {
    let cart = [
        Item { price: 1500, quantity: 2 },
        Item { price: 200, quantity: 5 },
    ];

    // The dynamic form: trait objects in a list, chosen at run time.
    let strategies: Vec<(&str, Box<dyn Fn(&[Item]) -> u32>)> = vec![
        ("regular", Box::new(regular)),
        ("ten percent off", Box::new(ten_percent_off)),
        ("bulk", Box::new(bulk)),
    ];
    for (name, pricing) in &strategies {
        let checkout = Checkout { pricing: |items: &[Item]| pricing(items) };
        println!("{name}: {} cents", checkout.total(&cart));
    }
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. Rust has closures and function items, and the `Fn` traits are the strategy interface.
- `Checkout<F>` is the static form. The compiler makes one copy of `Checkout` for each strategy type and can inline the call. The type of the checkout fixes its strategy, so it cannot change at run time.
- `Box<dyn Fn(&[Item]) -> u32>` is the dynamic form. Values of different closure types share one type, so they fit in one `Vec`, at the cost of a call through a vtable.
- `(self.pricing)(items)` needs the parentheses. Without them, Rust looks for a method named `pricing`.
