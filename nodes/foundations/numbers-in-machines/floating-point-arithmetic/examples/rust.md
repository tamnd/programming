```rust run id=total-cmp
fn main() {
    let x = 0.1_f64 + 0.2;
    println!("0.1 + 0.2 = {x}");
    println!("== 0.3: {}", x == 0.3);
    println!("EPSILON = {:e}", f64::EPSILON);
    println!("bits of 0.1 = {:#018x}", 0.1_f64.to_bits());

    let mut v = vec![2.5, f64::NAN, -0.0, 0.0, -1.0, f64::INFINITY];
    v.sort_by(|a, b| a.total_cmp(b));
    println!("sorted with total_cmp: {v:?}");

    println!("NaN == NaN: {}", f64::NAN == f64::NAN);
    println!("NaN.partial_cmp(1.0): {:?}", f64::NAN.partial_cmp(&1.0));
}
```

```output
0.1 + 0.2 = 0.30000000000000004
== 0.3: false
EPSILON = 2.220446049250313e-16
bits of 0.1 = 0x3fb999999999999a
sorted with total_cmp: [-1.0, -0.0, 0.0, 2.5, inf, NaN]
NaN == NaN: false
NaN.partial_cmp(1.0): None
```

- Verdict: native. `f64` is IEEE 754 binary64 and `f32` is binary32.
- `f64` implements `PartialEq` and `PartialOrd` but not `Eq` and `Ord`, because `NaN` is not equal to itself and has no order with other values. `partial_cmp` returns `None` for it.
- `total_cmp` gives the total order of IEEE 754: negative `NaN` first, then the numbers with `-0.0` before `0.0`, then positive `NaN`.

`sort` needs `Ord`, so the compiler rejects a sort of a vector of `f64`.

```rust fails expect=compile id=sort
fn main() {
    let mut v = vec![2.5, 1.0, 3.0];
    v.sort();
    println!("{v:?}");
}
```

```output
```
