```rust run id=char-boundaries-and-get
fn main() {
    let s = "naïve 😀";

    // s[0] does not compile: a str cannot be indexed by an integer.
    // as_bytes gives the bytes.
    println!("{} {:x} {:x}", s.len(), s.as_bytes()[2], s.as_bytes()[3]);

    // A range index must fall on character boundaries.
    println!("{} {}", s.is_char_boundary(2), s.is_char_boundary(3));
    println!("{:?} {:?}", s.get(0..4), s.get(0..3));

    // char_indices gives the byte index where each char starts.
    for (i, c) in s.char_indices().filter(|(_, c)| !c.is_ascii()) {
        println!("{c} at byte {i}");
    }

    // chars().nth(i) finds the i-th code point, in O(n) time.
    println!("{:?} {:?}", s.chars().nth(6), s.chars().nth(20));
}
```

```output for=char-boundaries-and-get
11 c3 af
true false
Some("naï") None
ï at byte 2
😀 at byte 7
Some('😀') None
```

- Verdict: native. `s[0]` does not compile, so the code reads bytes with `as_bytes`. Byte 2 is a character boundary, and byte 3 is not. `get(0..4)` gives `Some("naï")`, and `get(0..3)` gives `None` where `&s[0..3]` would panic. `char_indices` gives the byte where each character starts, and `chars().nth(6)` walks the string to find the emoji.
