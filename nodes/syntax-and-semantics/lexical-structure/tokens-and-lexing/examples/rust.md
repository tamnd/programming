```rust run id=raw-strings-and-raw-names
fn main() {
    let big = 1_000_000u64; // a suffix fixes the type of a literal
    println!("{big} {} {} {} {}", 0b1010, 0o17, 0xff, b'A');
    let raw = r#"He said "hi" to C:\temp"#; // a raw string with # delimiters
    println!("{raw}");
    let r#match = 3; // r# turns a keyword into a name
    println!("{}", r#match * 2);
    let s = "é";
    println!("{} {}", s.len(), s.chars().count());
    let v: Vec<Vec<i32>> = vec![vec![1]]; // >> closes two generic lists
    println!("{v:?}");
}
```

```output for=raw-strings-and-raw-names
1000000 10 15 255 65
He said "hi" to C:\temp
6
2 1
[[1]]
```

- Verdict: library. The compiler passes token streams to procedural macros through the `proc_macro` crate, and the `proc-macro2` and `syn` libraries parse Rust source in other programs. A suffix such as `u64` gives a literal its type, `b'A'` is a byte, raw strings with `#` delimiters hold quotes and backslashes, and `r#` turns a keyword into a name. The parser splits `>>` into two closing brackets where a type needs them.
