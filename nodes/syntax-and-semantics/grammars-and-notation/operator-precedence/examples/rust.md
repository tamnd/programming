```rust run id=method-calls-and-casts
fn main() {
    // A method call binds more tightly than unary minus, so this is -(2.pow(2)).
    println!("{} {}", -2i32.pow(2), (-2i32).pow(2));

    // As in Go, == binds less tightly than &, so this is (x & 1) == 0.
    let x = 6;
    println!("{}", x & 1 == 0);

    // + binds more tightly than <<, as in C.
    println!("{}", 1 << 2 + 1);

    // `as` binds more tightly than the binary operators.
    let big = 300i32;
    println!("{} {}", big as u8 / 2, (big / 2) as u8);

    // Comparisons do not associate: a < b < c is a compile error, so the program writes both tests.
    let (a, b, c) = (1, 2, 3);
    println!("{}", a < b && b < c);

    // The ? operator binds more tightly than unary minus and the binary operators.
    fn parse_neg(s: &str) -> Result<i32, std::num::ParseIntError> {
        Ok(-s.parse::<i32>()? * 2)
    }
    println!("{:?} {}", parse_neg("21"), parse_neg("x").is_err());
}
```

```output for=method-calls-and-casts
-4 4
true
8
22 150
true
Ok(-42) true
```

- Verdict: native. A method call binds more tightly than unary minus, so `-2i32.pow(2)` is −4. As in Go, `==` binds less tightly than `&`. `+` binds more tightly than `<<`, as in C. `as` binds more tightly than `/`, so `big as u8 / 2` truncates first. Comparisons do not associate, so `a < b < c` is a compile error. The `?` operator binds more tightly than unary minus and `*`.
