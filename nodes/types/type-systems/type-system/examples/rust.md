```rust run id=trait-bounds-and-unsafe
use std::any::Any;
use std::fmt::Display;

// A trait bound is checked when the program compiles.
fn show_all<T: Display>(items: &[T]) -> String {
    items.iter().map(|i| i.to_string()).collect::<Vec<_>>().join(",")
}

fn main() {
    println!("{} {}", show_all(&[1, 2, 3]), show_all(&["a", "b"]));

    // No implicit conversions: u8 to u32 needs From, and u32 to u8 needs a fallible try_from.
    let small: u8 = 200;
    let wide: u32 = u32::from(small) + 100;
    println!("{} {:?} {:?}", wide, u8::try_from(wide), u8::try_from(100u32));

    // Any gives a checked downcast at run time.
    let values: Vec<Box<dyn Any>> = vec![Box::new(5i32), Box::new("five")];
    for v in &values {
        match v.downcast_ref::<i32>() {
            Some(n) => println!("i32 {n}"),
            None => println!("not an i32"),
        }
    }

    // unsafe code may break the rules, so the programmer must check it.
    let f = 1.0f32;
    let bits: u32 = unsafe { *(&f as *const f32 as *const u32) };
    println!("{bits:#x} {:#x}", f.to_bits());
}
```

```output for=trait-bounds-and-unsafe
1,2,3 a,b
300 Err(TryFromIntError(PosOverflow)) Ok(100)
i32 5
not an i32
0x3f800000 0x3f800000
```

- Verdict: native. A trait bound such as `T: Display` is checked at compile time. Rust has no implicit numeric conversions: `u32::from` widens a `u8`, and `u8::try_from` returns an error when the value does not fit. `Any` gives a checked downcast at run time. In an `unsafe` block, a pointer cast reads the bits of an `f32` as a `u32`, and the programmer must make sure that this is valid.
