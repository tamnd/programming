```rust run id=traps
use std::collections::HashSet;

fn main() {
    let nan = f64::NAN;
    println!("nan == nan: {}, is_nan: {}", nan == nan, nan.is_nan());
    println!("1/0: {}, -1/0: {}", 1.0_f64 / 0.0, -1.0_f64 / 0.0);
    println!("-0.0 == 0.0: {}, is_sign_negative: {}", -0.0_f64 == 0.0, (-0.0_f64).is_sign_negative());

    let mut values = vec![3.0, nan, 1.0, -0.0, 0.0];
    values.sort_by(|a, b| a.total_cmp(b));
    println!("total_cmp sort: {:?}", values);
    println!("max(1, NaN): {}", 1.0_f64.max(nan));

    let zero: f64 = "0".parse().unwrap();
    let computed = zero / zero;
    let bits: HashSet<u64> = [nan, computed].iter().map(|x| x.to_bits()).collect();
    println!("f64::NAN bits: {:016x}, 0/0 bits: {:016x}", nan.to_bits(), computed.to_bits());
    println!("distinct NaN bit patterns: {}", bits.len());
    let n: i32 = "0".parse().unwrap();
    println!("checked 1 / 0: {:?}", 1_i32.checked_div(n));
}
```

```output for=traps
nan == nan: false, is_nan: true
1/0: inf, -1/0: -inf
-0.0 == 0.0: true, is_sign_negative: true
total_cmp sort: [-0.0, 0.0, 1.0, 3.0, NaN]
max(1, NaN): 1
f64::NAN bits: 7ff8000000000000, 0/0 bits: fff8000000000000
distinct NaN bit patterns: 2
checked 1 / 0: None
```

- Verdict: native. Rust `f64` has `NAN`, `INFINITY` and `-0.0`, and `PartialOrd` but not `Ord`.
- `Vec<f64>` has no `sort`, because `f64` is not `Ord`. `sort_by` with `total_cmp` gives the IEEE total order.
- `f64::max` returns the other argument when one argument is a NaN.
- `f64` is not `Hash`, so the set holds the bit patterns from `to_bits`. The NaN that the processor computes can have a different sign bit from the constant.
