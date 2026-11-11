```rust run id=points
use std::collections::HashSet;

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let a = Point { x: 1, y: 2 };
    let b = Point { x: 1, y: 2 };
    let c = &a;
    println!("a == b: {}", a == b);
    println!("ptr::eq(&a, &b): {}", std::ptr::eq(&a, &b));
    println!("ptr::eq(&a, c): {}", std::ptr::eq(&a, c));

    let set: HashSet<Point> = [a.clone(), b.clone(), c.clone()].into_iter().collect();
    println!("distinct points: {}", set.len());

    let s = String::from("hello");
    println!("s == \"hello\": {}", s == "hello");

    let nan = f64::NAN;
    println!("nan == nan: {}", nan == nan);
    println!("nan.total_cmp(&nan): {:?}", nan.total_cmp(&nan));
}
```

```output for=points
a == b: true
ptr::eq(&a, &b): false
ptr::eq(&a, c): true
distinct points: 1
s == "hello": true
nan == nan: false
nan.total_cmp(&nan): Equal
```

- Verdict: native. `==` calls `PartialEq::eq`, and `#[derive(PartialEq)]` compares the fields. `std::ptr::eq` compares addresses.
- `Eq` marks a type whose equality is reflexive. `f64` is `PartialEq` but not `Eq`, because `NaN != NaN`.
- `HashSet` and `HashMap` need `Eq + Hash` for their keys. The derive keeps the two consistent.

```rust fails expect=compile id=no-partialeq
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let a = Point { x: 1, y: 2 };
    let b = Point { x: 1, y: 2 };
    println!("{}", a == b);
    println!("{} {}", a.x, b.y);
}
```

`rustc` reports error E0369: binary operation `==` cannot be applied to type `Point`. A type has no `==` until it implements `PartialEq`. The fix is `#[derive(PartialEq)]`.

```rust fails expect=compile id=float-key
use std::collections::HashSet;

fn main() {
    let mut seen = HashSet::new();
    seen.insert(0.5_f64);
    println!("{}", seen.len());
}
```

`f64` does not implement `Eq` or `Hash`, so `insert` is not available. Use integer keys, or wrap the value in a type that defines equality with `total_cmp` and hashes the bits.
