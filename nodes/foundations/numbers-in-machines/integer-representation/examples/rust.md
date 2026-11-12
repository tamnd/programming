```rust run id=checked-wrapping-saturating
fn main() {
    let x: i32 = i32::MAX;
    // + panics on overflow in a debug build, so Rust has methods that say what to do.
    println!("{:?} {} {:?} {}", x.checked_add(1), x.wrapping_add(1), x.overflowing_add(1), x.saturating_add(1));
    println!("{:?} {}", i8::MIN.checked_abs(), i8::MIN.unsigned_abs());

    // `as` keeps the low bits, and widening a signed value copies the sign bit.
    println!("{} {} {} {}", 200i32 as i8, -5i8 as u8, -5i8 as i32, -5i8 as u8 as i32);
    println!("{} {:?}", u8::try_from(300i32).is_err(), i8::try_from(-5i32));

    // Byte order is explicit.
    println!("{:02x?} {:02x?}", (-1000i16).to_be_bytes(), (-1000i16).to_le_bytes());
    println!("{}", i16::from_be_bytes([0xfc, 0x18]));
    println!("{} {}", -16i32 >> 2, (-16i32 as u32) >> 28);
}
```

```output for=checked-wrapping-saturating
None -2147483648 (-2147483648, true) 2147483647
None 128
-56 251 -5 251
true Ok(-5)
[fc, 18] [18, fc]
-1000
-4 15
```

- Verdict: native. Rust has `i8` to `i128` and `u8` to `u128`. In a debug build, `+` panics on overflow, so the program uses methods that state the rule: `checked_add` returns `None`, `wrapping_add` wraps, `overflowing_add` returns a flag, and `saturating_add` stops at the limit. `as` keeps the low bits or extends the sign, and `try_from` returns an error when the value does not fit. `to_be_bytes` and `to_le_bytes` give each byte order.
