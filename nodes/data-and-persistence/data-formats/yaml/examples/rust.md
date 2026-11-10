```rust run id=tree deps=deps/rust/Cargo.lock
use saphyr::{LoadableYamlNode, Scalar, Yaml, YamlEmitter};

fn describe(node: &Yaml) -> String {
    match node {
        Yaml::Value(Scalar::String(text)) => format!("string {text:?}"),
        Yaml::Value(Scalar::Integer(number)) => format!("int {number}"),
        Yaml::Value(Scalar::FloatingPoint(number)) => format!("float {number}"),
        Yaml::Value(Scalar::Boolean(flag)) => format!("bool {flag}"),
        Yaml::Value(Scalar::Null) => "null".to_string(),
        other => format!("{other:?}"),
    }
}

fn main() {
    let text = "country: NO\nenabled: on\nmode: 010\nmask: 0o10\nsize: 1_000\nport: 12:30\nnone: ~\nversion: 1.10\n";
    let docs = Yaml::load_from_str(text).expect("valid YAML");
    if let Yaml::Mapping(map) = &docs[0] {
        for (key, value) in map {
            println!("{} -> {}", key.as_str().unwrap_or("?"), describe(value));
        }
    }

    let mut out = String::new();
    YamlEmitter::new(&mut out).dump(&docs[0]).expect("emit");
    println!("{out}");

    for bad in ["a: 1\na: 2\n", "list:\n\t- x\n"] {
        match Yaml::load_from_str(bad) {
            Ok(docs) => println!("ok {docs:?}"),
            Err(error) => println!("error: {error}"),
        }
    }
}
```

```output for=tree
country -> string "NO"
enabled -> string "on"
mode -> int 10
mask -> int 8
size -> string "1_000"
port -> string "12:30"
none -> null
version -> float 1.1
---
country: "NO"
enabled: "on"
mode: 10
mask: 8
size: 1_000
port: "12:30"
none: ~
version: 1.1
error: duplicated key in mapping at byte 5 line 2 column 1
error: tabs disallowed within this context (block indentation) at byte 7 line 2 column 2
```

```rust run id=typed deps=deps/rust/Cargo.lock
use serde::de::IgnoredAny;
use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;

#[derive(Debug, Deserialize, Serialize)]
struct Service {
    name: String,
    version: String,
    country: String,
    replicas: u32,
    ports: Vec<u16>,
}

#[derive(Debug, Deserialize)]
#[allow(dead_code)]
struct Named {
    name: String,
}

fn first_line(error: serde_saphyr::Error) -> String {
    error.to_string().lines().next().unwrap_or("").to_string()
}

fn main() {
    let text = "name: web\nversion: 1.10\ncountry: NO\nreplicas: 3\nports: [80, 443]\n";
    let service: Service = serde_saphyr::from_str(text).expect("valid service");
    println!("{service:?}");
    print!("{}", serde_saphyr::to_string(&service).expect("serialize"));

    let repeated = serde_saphyr::from_str::<Named>("name: a\nname: b\n");
    println!("{:?}", repeated.map_err(first_line));

    let items = vec!["x"; 10].join(", ");
    let mut bomb = format!("a: &a [{items}]\n");
    for (prev, name) in ('a'..='i').zip('b'..='j') {
        let aliases = vec![format!("*{prev}"); 10].join(", ");
        bomb.push_str(&format!("{name}: &{name} [{aliases}]\n"));
    }
    println!("{} bytes of text", bomb.len());
    let tree = serde_saphyr::from_str::<BTreeMap<String, Vec<IgnoredAny>>>(&bomb);
    println!("{:?}", tree.map(|map| map.len()).map_err(first_line));
}
```

```output for=typed
Service { name: "web", version: "1.10", country: "NO", replicas: 3, ports: [80, 443] }
name: web
version: "1.10"
country: "NO"
replicas: 3
ports:
- 80
- 443
Err("error: line 2 column 1: duplicate mapping key: name, set DuplicateKeyPolicy in Options if acceptable")
460 bytes of text
Err("error: line 6 column 12: budget breached: Nodes { nodes: 250001 }")
```

- Verdict: library. The Rust standard library has no YAML. The examples use saphyr 0.3.3 and serde-saphyr 2.1.1 with serde 1.0.229. The crate serde_yaml is deprecated, and its last version is 0.9.34+deprecated.
- `Yaml::load_from_str` from saphyr returns a `Vec` of documents. Each node is a `Yaml` value, and a scalar is a `Scalar::String`, `Integer`, `FloatingPoint`, `Boolean` or `Null`.
- The results of saphyr match the 1.2 core schema. `NO` and `on` stay strings, `010` is 10, `0o10` is 8, and `1_000` and `12:30` are strings.
- `YamlEmitter` writes a `---` line and quotes `"NO"`, `"on"` and `"12:30"`. It writes `1_000` with no quotes, which a YAML 1.1 reader takes as an integer.
- saphyr rejects a repeated key and a tab in the indentation.
- `serde_saphyr::from_str` reads into a type that implements `Deserialize`. A `String` field takes `1.10` and `NO` as text, so the struct keeps the exact characters. `serde_saphyr::to_string` quotes them on output.
- serde-saphyr rejects repeated keys by default. The error names `DuplicateKeyPolicy` in its options.
- serde-saphyr has a budget for each parse. In the example, a document of 460 bytes would expand to more than ten billion nodes, and the parse stops at 250,001 nodes.
