```rust run id=impl-blocks-in-modules
mod order {
    pub struct Order {
        pub count: u32,
        pub price: u32,
    }

    impl Order {
        pub fn new(count: u32, price: u32) -> Order {
            Order { count, price }
        }
    }
}

mod pricing {
    use super::order::Order;

    impl Order {
        // a second impl block for the same type, in another module
        pub fn total(&self) -> u32 {
            self.count * self.price
        }
    }
}

impl std::fmt::Display for order::Order {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "Order({}, {})", self.count, self.price)
    }
}

fn main() {
    let o = order::Order::new(3, 40);
    println!("{} {}", o.total(), o);
}
```

```output for=impl-blocks-in-modules
120 Order(3, 40)
```

- Verdict: idiom. A type can have many `impl` blocks, and they can be in any module of the crate. Here `new` is in the module `order` and `total` is in the module `pricing`.
- The fields are in one `struct` declaration. Another crate cannot write an `impl Order` block; it must define a trait and implement that for `Order`.
