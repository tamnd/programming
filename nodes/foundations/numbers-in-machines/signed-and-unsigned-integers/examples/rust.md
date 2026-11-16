```rust run id=convert
fn main() {
    let a: i32 = -1;
    let b: u32 = 1;
    // a < b does not compile: the types differ.
    println!("a as u32: {}", a as u32);
    println!("a.cast_unsigned(): {}", a.cast_unsigned());
    println!("u32::try_from(a): {}", if u32::try_from(a).is_ok() { "ok" } else { "error" });
    println!("i64::from(a) < i64::from(b): {}", if i64::from(a) < i64::from(b) { "yes" } else { "no" });

    println!("3u32.checked_sub(5): {:?}", 3u32.checked_sub(5));
    println!("i32::MIN.unsigned_abs(): {}", i32::MIN.unsigned_abs());
}
```

```output for=convert
a as u32: 4294967295
a.cast_unsigned(): 4294967295
u32::try_from(a): error
i64::from(a) < i64::from(b): yes
3u32.checked_sub(5): None
i32::MIN.unsigned_abs(): 2147483648
```

- Verdict: native. Rust has `u8` to `u128` and `usize` next to the signed types.
- Rust does not compare or add an `i32` and a `u32`. `as` and `cast_unsigned` keep the bits, `try_from` fails when the value does not fit, and `i64::from` widens with no loss.
- `checked_sub` returns `None` when an unsigned result would go below zero.
- `unsigned_abs` returns a `u32`, so the absolute value of `i32::MIN` fits.
