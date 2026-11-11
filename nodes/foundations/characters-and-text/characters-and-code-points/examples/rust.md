```rust run id=str-and-char
fn main() {
    let s = "cafe\u{301} \u{1F44D}\u{1F3FD}";
    println!("{} {}", s.len(), s.chars().count());
    println!("{} {}", s.is_char_boundary(4), s.is_char_boundary(5));

    let found: Vec<String> = s
        .char_indices()
        .filter(|(_, c)| !c.is_ascii())
        .map(|(i, c)| format!("{i}:U+{:04X}", c as u32))
        .collect();
    println!("{}", found.join(" "));

    let units: Vec<u16> = s.encode_utf16().collect();
    println!("{} {:X}", units.len(), units[6]);

    let bytes = vec![0x63, 0x61, 0x66, 0xE9];
    println!("{}", String::from_utf8(bytes.clone()).is_err());
    println!("{}", String::from_utf8_lossy(&bytes));
    println!("{}", "stra\u{df}e".to_uppercase());
}
```

```output for=str-and-char
15 8
true false
4:U+0301 7:U+1F44D 11:U+1F3FD
10 D83D
true
caf�
STRASSE
```

```rust fails expect=panic id=slice-inside-char
fn main() {
    let name = "Ng\u{f4} B\u{1ea3}o";
    let short = &name[..3]; // byte 3 is inside the two bytes of the second letter
    println!("{short}");
}
```

- Verdict: native. A Rust `str` is always valid UTF-8, and `len` counts bytes. `char` is a Unicode scalar value, `chars` decodes the string one `char` at a time, and `is_char_boundary` tells whether a byte index starts a character. `encode_utf16` gives UTF-16 units for APIs that need them. `String::from_utf8` rejects invalid bytes, and `from_utf8_lossy` replaces them with U+FFFD.
- The fails block slices a string at byte 3, which falls inside a two-byte character, and the program panics. Slice at indices from `char_indices`, or check `is_char_boundary` first.
