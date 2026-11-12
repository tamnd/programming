```rust run id=parse-emit deps=deps/rust/Cargo.lock
use serde::{Deserialize, Serialize};
use toml::value::Datetime;

#[derive(Debug, Serialize, Deserialize)]
struct Config {
    name: String,
    port: u16,
    started: Datetime,
    hosts: Vec<String>,
    database: Database,
}

#[derive(Debug, Serialize, Deserialize)]
struct Database {
    url: String,
    pool_size: u32,
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let text = r#"
name = "billing"
port = 8080
started = 2026-11-10T09:30:00+07:00
hosts = ["alpha", "beta"]

[database]
url = 'postgres://db.local/billing'
pool_size = 10
"#;

    let config: Config = toml::from_str(text)?;
    println!("{} {} {} {:?}", config.name, config.port, config.started, config.hosts);
    print!("{}", toml::to_string(&config)?);
    println!("---");

    let table: toml::Table = text.parse()?;
    println!("{}", table["database"]["pool_size"]);
    println!("{}", table["started"]);

    match toml::from_str::<Config>(&text.replace("8080", "70000")) {
        Ok(_) => println!("accepted"),
        Err(error) => print!("{error}"),
    }
    println!("---");
    match "limit = 9223372036854775808".parse::<toml::Table>() {
        Ok(table) => println!("{table:?}"),
        Err(error) => print!("{error}"),
    }
    Ok(())
}
```

```output
billing 8080 2026-11-10T09:30:00+07:00 ["alpha", "beta"]
name = "billing"
port = 8080
started = 2026-11-10T09:30:00+07:00
hosts = ["alpha", "beta"]

[database]
url = "postgres://db.local/billing"
pool_size = 10
---
10
2026-11-10T09:30:00+07:00
TOML parse error at line 3, column 8
  |
3 | port = 70000
  |        ^^^^^
invalid value: integer `70000`, expected u16
---
TOML parse error at line 1, column 9
  |
1 | limit = 9223372036854775808
  |         ^^^^^^^^^^^^^^^^^^^
u64 value was too large
```

```rust run id=edit deps=deps/rust/Cargo.lock
use toml_edit::{DocumentMut, value};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let text = r#"# Billing service
name = "billing"
port = 8080 # the public port

[database]
url = 'postgres://db.local/billing'
"#;

    let mut document: DocumentMut = text.parse()?;
    document["name"] = value("invoices");
    document["port"] = value(9090);
    document["database"]["pool_size"] = value(10);
    print!("{document}");
    println!("---");

    let table: toml::Table = text.parse()?;
    print!("{}", toml::to_string(&table)?);
    println!("---");

    let newer = "point = {\n  x = 1,\n  y = 2,\n}\nalarm = 07:30\n";
    let table: toml::Table = newer.parse()?;
    println!("{} {}", table["point"], table["alarm"]);
    Ok(())
}
```

```output
# Billing service
name = "invoices"
port = 9090

[database]
url = 'postgres://db.local/billing'
pool_size = 10
---
name = "billing"
port = 8080

[database]
url = "postgres://db.local/billing"
---
{ x = 1, y = 2 } 07:30
```

- Verdict: library. The Rust standard library has no TOML. The examples use the crates `toml` 1.2.1 and `toml_edit` 0.25.19, with `serde` 1.0.229 and the `derive` feature. The documentation of `toml` says that Cargo uses it.
- `toml::from_str` maps a document to a type that implements `Deserialize`, and `toml::to_string` writes a type that implements `Serialize`. `toml::Table` is a map of `toml::Value`, with no fixed type.
- A TOML datetime becomes `toml::value::Datetime`, which prints in the TOML form with its offset.
- A value that does not fit the field type is an error with the line and the column, so `port = 70000` fails for a `u16`. `toml::Value` keeps integers as `i64`, so `2^63` is an error, as TOML 1.0.0 requires.
- `toml_edit::DocumentMut` keeps comments, spaces and the order of keys. But `document["port"] = value(9090)` drops the comment at the end of that line, because the replacement value has no comment.
- `toml::to_string` of a parsed `Table` drops every comment, and it writes the literal string as a basic string.
- The crate version names the TOML version, as in `1.2.1+spec-1.1.0`. It parses the TOML 1.1.0 forms: an inline table over several lines and a time without seconds.
