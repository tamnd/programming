```rust run id=while-and-while-let
fn main() {
    // while with a Boolean condition.
    let mut n: u64 = 9_876_543_210;
    let mut digits = 0;
    while n > 0 {
        n /= 10;
        digits += 1;
    }
    println!("digits: {digits}");

    // while let loops while the pattern matches.
    let mut stack = vec!["a", "b", "c"];
    while let Some(top) = stack.pop() {
        println!("pop {top}");
    }

    // Two while let loops count the runs of equal values.
    let mut it = [1, 1, 2, 3, 3, 3].iter().peekable();
    while let Some(x) = it.next() {
        let mut run = 1;
        while it.peek() == Some(&x) {
            it.next();
            run += 1;
        }
        println!("{x} x{run}");
    }
}
```

```output for=while-and-while-let
digits: 10
pop c
pop b
pop a
1 x2
2 x1
3 x3
```

- Verdict: native. Rust has `while` with a `bool` condition and `while let`, which loops while a pattern matches. `stack.pop()` returns `None` when the vector is empty, and that ends the loop. The inner loop peeks at the next value and runs while it is equal to the current one.
