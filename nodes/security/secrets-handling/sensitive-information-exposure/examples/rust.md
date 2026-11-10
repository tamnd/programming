```rust wrong id=derive-debug
#[derive(Debug)]
#[allow(dead_code)] // the fields are only printed in this demo
struct OAuthClient {
    client_id: String,
    client_secret: String,
}

fn main() {
    // Placeholders. A real program reads these from a secret store.
    let client = OAuthClient {
        client_id: "demo-client".to_string(),
        client_secret: "PLACEHOLDER-SECRET".to_string(),
    };
    // A debug log line, as many programs write while they make a request.
    println!("debug: sending token request with {:?}", client);
}
```

```output for=derive-debug
debug: sending token request with OAuthClient { client_id: "demo-client", client_secret: "PLACEHOLDER-SECRET" }
```

A derived `Debug` prints the name and the `Debug` value of every field, so the secret goes into the debug line. This is the case of CVE-2022-31162.

```rust run id=manual-debug
use std::fmt;

/// Holds a secret. Debug and Display never show the value.
struct Secret(String);

impl Secret {
    fn expose(&self) -> &str {
        &self.0
    }
}

impl fmt::Debug for Secret {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str("Secret(***)")
    }
}

#[derive(Debug)]
#[allow(dead_code)] // the fields are only printed in this demo
struct OAuthClient {
    client_id: String,
    client_secret: Secret,
}

fn main() {
    // Placeholders. A real program reads these from a secret store.
    let client = OAuthClient {
        client_id: "demo-client".to_string(),
        client_secret: Secret("PLACEHOLDER-SECRET".to_string()),
    };
    println!("debug: sending token request with {:?}", client);
    // The code that needs the value asks for it by name.
    println!("secret length: {}", client.client_secret.expose().len());
}
```

```output for=manual-debug
debug: sending token request with OAuthClient { client_id: "demo-client", client_secret: Secret(***) }
secret length: 18
```

`Secret` has a hand-written `Debug` that never shows the value. `OAuthClient` can keep its derived `Debug`, because the derived code calls the `Debug` of each field.

- Verdict: stdlib. `std::fmt::Debug` and a newtype are enough. Crates such as `secrecy` give the same pattern with more features, such as clearing the memory on drop.
- Do not implement `Display` for a secret type. Without it, `format!("{}", secret)` does not compile, so a developer cannot print the value by accident.
- The Rust documentation says that derived `Debug` formats are not stable. Do not parse them, and do not rely on them to hide anything.
