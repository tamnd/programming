```rust run id=scaled
const SHIFT: u32 = 16;

fn mul(a: i32, b: i32) -> Result<i32, std::num::TryFromIntError> {
    let p = (a as i64 * b as i64 + (1i64 << (SHIFT - 1))) >> SHIFT;
    i32::try_from(p)
}

fn mul_sat(a: i32, b: i32) -> i32 {
    let p = (a as i64 * b as i64 + (1i64 << (SHIFT - 1))) >> SHIFT;
    p.clamp(i32::MIN as i64, i32::MAX as i64) as i32
}

fn main() {
    let a: i32 = 3 << (SHIFT - 1);
    let b: i32 = 9 << (SHIFT - 2);
    println!("1.5 * 2.25: {}", mul(a, b).unwrap() as f64 / 65536.0);
    let big: i32 = 300 << SHIFT;
    println!("300 * 300 is an error: {}", if mul(big, big).is_err() { "yes" } else { "no" });
    println!("300 * 300 saturated: {}", mul_sat(big, big) as f64 / 65536.0);
    let cents: i64 = 1999 * 3;
    println!("3 x 19.99: {}.{:02}", cents / 100, cents % 100);
}
```

```output for=scaled
1.5 * 2.25: 3.375
300 * 300 is an error: yes
300 * 300 saturated: 32767.99998474121
3 x 19.99: 59.97
```

- Verdict: idiom. The standard library has no fixed-point type, so the block keeps Q16.16 values in an `i32` and multiplies in an `i64`.
- `i32::try_from` returns an error when the product does not fit, and `clamp` saturates it to the largest Q16.16 value.
