```rust run id=shop
// The module is the facade. Only the items marked pub can be used outside it.
mod shop {
    use std::collections::HashMap;

    struct Inventory {
        stock: HashMap<String, u32>,
    }

    impl Inventory {
        fn reserve(&mut self, item: &str) -> bool {
            match self.stock.get_mut(item) {
                Some(n) if *n > 0 => {
                    *n -= 1;
                    true
                }
                _ => false,
            }
        }

        fn release(&mut self, item: &str) {
            *self.stock.entry(item.to_string()).or_insert(0) += 1;
        }
    }

    struct Payments;

    impl Payments {
        fn charge(&self, card: &str, _amount: f64) -> bool {
            card != "expired"
        }
    }

    struct Shipping {
        parcels: u32,
    }

    impl Shipping {
        fn schedule(&mut self, _item: &str) -> u32 {
            self.parcels += 1;
            self.parcels
        }
    }

    pub struct Shop {
        inventory: Inventory,
        payments: Payments,
        shipping: Shipping,
    }

    impl Shop {
        pub fn new(stock: &[(&str, u32)]) -> Self {
            let stock = stock.iter().map(|&(k, v)| (k.to_string(), v)).collect();
            Shop { inventory: Inventory { stock }, payments: Payments, shipping: Shipping { parcels: 0 } }
        }

        pub fn place_order(&mut self, item: &str, card: &str, price: f64) -> String {
            if !self.inventory.reserve(item) {
                return format!("failed: {item} is out of stock");
            }
            if !self.payments.charge(card, price) {
                self.inventory.release(item);
                return format!("failed: card declined, {item} back in stock");
            }
            let parcel = self.shipping.schedule(item);
            format!("ok: {item}, paid {price:.2}, parcel {parcel}")
        }

        pub fn stock(&self, item: &str) -> u32 {
            self.inventory.stock.get(item).copied().unwrap_or(0)
        }
    }
}

fn main() {
    let mut shop = shop::Shop::new(&[("book", 2), ("lamp", 0)]);
    println!("{}", shop.place_order("book", "visa", 12.50));
    println!("{}", shop.place_order("book", "expired", 12.50));
    println!("{}", shop.place_order("lamp", "visa", 30.00));
    println!("stock of book: {}", shop.stock("book"));
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: native. Items in a Rust module are private by default. `Shop` and its three `pub` methods are the only names that `main` can use, so the compiler enforces the facade.
- A library can make a facade over its inner modules with `pub use`, which gives an item a public path in a different place.
- `place_order` takes `&mut self`, because an order changes the inventory and the parcel counter. The borrow checker then forbids two orders at the same time through one `Shop`.
