```rust run id=option
use std::collections::HashMap;
use std::mem::size_of;

fn main() {
    let mut ages = HashMap::new();
    ages.insert("ada", 36);

    for name in ["ada", "bob"] {
        match ages.get(name) {
            Some(age) => println!("{name}: {age}"),
            None => println!("{name}: unknown"),
        }
    }

    let bob = ages.get("bob").copied().unwrap_or(0);
    println!("{bob}");

    println!("{} {}", size_of::<&i32>(), size_of::<Option<&i32>>());
}
```

```output
ada: 36
bob: unknown
0
8 8
```

- Verdict: stdlib. Safe Rust has no null reference. A reference or a `Box` must be non-null, and a null reference is undefined behaviour.
- A value that can be absent has the type `Option<T>`, an enum with the variants `Some(T)` and `None`. `HashMap::get` returns `Option<&V>`. A `match` must handle both variants, so the compiler rejects code that forgets `None`.
- `unwrap_or`, `map` and the `?` operator work on an `Option` without a `match`. `unwrap` panics on `None` (see Pitfalls on the concept page).
- `Option<&T>` has the same size as `&T`, as the last line shows. The compiler uses the all-zero bit pattern, which no reference can have, for `None`. This is the null pointer optimization.
