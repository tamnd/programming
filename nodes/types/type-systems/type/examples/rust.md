```rust run id=inferred-types-and-sizes
use std::any::type_name;
use std::mem::size_of;

// A tuple struct with one field makes a new type from an old one.
#[derive(Debug, Clone, Copy)]
struct Meters(f64);

fn name_of<T>(_: &T) -> &'static str {
    type_name::<T>()
}

fn main() {
    // The type of a literal comes from its suffix or from the context.
    let a = 7;
    let b = 7u8;
    let c: i64 = 7;
    let d = 7.0;
    println!("{} {} {} {}", name_of(&a), name_of(&b), name_of(&c), name_of(&d));

    // The type decides the size in memory.
    println!("{} {} {} {} {}", size_of::<u8>(), size_of::<i64>(), size_of::<char>(), size_of::<()>(), size_of::<Meters>());

    // Two integer types do not mix without a conversion.
    let total = c + i64::from(b);
    println!("{total}");

    // Option<&T> uses the null value for None, so it costs no space.
    println!("{} {}", size_of::<&u8>(), size_of::<Option<&u8>>());

    // The new type keeps its own name and its own operations.
    let m = Meters(3.5);
    println!("{:?} {}", m, m.0 * 2.0);

    // The unit type () has one value.
    let unit: () = ();
    println!("{:?} {}", unit, name_of(&unit));
}
```

```output for=inferred-types-and-sizes
i32 u8 i64 f64
1 8 4 0 8
14
8 8
Meters(3.5) 7
() ()
```

- Verdict: native. A literal with no suffix is `i32` or `f64`, and a suffix or an annotation gives another type. `size_of` shows that `char` takes 4 bytes, `()` takes none, and the newtype `Meters` takes as much as its `f64`. `i64` and `u8` do not mix without `i64::from`. `Option<&u8>` is as small as `&u8`, because `None` uses the null value.
