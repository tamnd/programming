```rust run id=port
use std::collections::HashMap;
use std::fmt;
use std::num::ParseIntError;

#[derive(Debug)]
enum ConfigError {
    Missing(String),
    NotANumber(ParseIntError),
    OutOfRange(u32),
}

impl fmt::Display for ConfigError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            ConfigError::Missing(key) => write!(f, "missing key {key:?}"),
            ConfigError::NotANumber(e) => write!(f, "not a number: {e}"),
            ConfigError::OutOfRange(n) => write!(f, "port {n} is out of range"),
        }
    }
}

impl From<ParseIntError> for ConfigError {
    fn from(e: ParseIntError) -> Self {
        ConfigError::NotANumber(e)
    }
}

fn lookup<'a>(config: &HashMap<&str, &'a str>, key: &str) -> Result<&'a str, ConfigError> {
    config
        .get(key)
        .copied()
        .ok_or_else(|| ConfigError::Missing(key.to_string()))
}

fn port_setting(config: &HashMap<&str, &str>) -> Result<u16, ConfigError> {
    let text = lookup(config, "port")?;
    let n: u32 = text.parse()?;
    if n == 0 || n > 65535 {
        return Err(ConfigError::OutOfRange(n));
    }
    Ok(n as u16)
}

fn main() {
    let configs = [
        HashMap::from([("port", "8080")]),
        HashMap::from([("port", "x")]),
        HashMap::from([("port", "70000")]),
        HashMap::new(),
    ];
    for config in &configs {
        match port_setting(config) {
            Ok(port) => println!("port {port}"),
            Err(ConfigError::Missing(key)) => println!("no {key}, use the default 80"),
            Err(e) => println!("error: {e}"),
        }
    }
}
```

```output
port 8080
error: not a number: invalid digit found in string
error: port 70000 is out of range
no port, use the default 80
```

- Verdict: stdlib. `Result<T, E>` is an enum of the standard library with the variants `Ok(T)` and `Err(E)`. The operator `?` is part of the language.
- `?` returns early with the error. It converts the error with `From::from`, so the `ParseIntError` of `parse` becomes a `ConfigError` through the `From` implementation.
- The `match` in `main` must cover every variant. A new variant of `ConfigError` makes the compiler point at each `match` that does not handle it.
- `Result` is marked `#[must_use]`, so the compiler warns when a caller drops a `Result` without looking at it.

`unwrap` turns an error value into a panic. It is for prototypes, tests, and cases where the program knows that the value is `Ok`. Here the text is not a number, so the program stops with a panic.

```rust fails expect=panic id=unwrap
fn main() {
    let port: u16 = "x".parse().unwrap();
    println!("port {port}");
}
```
