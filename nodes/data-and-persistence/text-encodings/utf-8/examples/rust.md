```rust run id=chars
fn main() {
    let text = "nai\u{308}ve \u{1F469}\u{200D}\u{1F4BB}";
    println!("bytes {}", text.len());
    println!("chars {}", text.chars().count());
    println!("{:?} {:?}", text.get(0..3), text.get(0..4));
    println!("{} {}", text.is_char_boundary(3), text.is_char_boundary(4));

    for c in ['A', '\u{E9}', '\u{20AC}', '\u{1D11E}'] {
        let mut buffer = [0u8; 4];
        let bytes = c.encode_utf8(&mut buffer).as_bytes();
        println!("U+{:04X} {} {:02X?}", c as u32, c.len_utf8(), bytes);
    }
    println!("{:?} {:?}", char::from_u32(0xD800), char::from_u32(0x110000));
}
```

```output for=chars
bytes 19
chars 10
Some("nai") None
true false
U+0041 1 [41]
U+00E9 2 [C3, A9]
U+20AC 3 [E2, 82, AC]
U+1D11E 4 [F0, 9D, 84, 9E]
None None
```

```rust run id=decode
fn main() {
    let samples: [&[u8]; 4] = [
        b"ok \xE2\x82\xAC",
        b"a\xC0\xAFb",
        b"a\xED\xA0\x80b",
        b"a\xE2\x82",
    ];
    for bytes in samples {
        match std::str::from_utf8(bytes) {
            Ok(text) => println!("valid {text:?}"),
            Err(error) => println!(
                "valid_up_to {} error_len {:?} lossy {:?}",
                error.valid_up_to(),
                error.error_len(),
                String::from_utf8_lossy(bytes)
            ),
        }
    }

    let owned = String::from_utf8(vec![0xEF, 0xBB, 0xBF, b'h', b'i']).unwrap();
    println!("{} {:?}", owned.chars().count(), owned.strip_prefix('\u{FEFF}'));
}
```

```output for=decode
valid "ok €"
valid_up_to 1 error_len Some(1) lossy "a��b"
valid_up_to 1 error_len Some(1) lossy "a���b"
valid_up_to 1 error_len None lossy "a�"
3 Some("hi")
```

- Verdict: stdlib. The types `str` and `String` always hold valid UTF-8. The type `char` holds one Unicode scalar value, so it cannot hold a surrogate.
- `len` counts bytes, and `chars().count()` counts scalar values. A range index must fall on a character boundary. `get` returns `None` at other offsets, and the index operator `&text[0..4]` panics there.
- `str::from_utf8` checks the bytes. Its error gives `valid_up_to`, the length of the valid prefix, and `error_len`. `error_len` is `None` when the input ends in the middle of a sequence, so a stream reader can wait for more bytes.
- `String::from_utf8_lossy` replaces each maximal subpart of a bad sequence with U+FFFD.
- A leading byte order mark stays in the string as U+FEFF. The program must remove it.
- The standard library has no normalization and no grapheme segmentation. The crates `unicode-normalization` and `unicode-segmentation` do them.
