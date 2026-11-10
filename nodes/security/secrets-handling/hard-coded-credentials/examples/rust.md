```rust run id=from-environment
use std::collections::HashMap;
use std::env;
use std::fmt;

struct Config {
    user: String,
    password: String,
}

// A hand-written Debug keeps the password out of {:?} and of panic messages.
impl fmt::Debug for Config {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Config {{ user: {:?}, password: <set> }}", self.user)
    }
}

// lookup is |name| env::var(name).ok() in production.
fn load_config(lookup: impl Fn(&str) -> Option<String>) -> Result<Config, String> {
    let password = lookup("EXAMPLE_DB_PASSWORD")
        .filter(|p| !p.trim().is_empty())
        .ok_or("EXAMPLE_DB_PASSWORD is not set")?;
    let user = lookup("EXAMPLE_DB_USER").unwrap_or_else(|| "app".to_string());
    Ok(Config { user, password })
}

fn main() {
    if let Err(e) = load_config(|name| env::var(name).ok()) {
        println!("refused to start: {e}");
    }

    let deployment = HashMap::from([("EXAMPLE_DB_PASSWORD", "example-not-a-real-password")]);
    let config = load_config(|name| deployment.get(name).map(|v| v.to_string())).unwrap();
    println!("{config:?}");
    // The program gives config.password to the database driver here.
    let _password = &config.password;
}
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
Config { user: "app", password: <set> }
```

`load_config` takes a closure. The program passes `std::env::var` to it, and a test passes a map. The hand-written `Debug` hides the password.

- Verdict: stdlib. `std::env::var` returns an error when the variable is not set or is not valid Unicode. The task [[read-an-environment-variable]] has the details.
- `#[derive(Debug)]` prints every field. `{:?}`, `dbg!` and many error messages use `Debug`. Write `Debug` by hand for a type that holds a secret.
- The macros `env!` and `option_env!` read a variable at compile time and put its value in the binary. Use `std::env::var` at run time for secrets.
- A string literal goes into the binary, and the `strings` tool shows it.
