```rust run id=money
fn main() {
    let mut total = 0.0_f64;
    for _ in 0..10 {
        total += 0.1;
    }
    println!("loop: {}", total);
    println!("0.1 exact: {:.55}", 0.1_f64);
    println!("0.1 + 0.2 == 0.3: {}", if 0.1 + 0.2 == 0.3 { "yes" } else { "no" });

    let mut sum = 0.0_f64;
    let mut c = 0.0_f64;
    for _ in 0..10 {
        let y = 0.1 - c;
        let t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    println!("Kahan: {}", sum);

    let cents: i64 = (0..10).map(|_| 10).sum();
    println!("cents: {} -> {}.{:02}", cents, cents / 100, cents % 100);
}
```

```output for=money
loop: 0.9999999999999999
0.1 exact: 0.1000000000000000055511151231257827021181583404541015625
0.1 + 0.2 == 0.3: no
Kahan: 1
cents: 100 -> 1.00
```

- Verdict: native. Rust `f64` is binary64.
- `{:.55}` prints 55 digits after the point, which is the exact value of the stored 0.1.
- The program keeps money as integer cents, which add without error.
