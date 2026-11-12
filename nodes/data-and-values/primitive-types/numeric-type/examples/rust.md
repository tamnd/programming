```rust run id=defaults-and-euclid
fn main() {
    // An integer literal is i32 and a float literal is f64, unless a suffix or a type says otherwise.
    let a = 7;
    let b = 7.0;
    let c = 7u8;
    println!("{} {} {} {}", a / 2, b / 2.0, c / 2, -7 / 2);

    // / and % truncate; div_euclid and rem_euclid give a remainder that is not negative.
    println!("{} {} {} {}", -7 % 2, (-7i32).div_euclid(2), (-7i32).rem_euclid(2), 7.5f64.div_euclid(2.0));

    // Numeric types do not mix: i32 + i64 does not compile, so convert.
    let x: i32 = 5;
    let y: i64 = 7;
    println!("{} {} {}", i64::from(x) + y, x as f64 / 2.0, u8::try_from(300).is_err());

    // Each type has its range, and the checked methods return None on overflow.
    println!("{} {} {:?}", i32::MAX, u64::MAX, i32::MAX.checked_add(1));

    // usize has the size of a pointer.
    println!("{} {} {}", size_of::<usize>(), size_of::<i128>(), size_of::<f32>());

    // Floating-point division by zero gives infinity, and NaN is not equal to itself.
    let zero = 0.0f64;
    println!("{} {} {}", 1.0 / zero, (zero / zero).is_nan(), 0.1 + 0.2);
}
```

```output for=defaults-and-euclid
3 3.5 3 -3
-1 -4 1 3
12 2.5 true
2147483647 18446744073709551615 None
8 16 4
inf true 0.30000000000000004
```

- Verdict: native. A literal with no suffix is `i32` or `f64`. `/` and `%` truncate, and `div_euclid` and `rem_euclid` round so that the remainder is not negative. `i32` and `i64` do not mix, so the program uses `i64::from`, and `u8::try_from(300)` fails. `checked_add` returns `None` at `i32::MAX`. `usize` has the size of a pointer. Floating-point division by zero gives `inf`.
