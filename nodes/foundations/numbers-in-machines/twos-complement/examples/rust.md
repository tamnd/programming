```rust run id=negation
fn main() {
    let x: i32 = 6;
    println!("-x == !x + 1: {}", if -x == !x + 1 { "yes" } else { "no" });
    println!("bits of -6i8: {:08b}", -6i8);
    println!("-6i8 as i16: {:016b}", -6i8 as i16);

    println!("i32::MIN.checked_neg(): {:?}", i32::MIN.checked_neg());
    println!("i32::MIN.wrapping_neg(): {}", i32::MIN.wrapping_neg());
    println!("i32::MIN.overflowing_neg(): {:?}", i32::MIN.overflowing_neg());
}
```

```output for=negation
-x == !x + 1: yes
bits of -6i8: 11111010
-6i8 as i16: 1111111111111010
i32::MIN.checked_neg(): None
i32::MIN.wrapping_neg(): -2147483648
i32::MIN.overflowing_neg(): (-2147483648, true)
```

- Verdict: native. Rust's signed types `i8` to `i128` and `isize` use two's complement.
- Rust writes bitwise NOT as `!`. The format `{:b}` of a signed value prints its two's complement bits.
- `checked_neg`, `wrapping_neg` and `overflowing_neg` say what to do with the most negative value. A plain `-x` on that value panics in a debug build.
