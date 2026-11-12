```rust run id=binary-formatting
fn main() {
    let n: u32 = 0b1010_1100; // binary literal; the underscore groups digits
    println!("{n} {n:b} {n:o} {n:x} {:08b} {:#010b}", 5, 5);

    // A negative number prints its two's complement bits at the width of its type.
    println!("{:b} {:b}", -5i8, -5i32);

    println!("{:?} {:?}", u8::from_str_radix("101101", 2), u8::from_str_radix("100000000", 2).is_err());
    println!("{:?}", i64::from_str_radix("-ff", 16));

    println!("{} {} {} {}", n.count_ones(), n.leading_zeros(), n.trailing_zeros(), 200u32.ilog2());
    println!("{}", u128::MAX.count_ones());
}
```

```output for=binary-formatting
172 10101100 254 ac 00000101 0b00000101
11111011 11111111111111111111111111111011
Ok(45) true
Ok(-255)
4 24 2 7
128
```

- Verdict: native. The format `{:b}` prints binary, `{:08b}` pads it and `{:#010b}` adds the `0b` prefix inside the width. A negative value prints its two's complement bits at the width of its own type, 8 bits for `i8` and 32 for `i32`. `from_str_radix` returns an error when the value does not fit, as nine bits do not fit in a `u8`. The methods `count_ones`, `leading_zeros`, `trailing_zeros` and `ilog2` work on any integer type.
