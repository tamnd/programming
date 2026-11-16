```rust run id=tiny
fn yes(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    let min_normal = f64::MIN_POSITIVE;
    let smallest = f64::from_bits(1);
    println!("MIN_POSITIVE: {min_normal:e}");
    println!("from_bits(1): {smallest:e}");
    println!("bits of MIN_POSITIVE: {:#018x}", min_normal.to_bits());

    let half = min_normal / 2.0;
    println!("is_subnormal(MIN_POSITIVE): {}", yes(min_normal.is_subnormal()));
    println!("is_subnormal(MIN_POSITIVE / 2): {}", yes(half.is_subnormal()));
    println!("is_subnormal(0.0): {}", yes(0.0f64.is_subnormal()));
    println!("classify(MIN_POSITIVE / 2): {:?}", half.classify());

    let x = 1.25 * min_normal;
    println!("x - MIN_POSITIVE: {:e}", x - min_normal);
    println!("from_bits(1) / 2: {:e}", smallest / 2.0);
}
```

```output for=tiny
MIN_POSITIVE: 2.2250738585072014e-308
from_bits(1): 5e-324
bits of MIN_POSITIVE: 0x0010000000000000
is_subnormal(MIN_POSITIVE): no
is_subnormal(MIN_POSITIVE / 2): yes
is_subnormal(0.0): no
classify(MIN_POSITIVE / 2): Subnormal
x - MIN_POSITIVE: 5.562684646268003e-309
from_bits(1) / 2: 0e0
```

- Verdict: native. Rust `f64` follows IEEE 754, with `is_subnormal` and `classify`.
- `f64::MIN_POSITIVE` is the smallest positive normal value. Rust has no named constant for the smallest subnormal, so the code builds it with `f64::from_bits(1)`.
- `classify` returns `FpCategory::Subnormal` for a subnormal and `FpCategory::Zero` for zero.
