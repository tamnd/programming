```rust run id=std-fmt
fn main() {
    let x = 3.14159_f64;
    println!("[{x:8.2}] [{x:<8.2}] [{x:08.2}] [{x:+.1}]");
    println!("[{:>6}] [{:*^9}] [{:x}] [{:#06x}] [{:b}]", "abc", "abc", 255, 255, 5);
    println!("[{:e}] [{:?}] [{:?}]", 1234.5, "a\"b", (1, 2.0));
    let w = 7;
    let p = 3;
    println!("[{x:w$.p$}] [{0}-{1}-{0}]", "a", "b");
}
```

```output for=std-fmt
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [***abc***] [ff] [0x00ff] [101]
[1.2345e3] ["a\"b"] [(1, 2.0)]
[  3.142] [a-b-a]
```

- Verdict: native. The macros `format!` and `println!` take a format string in the brace syntax, with fill, alignment, sign, `#`, zero fill, width and precision. The type letter picks a trait: none for `Display`, `?` for `Debug`, `x` and `b` for hexadecimal and binary. `w$` and `p$` read the width and the precision from variables. The compiler checks the whole format string.
