```rust run id=derive
#[derive(Debug, Clone, PartialEq, Default)]
struct Point {
    x: i32,
    y: i32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
enum Level {
    Low,
    High,
}

fn main() {
    let a = Point { x: 1, y: 2 };
    let b = a.clone();
    println!("{:?}", a);
    println!("a == b: {}", a == b);
    println!("default: {:?}", Point::default());
    println!("Low < High: {}", Level::Low < Level::High);
    #[cfg(target_pointer_width = "64")]
    println!("64-bit target");
}
```

```output for=derive
Point { x: 1, y: 2 }
a == b: true
default: Point { x: 0, y: 0 }
Low < High: true
64-bit target
```

`#[derive(...)]` asks the compiler to write the implementations of the listed traits. `PartialEq` compares the fields one by one, `Debug` prints the fields, and `PartialOrd` on an enum orders the variants by their position. `#[cfg(...)]` keeps the statement only when the condition holds for the target.

```rust fails expect=compile id=no-derive
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let a = Point { x: 1, y: 2 };
    let b = Point { x: 1, y: 2 };
    println!("{}", a == b);
}
```

Rust gives a struct no equality by default. Without `#[derive(PartialEq)]` or a hand-written `impl`, `==` is a compile error (E0369).

```rust fails expect=compile id=derive-bound
use std::marker::PhantomData;

struct Meters;

#[derive(Clone)]
struct Id<T> {
    value: u32,
    unit: PhantomData<T>,
}

fn main() {
    let a: Id<Meters> = Id { value: 7, unit: PhantomData };
    let b = a.clone();
    println!("{}", b.value);
}
```

The derived `impl` requires `T: Clone`, even though `PhantomData<T>` is `Clone` for each `T`. `Meters` is not `Clone`, so `a.clone()` does not exist. Write the `impl Clone` by hand, or derive `Clone` for `Meters` too.

```rust fails expect=compile id=must-use
#![deny(unused_must_use)]

#[must_use = "the new balance is the result"]
fn deposit(balance: u64, amount: u64) -> u64 {
    balance + amount
}

fn main() {
    let balance = 100;
    deposit(balance, 50);
    println!("{balance}");
}
```

`#[must_use]` marks a value that the caller should use. Discarding it triggers the lint `unused_must_use`, which is a warning by default. The attribute `#![deny(unused_must_use)]` changes the lint level for the crate, so the warning becomes an error.

- Verdict: native. Attributes are written `#[...]` on the item that follows or `#![...]` on the enclosing item. Built-in attributes control the compiler, and `derive` runs derive macros that generate trait implementations.
- Procedural macros can define new attributes and new derives. They live in a separate crate, so no procedural macro runs on this page.
