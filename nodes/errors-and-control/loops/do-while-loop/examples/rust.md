```rust run id=loop-and-while-with-a-block-condition
fn main() {
    // loop with the test at the end of the body.
    let mut n: u32 = 0;
    let mut digits = 0;
    loop {
        n /= 10;
        digits += 1;
        if n == 0 {
            break;
        }
    }
    println!("digits of 0: {digits}");

    // The body goes into the condition block of a while loop with an empty body.
    let mut x = 10;
    while {
        println!("x = {x}");
        x += 1;
        x < 5
    } {}

    // loop gives a value with break.
    let answers = ["no", "no", "yes"];
    let mut tries = 0;
    let answer = loop {
        let a = answers[tries];
        tries += 1;
        if a == "yes" {
            break a;
        }
    };
    println!("{answer} after {tries} tries");
}
```

```output for=loop-and-while-with-a-block-condition
digits of 0: 1
x = 10
yes after 3 tries
```

- Verdict: idiom. Rust has no do-while loop. `loop` with `if ... { break; }` at the end is the usual form, and `break a` gives the loop a value. The condition of `while` is an expression, so a block can hold the body and end with the test, but this form is hard to read.
