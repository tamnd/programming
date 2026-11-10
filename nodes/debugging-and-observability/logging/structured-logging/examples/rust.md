```rust run id=records
use std::fmt::Write;

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
enum Level {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
}

struct Logger {
    min: Level,
}

impl Logger {
    // Writes one line of key=value pairs, with no timestamp.
    // {:?} quotes each string and escapes newlines and quotes in it.
    fn log(&self, level: Level, msg: &str, fields: &[(&str, &dyn std::fmt::Debug)]) {
        if level < self.min {
            return;
        }
        let mut line = format!("level={level:?} msg={msg:?}");
        for (key, value) in fields {
            write!(line, " {key}={value:?}").unwrap();
        }
        println!("{line}");
    }
}

fn main() {
    let log = Logger { min: Level::Info };
    log.log(Level::Trace, "scan started", &[]); // below Info: dropped
    log.log(Level::Debug, "cart loaded", &[("items", &3)]); // dropped
    log.log(Level::Info, "order placed", &[("order", &1042), ("total_cents", &1999)]);
    log.log(Level::Warn, "stock low", &[("sku", &"A-17"), ("left", &2)]);
    let user = "bob\nlevel=Info msg=\"login ok\"";
    log.log(Level::Info, "login failed", &[("user", &user)]);
    match "12x".parse::<u32>() {
        Ok(qty) => log.log(Level::Info, "quantity", &[("qty", &qty)]),
        Err(e) => log.log(Level::Error, "bad quantity", &[("input", &"12x"), ("error", &e.to_string())]),
    }
}
```

```output for=records
level=Info msg="order placed" order=1042 total_cents=1999
level=Warn msg="stock low" sku="A-17" left=2
level=Info msg="login failed" user="bob\nlevel=Info msg=\"login ok\""
level=Error msg="bad quantity" input="12x" error="invalid digit found in string"
```

- Verdict: library. The `log` crate is the common logging facade. Without a logger implementation, its macros do nothing. The standard library has no logger.
- The `{:?}` format quotes a string and escapes a newline and a quote in it, so a field stays on one line.
- `#[derive(PartialOrd)]` orders the variants of `Level` by their position, so `level < self.min` filters the records.
