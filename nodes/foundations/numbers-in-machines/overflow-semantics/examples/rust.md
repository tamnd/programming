```rust run id=methods
use std::num::Wrapping;

fn main() {
    let x = i32::MAX;
    println!("x.checked_add(1): {:?}", x.checked_add(1));
    println!("x.wrapping_add(1): {}", x.wrapping_add(1));
    println!("x.saturating_add(1): {}", x.saturating_add(1));
    println!("x.overflowing_add(1): {:?}", x.overflowing_add(1));
    let w = Wrapping(x) + Wrapping(1);
    println!("Wrapping(x) + Wrapping(1): {}", w.0);
    println!("(-5i32).checked_add(3): {:?}", (-5i32).checked_add(3));
}
```

```output for=methods
x.checked_add(1): None
x.wrapping_add(1): -2147483648
x.saturating_add(1): 2147483647
x.overflowing_add(1): (-2147483648, true)
Wrapping(x) + Wrapping(1): -2147483648
(-5i32).checked_add(3): Some(-2)
```

- Verdict: native. The plain operators panic on overflow in debug builds. The `checked_`, `wrapping_`, `saturating_` and `overflowing_` methods choose a rule for one operation, and the `Wrapping` type makes the operators wrap.
- The example does not use `x + 1`, because the compiler rejects an overflow that it can find at compile time.
