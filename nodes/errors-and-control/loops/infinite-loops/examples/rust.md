```rust run id=loop-without-break-and-checked-add
// No return after the loop: a loop without break has the type !.
fn first_over(limit: u64) -> u64 {
    let mut n = 1;
    loop {
        n *= 3;
        if n > limit {
            return n;
        }
    }
}

fn main() {
    println!("first power of 3 over 100: {}", first_over(100));

    // checked_add gives None at the limit, so the loop cannot wrap around.
    let mut x: u8 = 250;
    let mut passes = 0;
    while let Some(next) = x.checked_add(1) {
        x = next;
        passes += 1;
    }
    println!("u8 stops at {x} after {passes} passes");

    // loop gives a value with break.
    let mut tries = 0;
    let result = loop {
        tries += 1;
        if tries == 3 {
            break "connected";
        }
    };
    println!("{result} after {tries} tries");
}
```

```output for=loop-without-break-and-checked-add
first power of 3 over 100: 243
u8 stops at 255 after 5 passes
connected after 3 tries
```

- Verdict: native. `loop` is the infinite loop of Rust. In `first_over` the loop has no `break`, so it has the type `!` and the function needs no value after it. `checked_add` returns `None` at 255, which ends the `while let` loop where a wrapping add would start again at 0.
