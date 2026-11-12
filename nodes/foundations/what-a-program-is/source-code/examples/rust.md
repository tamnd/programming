```rust run id=stringify-and-line
macro_rules! check {
    ($e:expr) => {
        // stringify! turns the argument back into text.
        println!("line {}: {} is {}", line!(), stringify!($e), $e)
    };
}

fn main() {
    let x = -2;
    check!(x > 0);
    check!(x*x==4);
    println!("column of this call: {}", column!());
}
```

```output for=stringify-and-line
line 10: x > 0 is false
line 11: x*x==4 is true
column of this call: 41
```

- Verdict: native. The macro gets the expression as tokens, and `stringify!` turns them back into a string when the program compiles. `line!` and `column!` give the position of the macro call, so the program prints the source text and the line of each check with no cost at run time. The documentation of `stringify!` warns that the exact text of the result may change in future compilers.
