```rust run id=order-data
mod order {
    pub struct OrderData {
        order_id: u32, // private: no code outside this module can write it
        customer: String,
    }

    impl OrderData {
        pub fn order_id(&self) -> u32 {
            self.order_id
        }
        pub fn customer(&self) -> &str {
            &self.customer
        }
    }

    pub struct Order {
        data: OrderData,    // fixed after construction
        items: Vec<String>, // changes
    }

    impl Order {
        pub fn new(order_id: u32, customer: &str, items: &[&str]) -> Order {
            Order {
                data: OrderData { order_id, customer: customer.to_string() },
                items: items.iter().map(|s| s.to_string()).collect(),
            }
        }
        pub fn add(&mut self, item: &str) {
            self.items.push(item.to_string());
        }
        pub fn describe(&self) -> String {
            format!("order {} for {}: {} items", self.data.order_id(), self.data.customer(), self.items.len())
        }
    }
}

fn main() {
    let mut order = order::Order::new(42, "ann", &["pen", "ink"]);
    println!("{}", order.describe());
    order.add("paper");
    println!("{}", order.describe());
}
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. The fields of a struct are private even when the struct is public, so the module boundary gives read-only access through methods. A shared reference `&OrderData` also cannot change the data.
