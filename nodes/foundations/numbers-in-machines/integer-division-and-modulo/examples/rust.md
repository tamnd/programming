```rust run id=euclid
fn main() {
    let a: i32 = -7;
    println!("-7 / 3: {}", a / 3);
    println!("-7 % 3: {}", a % 3);
    println!("(-7).div_euclid(3): {}", a.div_euclid(3));
    println!("(-7).rem_euclid(3): {}", a.rem_euclid(3));
    println!("7.div_euclid(-3): {}", 7i32.div_euclid(-3));
    println!("7.rem_euclid(-3): {}", 7i32.rem_euclid(-3));
    println!("-7i32.rem_euclid(3): {}", -7i32.rem_euclid(3));
    println!("(-7).checked_div(0): {:?}", a.checked_div(0));
}
```

```output for=euclid
-7 / 3: -2
-7 % 3: -1
(-7).div_euclid(3): -3
(-7).rem_euclid(3): 2
7.div_euclid(-3): -2
7.rem_euclid(-3): 1
-7i32.rem_euclid(3): -1
(-7).checked_div(0): None
```

- Verdict: native. `/` and `%` truncate toward zero, and `div_euclid` and `rem_euclid` give the Euclidean results, where the remainder is never negative.
- `-7i32.rem_euclid(3)` is -1, because the method call binds before the unary minus.
- Division by zero panics, and `checked_div` returns `None` instead.
