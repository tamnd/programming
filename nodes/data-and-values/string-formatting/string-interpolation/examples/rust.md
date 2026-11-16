```rust run id=format-captures
fn main() {
    let name = "Ada";
    let price = 4.5_f64;
    let items = ["tea", "milk"];

    let total = price * 2.0;
    let n = items.len();
    println!("{name} pays {total:.2} for {n} items.");
    println!("{} pays {:.2} for {} items.", name, price * 2.0, items.len());
    println!("[{name:>6}] [{name:<6}]");
    println!("{{name}} is text, {{{name}}} is not.");
    let s = format!("{name}-{n}");
    println!("{s}");
}
```

```output for=format-captures
Ada pays 9.00 for 2 items.
Ada pays 9.00 for 2 items.
[   Ada] [Ada   ]
{name} is text, {Ada} is not.
Ada-2
```

- Verdict: native. A name in braces in the format string of `println!` or `format!` reads the variable with that name, and a format can follow after `:`. A placeholder holds only a name, not an expression, so the program puts `price * 2.0` in a variable first or passes it as an argument. The macro checks the format string at compile time, and two braces give one brace.
