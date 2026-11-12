```rust run id=string-capacity-and-slices
use std::mem::size_of;

fn main() {
    // String is a pointer, a capacity and a length. &str is a pointer and a length.
    println!("{} {} {}", size_of::<String>(), size_of::<&str>(), size_of::<Box<str>>());

    // The capacity can be larger than the length, so a push may not allocate.
    let mut s = String::with_capacity(16);
    s.push_str("hello");
    println!("{} {}", s.len(), s.capacity());

    // A slice points into the same buffer. It copies nothing.
    let world = String::from("hello, world");
    let part: &str = &world[7..];
    let offset = part.as_ptr() as usize - world.as_ptr() as usize;
    println!("{part} {offset}");

    // The buffer holds UTF-8 bytes.
    for t in ["café", "日本", "😀"] {
        println!("{t}: bytes={} {:02x?}", t.len(), t.as_bytes());
    }
}
```

```output for=string-capacity-and-slices
24 16 16
5 16
world 7
café: bytes=5 [63, 61, 66, c3, a9]
日本: bytes=6 [e6, 97, a5, e6, 9c, ac]
😀: bytes=4 [f0, 9f, 98, 80]
```

- Verdict: native. `String` is 24 bytes (pointer, capacity and length), and `&str` and `Box<str>` are 16 bytes (pointer and length). `with_capacity(16)` makes room before the push, so the capacity is larger than the length. The slice `&world[7..]` points 7 bytes into the same buffer. `as_bytes` shows the UTF-8 bytes.
