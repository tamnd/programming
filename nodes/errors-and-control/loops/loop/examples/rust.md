```rust run id=loop-value-while-let-and-for
fn main() {
    // loop is an expression: break gives its value.
    let mut n = 1;
    let first_over_100 = loop {
        n *= 3;
        if n > 100 {
            break n;
        }
    };
    println!("loop: {first_over_100}");

    // while let loops while a pattern matches.
    let mut stack = vec![1, 2, 3];
    let mut popped = Vec::new();
    while let Some(top) = stack.pop() {
        popped.push(top);
    }
    println!("while let: {popped:?}");

    // for takes the values of an iterator, here a range.
    let mut sum = 0;
    for i in 1..=5 {
        sum += i;
    }
    println!("for: {sum}");

    // An iterator chain replaces the loop.
    let squares: Vec<i32> = (1..=4).map(|x| x * x).collect();
    println!("{squares:?}");
}
```

```output for=loop-value-while-let-and-for
loop: 243
while let: [3, 2, 1]
for: 15
[1, 4, 9, 16]
```

- Verdict: native. Rust has `loop`, `while` and `for`. The `loop` gives 243, the value of its `break`, and it is the first power of 3 over 100. `while let` pops the stack until `pop` returns `None`. `for` takes the values of the range `1..=5`, and `map` with `collect` builds a vector with no loop statement.
