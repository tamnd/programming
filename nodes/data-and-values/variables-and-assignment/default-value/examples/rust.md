```rust run id=default-trait
#[derive(Debug, Default)]
struct Options {
    width: u32,
    name: String,
    verbose: bool,
}

fn main() {
    let o = Options::default();
    println!("{o:?}");
    let p = Options {
        width: 80,
        ..Default::default()
    };
    println!("{} {:?} {}", p.width, p.name, p.verbose);
    let words: Vec<&str> = Vec::new();
    println!("{}", words.first().copied().unwrap_or("none"));
}
```

```output for=default-trait
Options { width: 0, name: "", verbose: false }
80 "" false
none
```

- Verdict: library. Rust gives no value to a variable without one. The trait `Default` gives a type a default value, and `#[derive(Default)]` uses the default of each field. `..Default::default()` fills the fields that a struct literal leaves out. `unwrap_or` gives a default for a missing value.
