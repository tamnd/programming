```rust run id=scopes
static LEVEL: &str = "crate";

fn reads_static() -> &'static str {
    LEVEL
}

fn caller() -> String {
    let level = "caller";
    format!("{}, local {}", reads_static(), level)
}

fn main() {
    println!("{}", caller());

    let input = " 42 ";
    let input = input.trim();
    let input: i32 = input.parse().unwrap();
    println!("input: {}", input + 1);

    let x = 1;
    {
        let x = x * 10;
        println!("x in the block: {x}");
    }
    println!("x after the block: {x}");
}
```

```output for=scopes
crate, local caller
input: 43
x in the block: 10
x after the block: 1
```

- Verdict: native. A `let` binding is in scope from just after the `let` statement to the end of its block.
- A new `let` can shadow an earlier binding with the same name, also in the same block. This is common when a value changes type, as `input` does here. The old binding still exists, but no name refers to it.
