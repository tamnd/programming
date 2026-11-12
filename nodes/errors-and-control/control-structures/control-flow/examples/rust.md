```rust run id=control-expressions-and-question-mark
use std::num::ParseIntError;

// if, match and loop are expressions, so they give values.
fn sign(n: i32) -> &'static str {
    if n < 0 {
        "negative"
    } else if n == 0 {
        "zero"
    } else {
        "positive"
    }
}

// The ? operator returns early with the error.
fn sum(a: &str, b: &str) -> Result<i32, ParseIntError> {
    let x: i32 = a.parse()?;
    let y: i32 = b.parse()?;
    Ok(x + y)
}

fn main() {
    println!("{} {} {}", sign(-5), sign(0), sign(8));

    // break can give a value to the loop expression.
    let mut n = 1;
    let first = loop {
        n *= 3;
        if n > 50 {
            break n;
        }
    };
    println!("{first}");

    // match must cover every value.
    let size = match first {
        0..=9 => "one digit",
        10..=99 => "two digits",
        _ => "more",
    };
    println!("{size}");

    println!("{:?} {}", sum("2", "3"), sum("2", "x").is_err());

    // while let runs while a pattern matches.
    let mut stack = vec![1, 2, 3];
    while let Some(top) = stack.pop() {
        print!("{top} ");
    }
    println!();
}
```

```output for=control-expressions-and-question-mark
negative zero positive
81
two digits
Ok(5) true
3 2 1 
```

- Verdict: native. `if` and `match` are expressions, so `sign` has no `return`. `break n` gives a value to the `loop`, and `match` must cover every value. The `?` operator returns the parse error early. `while let` runs while `pop` returns `Some`.
