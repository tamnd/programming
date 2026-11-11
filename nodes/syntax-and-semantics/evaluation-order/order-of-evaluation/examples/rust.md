```rust run id=compound
use std::num::Wrapping;

fn index(side: &str) -> usize {
    println!("{side}: index");
    0
}

fn value(side: &str) -> i32 {
    println!("{side}: value");
    1
}

fn main() {
    let mut plain = [10, 20];
    plain[index("plain")] += value("plain");

    let mut wrapped = [Wrapping(10), Wrapping(20)];
    wrapped[index("wrapped")] += Wrapping(value("wrapped"));

    println!("{} {}", plain[0], wrapped[0]);
}
```

```output for=compound
plain: value
plain: index
wrapped: index
wrapped: value
11 11
```

- Verdict: native. The Rust Reference states that the operands of an expression are evaluated from left to right as written, and that the arguments of a call are evaluated from left to right.
- Compound assignment is the exception. When both operands are primitive types, the right-hand side is evaluated first. When either operand is another type, `+=` is a call to `AddAssign::add_assign` and the left-hand side is evaluated first. `Wrapping<i32>` is not primitive, so the two lines give opposite orders.
- The Reference warns that code must not depend on this difference.
- Rust has no undefined behavior from evaluation order in safe code, because the borrow checker does not let one expression change a variable while another part of the expression borrows it.
