```rust run id=fields
fn main() {
    for x in [1.0f64, -2.5, 0.1] {
        println!("{:>4} binary32 {:08x} binary64 {:016x}", x, (x as f32).to_bits(), x.to_bits());
    }

    let b = (-2.5f64).to_bits();
    println!(
        "-2.5: sign {}, exponent {}, fraction {:x}",
        b >> 63,
        ((b >> 52) & 0x7ff) as i64 - 1023,
        b & ((1u64 << 52) - 1)
    );

    println!("MANTISSA_DIGITS: f32 {}, f64 {}", f32::MANTISSA_DIGITS, f64::MANTISSA_DIGITS);
    println!("MAX_EXP: f32 {}, f64 {}", f32::MAX_EXP, f64::MAX_EXP);
    println!("0.1.mul_add(10.0, -1.0): {:e}", 0.1f64.mul_add(10.0, -1.0));
}
```

```output for=fields
   1 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary32 c0200000 binary64 c004000000000000
 0.1 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
MANTISSA_DIGITS: f32 24, f64 53
MAX_EXP: f32 128, f64 1024
0.1.mul_add(10.0, -1.0): 5.551115123125783e-17
```

- Verdict: native. Rust `f32` and `f64` are binary32 and binary64.
- The method `to_bits` gives the bits, and `from_bits` does the reverse.
- `MANTISSA_DIGITS` counts the hidden bit, so it is 24 and 53. `MAX_EXP` uses the same convention as C, so it is one more than the largest exponent.
- `mul_add` is the fused multiply-add of the standard, with one rounding.
