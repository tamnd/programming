```rust run id=if-expression-and-let-chains
fn parse_port(s: &str) -> Option<u16> {
    // let else leaves the function when the pattern does not match.
    let Ok(port) = s.parse::<u16>() else {
        return None;
    };
    if port == 0 { None } else { Some(port) }
}

fn main() {
    // if is an expression, so both branches must have the same type.
    let n = 7;
    let parity = if n % 2 == 0 { "even" } else { "odd" };
    println!("{n} is {parity}");

    // if let runs a block when a pattern matches.
    let config: Option<&str> = Some("8080");
    if let Some(text) = config {
        println!("config {text}");
    }

    // A let chain joins patterns and conditions with &&.
    for s in ["8080", "0", "abc"] {
        if let Some(p) = parse_port(s)
            && p >= 1024
        {
            println!("{s}: user port {p}");
        } else {
            println!("{s}: rejected");
        }
    }

    // The condition must be a bool; an integer does not work.
    let count = 3;
    println!("{}", if count != 0 { "non-zero" } else { "zero" });
}
```

```output for=if-expression-and-let-chains
7 is odd
config 8080
8080: user port 8080
0: rejected
abc: rejected
non-zero
```

- Verdict: native. `if` is an expression, so it gives `parity` its value. `if let` runs a block when `Some` matches. `let else` returns `None` from `parse_port` when the parse fails. A let chain joins the pattern and the test `p >= 1024`, so `"0"` and `"abc"` are rejected.
