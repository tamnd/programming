```rust run id=either
enum IntOrText {
    Int(i64),
    Text(String),
}

fn describe(value: &IntOrText) -> String {
    match value {
        IntOrText::Int(n) => format!("number {}, doubled {}", n, n * 2),
        IntOrText::Text(s) => format!("text {}, upper {}", s, s.to_uppercase()),
    }
}

union Bits {
    float: f32,
    int: u32,
}

fn main() {
    for v in [IntOrText::Int(21), IntOrText::Text(String::from("ok"))] {
        println!("{}", describe(&v));
    }

    let b = Bits { float: 1.0 };
    let raw = unsafe { b.int };
    println!("bits of 1.0: {:#x}", raw);
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
bits of 1.0: 0x3f800000
```

- Verdict: native. An `enum` with data is a tagged union, and `match` must handle every case.
- A `union` is untagged. Reading a field reads the stored bits as that field's type, so it needs `unsafe`.
- Here the read is valid, because every bit pattern is a valid `u32`. It shows the bits of the `f32` value 1.0.
