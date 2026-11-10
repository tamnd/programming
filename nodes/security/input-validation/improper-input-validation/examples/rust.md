```rust run id=parse
mod order {
    pub struct Order {
        product: &'static str,
        quantity: u8,
    }

    const PRICES: [(&str, u32); 2] = [("pen", 150), ("book", 1200)]; // in cents

    impl Order {
        pub fn parse(product: &str, quantity: &str) -> Result<Order, String> {
            let (name, _) = PRICES
                .iter()
                .find(|(name, _)| *name == product)
                .ok_or("unknown product")?;
            if quantity.is_empty() || quantity.len() > 3 || !quantity.bytes().all(|b| b.is_ascii_digit()) {
                return Err("quantity must be 1 to 3 ASCII digits".into());
            }
            let quantity: u8 = quantity.parse().map_err(|_| "quantity is too large")?;
            if !(1..=100).contains(&quantity) {
                return Err("quantity must be from 1 to 100".into());
            }
            Ok(Order { product: name, quantity })
        }

        pub fn total(&self) -> u32 {
            let (_, price) = PRICES.iter().find(|(name, _)| *name == self.product).unwrap();
            price * u32::from(self.quantity)
        }
    }
}

fn main() {
    for (product, quantity) in [("book", "2"), ("book", "-3"), ("book", "+2"), ("book", "300"), ("boat", "1")] {
        match order::Order::parse(product, quantity) {
            Ok(order) => println!("{}", order.total()),
            Err(error) => println!("rejected: {error}"),
        }
    }
}
```

```output for=parse
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity is too large
rejected: unknown product
```

```rust fails expect=compile id=private-fields
mod order {
    pub struct Order {
        product: &'static str,
        quantity: u8,
    }
}

fn main() {
    let order = order::Order { product: "book", quantity: 0 };
    let _ = order;
}
```

- Verdict: stdlib. `str::parse` into a sized integer type and a struct with private fields give "parse, do not validate" with no library.
- Parsing to `u8` rejects `-3` and values above 255 by the type alone. The range check from 1 to 100 still has to be written.
- `u8::from_str` accepts a leading `+`, so `"+2"` parses to `2`. The parser rejects it before the conversion, because the field allows digits only.
- The fields of `Order` are private to the module `order`. The second program tries to build an `Order` with quantity `0` from outside the module, and the compiler rejects it. So every `Order` in the program came from `Order::parse`.
