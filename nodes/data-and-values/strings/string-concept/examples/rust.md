```rust run id=str-slices-and-string
fn main() {
    // A string literal is a &str: a borrowed slice of UTF-8 bytes.
    let lit: &str = "naïve 😀";
    println!("{} {}", lit.len(), lit.chars().count());

    // String is the owned, growable type.
    let mut owned = String::from("cat");
    owned.push('s');
    owned.push_str(" and dogs");
    println!("{owned} {} {}", owned.len(), owned.capacity() >= owned.len());

    // A &str can borrow part of a String, on character boundaries.
    let first: &str = &owned[..4];
    println!("{first}");

    // Bytes become a String only if they are valid UTF-8.
    let good = String::from_utf8(vec![0x63, 0x61, 0x66, 0xC3, 0xA9]);
    let bad = String::from_utf8(vec![0x61, 0xFF]);
    println!("{:?} {}", good, bad.is_err());

    // as_bytes shows the UTF-8 bytes of the text.
    println!("{:?}", "é".as_bytes());
}
```

```output for=str-slices-and-string
11 7
cats and dogs 13 true
cats
Ok("café") true
[195, 169]
```

- Verdict: native. A literal is a `&str`, and `len` counts its 11 bytes. `String` is owned and growable, with `push` and `push_str`. `&owned[..4]` borrows a slice of the `String`. `String::from_utf8` accepts valid UTF-8 and returns an error for the byte `0xFF`. `as_bytes` shows the two bytes of `é`.
