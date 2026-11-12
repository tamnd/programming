```rust run id=language-features
use std::sync::OnceLock;

#[derive(Debug, Clone, Default)]
struct Request {
    url: String,
    method: String,
    retries: u32,
}

impl Request {
    // By convention, `new` is the constructor; it is an ordinary function.
    fn new(url: &str) -> Self {
        Request { url: url.to_string(), method: "GET".to_string(), ..Default::default() }
    }
}

fn config() -> &'static str {
    static CONFIG: OnceLock<String> = OnceLock::new();
    CONFIG.get_or_init(|| {
        println!("loading config");
        "prod".to_string()
    })
}

fn main() {
    let r = Request::new("https://example.com/");
    println!("new: {r:?}");

    // Struct update syntax copies the other fields from another value.
    let r2 = Request { retries: 2, ..r.clone() };
    println!("update: {r2:?}");

    println!("default: {:?}", Request::default());

    // Clone is the prototype operation.
    let mut c = r2.clone();
    c.method = "POST".to_string();
    println!("clone: {} {}", r2.method, c.method);

    println!("singleton: {} {}", config(), std::ptr::eq(config(), config()));
}
```

```output
new: Request { url: "https://example.com/", method: "GET", retries: 0 }
update: Request { url: "https://example.com/", method: "GET", retries: 2 }
default: Request { url: "", method: "", retries: 0 }
clone: GET POST
loading config
singleton: prod true
```

- Verdict: idiom. Rust has no constructors in the language. `new` is a convention for an associated function, `Default` gives default values, and struct update syntax fills the other fields from another value. `Clone` is the prototype operation. A builder type is common for values with many options, and `OnceLock` in a `static` is a lazy singleton.
