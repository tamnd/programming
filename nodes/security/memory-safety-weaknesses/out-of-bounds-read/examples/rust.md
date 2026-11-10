```rust fails expect=panic id=range-panic
fn echo(payload: &[u8], claimed: usize) -> &[u8] {
    &payload[..claimed]
}

fn main() {
    let payload = b"hello";
    println!("{:?}", echo(payload, 5));
    println!("{:?}", echo(payload, 64));
}
```

The range `..64` is past the end of a slice of 5 bytes. The program panics with `range end index 64 out of range for slice of length 5`, and no memory past the slice is read.

```rust run id=get
fn echo(payload: &[u8], claimed: usize) -> Option<&[u8]> {
    payload.get(..claimed)
}

fn main() {
    let payload = b"hello";
    for claimed in [5, 64] {
        match echo(payload, claimed) {
            Some(reply) => println!("claimed {claimed}: {}", String::from_utf8_lossy(reply)),
            None => println!("claimed {claimed}: discarded"),
        }
    }
}
```

```output for=get
claimed 5: hello
claimed 64: discarded
```

- Verdict: native. Every index and range on a slice is checked. A failed check panics, and `get` returns `None` instead.
- A panic is still a denial of service. Use `get` for lengths and indices that come from input.
- `unsafe` code with `get_unchecked` or raw pointers has no check, and it can read out of bounds as C does.
