```rust run id=raw-identifiers-and-nfc
// A raw identifier r#name lets a keyword be a name, for example to call code
// from an older edition in which the word was not a keyword.
fn r#match(r#type: &str) -> usize {
    r#type.len()
}

fn main() {
    println!("{}", r#match("abc"));

    // Rust names can use letters of other scripts. rustc warns when all the names
    // of a script could be confused with letters of another script, as with π here.
    let größe = 2;
    let π = 3.0_f64;
    println!("{}", π * größe as f64);

    // The compiler normalizes names to NFC. The second name below is spelled
    // with e and the combining accent U+0301, and it is the same variable.
    let café = 10;
    println!("{}", café + 1);

    // _ is a pattern that binds nothing; a name that starts with _ turns off the unused warning.
    let _unused = 5;
    let _ = r#match("");
}
```

```output for=raw-identifiers-and-nfc
3
6
11
```

- Verdict: native. A raw identifier such as `r#match` uses a keyword as a name. Rust names can use letters of other scripts, and rustc warns when the names of a script could be confused with letters of another script, as for `π`. The compiler normalizes names to NFC, so the second spelling of `café` names the same variable. A name that starts with `_` turns off the warning for an unused variable.
