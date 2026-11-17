```rust run id=trait-bounds
use std::fmt::Display;

fn largest<T: PartialOrd + Copy>(items: &[T]) -> T {
    let mut best = items[0];
    for &x in items {
        if x > best {
            best = x;
        }
    }
    best
}

fn show_pair<T, U>(a: T, b: U) -> String
where
    T: Display,
    U: Display + Clone,
{
    format!("{a} and {b}")
}

fn main() {
    println!("{}", largest(&[3, 9, 4]));
    println!("{}", largest(&['x', 'b']));
    println!("{}", show_pair(1.5, "two"));
}
```

```output for=trait-bounds
9
x
1.5 and two
```

```rust fails expect=compile id=missing-trait
fn largest<T: PartialOrd + Copy>(items: &[T]) -> T {
    let mut best = items[0];
    for &x in items {
        if x > best {
            best = x;
        }
    }
    best
}

struct Point {
    x: i32,
}

fn main() {
    let p = largest(&[Point { x: 1 }]);
    println!("{}", p.x);
}
```

- Verdict: native. `T: PartialOrd + Copy` lets the body compare and copy the items. The `where` clause puts the bounds of `show_pair` after the signature. `Point` does not implement `PartialOrd` or `Copy`, so the second program does not compile.
