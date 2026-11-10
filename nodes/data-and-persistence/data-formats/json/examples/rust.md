```rust run id=parse-emit deps=deps/rust/Cargo.lock
use serde::{Deserialize, Serialize};
use serde_json::Value;

#[derive(Debug, Serialize, Deserialize)]
struct Order {
    id: u64,
    total: f64,
    tags: Vec<String>,
    note: Option<String>,
}

fn main() -> Result<(), serde_json::Error> {
    let text = r#"{"id": 9007199254740993, "total": 0.10, "tags": ["new"], "note": null}"#;

    let order: Order = serde_json::from_str(text)?;
    println!("{order:?}");
    println!("{}", serde_json::to_string(&order)?);

    let tree: Value = serde_json::from_str(text)?;
    println!("{}", tree["id"]);
    println!("{}", tree["tags"][0]);
    Ok(())
}
```

```output for=parse-emit
Order { id: 9007199254740993, total: 0.1, tags: ["new"], note: None }
{"id":9007199254740993,"total":0.1,"tags":["new"],"note":null}
9007199254740993
"new"
```

```rust run id=round-trip deps=deps/rust/Cargo.lock
use serde::Deserialize;
use serde_json::Value;

#[derive(Debug, Deserialize)]
#[allow(dead_code)]
struct Invoice {
    total: u64,
}

fn main() {
    let repeated = r#"{"total": 1, "total": 2}"#;
    println!("{:?}", serde_json::from_str::<Invoice>(repeated));
    println!("{}", serde_json::from_str::<Value>(repeated).unwrap());

    let big: Value = serde_json::from_str("18446744073709551616").unwrap();
    println!("{big}");

    println!("{}", serde_json::to_string(&f64::NAN).unwrap());

    let tree: Value = serde_json::from_str(r#"{"zeta": 1, "alpha": 2}"#).unwrap();
    println!("{tree}");
}
```

```output for=round-trip
Err(Error("duplicate field `total`", line: 1, column: 20))
{"total":2}
1.8446744073709552e+19
null
{"alpha":2,"zeta":1}
```

- Verdict: library. The Rust standard library has no JSON. The examples use `serde_json` 1.0.154 with `serde` 1.0.229 and the `derive` feature.
- `serde_json::from_str`, `from_slice` and `from_reader` parse into a type that implements `Deserialize`. `to_string`, `to_string_pretty` and `to_writer` write a type that implements `Serialize`.
- `Value` keeps an integer as `u64` or `i64` when it fits, so `9007199254740993` survives. An integer above `u64::MAX` becomes an `f64`, unless the crate is built with the feature `arbitrary_precision`.
- A derived struct rejects a repeated field. A `Value` keeps the last one.
- `NaN` and the infinities serialize as `null`.
- `Value` keeps object members in a `BTreeMap`, sorted by name. The feature `preserve_order` keeps the order of the input.
