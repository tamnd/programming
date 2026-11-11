```rust run id=iterators
#[derive(Debug, Clone)]
struct Order {
    item: &'static str,
    qty: u32,
    price: u32,
}

fn main() {
    let orders = vec![
        Order { item: "pen", qty: 3, price: 2 },
        Order { item: "book", qty: 1, price: 12 },
        Order { item: "ink", qty: 2, price: 5 },
    ];

    let total: u32 = orders.iter().filter(|o| o.qty > 1).map(|o| o.qty * o.price).sum();
    println!("{total}");

    let compose = |f: fn(u32) -> u32, g: fn(u32) -> u32| move |x| g(f(x));
    let inc_then_double = compose(|x| x + 1, |x| x * 2);
    println!("{}", inc_then_double(5));

    let items = orders.iter().fold(String::new(), |acc, o| if acc.is_empty() { o.item.to_string() } else { acc + "," + o.item });
    println!("{items}");

    let cheaper = Order { price: 10, ..orders[1].clone() };
    println!("{} {}", cheaper.price, orders[1].price);
}
```

```output for=iterators
16
12
pen,book,ink
10 12
```

- Verdict: native. Rust has closures, iterators with `map`, `filter` and `fold`, enums with `match`, and variables that are immutable unless marked `mut`. The Rust book names functional programming as "one significant influence" on the design.
- Rust allows mutation, but the borrow rules stop a value from being changed while other code reads it.
