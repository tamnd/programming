```rust run id=ports
use std::fmt;
use std::num::ParseIntError;

#[derive(Debug)]
enum ConfigError {
    NotANumber(ParseIntError),
    OutOfRange(u32),
}

impl fmt::Display for ConfigError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            ConfigError::NotANumber(e) => write!(f, "not a number: {e}"),
            ConfigError::OutOfRange(n) => write!(f, "out of range: {n}"),
        }
    }
}

fn parse_port(text: &str) -> Result<u16, ConfigError> {
    let port: u32 = text.parse().map_err(ConfigError::NotANumber)?;
    if !(1..=65535).contains(&port) {
        return Err(ConfigError::OutOfRange(port));
    }
    Ok(port as u16)
}

fn parse_all(texts: &[&str]) -> Result<Vec<u16>, ConfigError> {
    let mut ports = Vec::new();
    for text in texts {
        ports.push(parse_port(text)?);
    }
    Ok(ports)
}

fn main() {
    for text in ["8080", "70000", "abc"] {
        match parse_port(text) {
            Ok(port) => println!("port {port}"),
            Err(e) => println!("error: {e}"),
        }
    }
    println!("{:?}", parse_all(&["80", "443"]));
    println!("{:?}", parse_all(&["80", "x", "443"]).map_err(|e| e.to_string()));
}
```

```output
port 8080
error: out of range: 70000
error: not a number: invalid digit found in string
Ok([80, 443])
Err("not a number: invalid digit found in string")
```

- Verdict: idiom. Rust has no exceptions. A function that can fail returns `Result<T, E>`, and `?` returns the error to the caller.
- The type of each function shows that it can fail, and the compiler warns when a `Result` is not used.
- `panic!` is for bugs. By default it unwinds the stack and drops the values of each frame. With `panic = "abort"` in the Cargo profile, it ends the process at once.
- `std::panic::catch_unwind` can stop an unwinding panic, but its documentation says not to use it as a general `try` and `catch`.
