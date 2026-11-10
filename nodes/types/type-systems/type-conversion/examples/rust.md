```rust run id=conversions
fn main() {
    let small: i32 = 300;
    let wide: i64 = small.into();
    let low = small as u8;
    let checked = u8::try_from(small);
    println!("{wide} {low} {checked:?}");
    println!("{} {} {}", 3.99_f64 as i32, -1.0e10_f64 as i32, f64::NAN as i32);
    println!("{}", -1_i32 as u32);
    println!("{:?}", "42".parse::<i32>());
    println!("{:?}", "42abc".parse::<i32>());
}
```

```output for=conversions
300 44 Err(TryFromIntError(PosOverflow))
3 -2147483648 0
4294967295
Ok(42)
Err(ParseIntError { kind: InvalidDigit })
```

- Verdict: native. Rust has no implicit numeric conversion. `as` is a cast that never fails, `From` and `Into` are conversions that cannot fail, and `TryFrom` returns a `Result`.
- `300 as u8` keeps the low 8 bits and gives 44. `u8::try_from(300)` returns an error instead.
- A float to integer `as` cast rounds toward zero, saturates at the limits of the target type and maps NaN to 0.
- `str::parse` returns a `Result`, so a caller must handle bad text.
