```rust run id=propagate-rust
fn parse_port(text: &str) -> Result<u16, String> {
    text.parse::<u16>().map_err(|_| format!("not a number: {text}"))
}

fn load_config(line: &str) -> Result<u16, String> {
    let value = line.split_once('=').map_or("", |(_, v)| v);
    // map_err adds context, and ? returns the error to the caller.
    let port = parse_port(value).map_err(|e| format!("config line '{line}': {e}"))?;
    Ok(port)
}

fn main() {
    for line in ["port=8080", "port=x"] {
        match load_config(line) {
            Ok(port) => println!("port {port}"),
            Err(e) => println!("error: {e}"),
        }
    }
}
```

```output for=propagate-rust
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. The operator `?` returns the error early from the function, so the caller gets it. The type `Result` shows in each signature that the function can fail.
