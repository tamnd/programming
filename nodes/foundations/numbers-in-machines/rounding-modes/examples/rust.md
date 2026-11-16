```rust run id=ties
fn main() {
    println!("    x round  even floor  ceil trunc");
    for x in [0.5_f64, 1.5, 2.5, -0.5, -1.5, -2.5] {
        let row = [x.round(), x.round_ties_even(), x.floor(), x.ceil(), x.trunc()];
        let cells: Vec<String> = row.iter().map(|r| format!("{:6}", r + 0.0)).collect();
        println!("{x:5}{}", cells.join(""));
    }

    println!("format {{:.0}}: {:.0} {:.0} {:.0}", 0.5, 1.5, 2.5);
    println!("format {{:.2}} of 2.675: {:.2}", 2.675);
    println!("-2.7 as i32: {}", -2.7_f64 as i32);
    println!("-7 / 2: {}, (-7_i32).div_euclid(2): {}", -7 / 2, (-7_i32).div_euclid(2));
}
```

```output for=ties
    x round  even floor  ceil trunc
  0.5     1     0     0     1     0
  1.5     2     2     1     2     1
  2.5     3     2     2     3     2
 -0.5    -1     0    -1     0     0
 -1.5    -2    -2    -2    -1    -1
 -2.5    -3    -2    -3    -2    -2
format {:.0}: 0 2 2
format {:.2} of 2.675: 2.67
-2.7 as i32: -2
-7 / 2: -3, (-7_i32).div_euclid(2): -4
```

- Verdict: native. `round` rounds half away from zero, and `round_ties_even` rounds ties to even.
- `as` casts from float to integer truncate toward zero. Integer `/` also truncates, and `div_euclid` rounds toward minus infinity for a positive divisor.
- The `{:.0}` format rounds ties to even, so it does not agree with `round`.
