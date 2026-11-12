```rust run id=redacting-logger
use std::fmt::Display;

/// Marker: the value must not be written to a log.
trait Sensitive {}

struct Username(String);
struct Password(String);
struct ApiKey(String);

impl Sensitive for Password {}
impl Sensitive for ApiKey {}

impl Display for Username {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

fn log<T: Display>(label: &str, item: &T) {
    println!("{label}: {item}");
}

// Only a marked type can be passed here; the compiler checks the bound.
fn log_secret<T: Sensitive>(label: &str, _item: &T) {
    println!("{label}: [redacted]");
}

fn main() {
    log("user", &Username("ann".to_string()));
    log_secret("password", &Password("hunter2".to_string()));
    log_secret("api key", &ApiKey("k-123".to_string()));
}
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. A trait with no items is the marker, and a trait bound tests it at compile time. `log_secret` with a `Username` does not compile.
- The standard library uses the same form in `std::marker`, for `Copy`, `Send`, `Sized` and `Sync`. `Password` and `ApiKey` do not implement `Display`, so they cannot reach the normal `log` by mistake.
