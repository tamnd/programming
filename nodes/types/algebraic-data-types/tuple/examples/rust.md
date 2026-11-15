```rust run id=pair
fn min_max(values: &[i32]) -> (i32, i32) {
    let lo = *values.iter().min().unwrap();
    let hi = *values.iter().max().unwrap();
    (lo, hi)
}

fn yn(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    let pair = min_max(&[7, 2, 9, 4]);
    println!("pair: {:?}", pair);
    let (lo, hi) = pair;
    println!("lo: {} hi: {}", lo, hi);
    println!("second part: {}", pair.1);
    println!("equal pairs: {}", yn(pair == (2, 9)));
    println!("unit: {:?}", ());
}
```

```output for=pair
pair: (2, 9)
lo: 2 hi: 9
second part: 9
equal pairs: yes
unit: ()
```

- Verdict: native. A tuple type such as `(i32, i32)` lists the type of each position.
- `pair.1` reads a part by position, and `let (lo, hi) = pair` unpacks it with a pattern.
- The empty tuple `()` is the unit value.
