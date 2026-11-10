```rust fails expect=panic id=debug-panic
fn order_fits(price: i32, quantity: i32, budget: i32) -> bool {
    let total = price * quantity;
    total <= budget
}

fn main() {
    println!("3 items: {}", order_fits(100, 3, 1000));
    println!("42949673 items: {}", order_fits(100, 42949673, 1000));
}
```

A debug build panics with `attempt to multiply with overflow`. A release build has no such check and wraps the product to 4.

```rust fails expect=compile id=constant
fn main() {
    let count: u8 = 255 + 1;
    println!("{count}");
}
```

When both operands are constants, the lint `arithmetic_overflow` denies the program at compile time.

```rust run id=checked-mul
fn order_fits(price: i32, quantity: i32, budget: i32) -> bool {
    match price.checked_mul(quantity) {
        Some(total) => total <= budget,
        None => false, // the total does not fit in an i32
    }
}

fn main() {
    println!("3 items: {}", order_fits(100, 3, 1000));
    println!("42949673 items: {}", order_fits(100, 42949673, 1000));
    println!("wrapping: {}", 100i32.wrapping_mul(42949673));
}
```

```output for=checked-mul
3 items: true
42949673 items: false
wrapping: 4
```

- Verdict: native. A debug build panics on overflow, and the Rust reference calls overflow a programmer error, not undefined behavior.
- A release build wraps unless the profile sets `overflow-checks = true`. Do not rely on the debug check to protect a release.
- `checked_*` methods return `None` on overflow in every build. `wrapping_*`, `overflowing_*` and `saturating_*` state the other intents in the code.
- A conversion with `as` truncates silently. `i32::try_from(value)` returns an error when the value does not fit.
