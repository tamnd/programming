```rust run id=seeds
/// Reads records of the form [tag][length][value]; a truncated input is an error.
fn parse(data: &[u8]) -> Result<Vec<(u8, &[u8])>, String> {
    let mut records = Vec::new();
    let mut rest = data;
    while !rest.is_empty() {
        let offset = data.len() - rest.len();
        let [tag, length, tail @ ..] = rest else {
            return Err(format!("truncated header at offset {offset}"));
        };
        let Some(value) = tail.get(..usize::from(*length)) else {
            return Err(format!("truncated value at offset {offset}"));
        };
        records.push((*tag, value));
        rest = &tail[value.len()..];
    }
    Ok(records)
}

/// The fuzz target, as the body of cargo fuzz's fuzz_target! would be.
fn fuzz_one(data: &[u8]) -> String {
    match parse(data) {
        Ok(records) => format!("records={}", records.len()),
        Err(e) => format!("rejected: {e}"),
    }
}

fn main() {
    let seeds: [&[u8]; 5] = [b"", b"\x01\x02hi", b"\x01\x02hi\x02\x00", b"z", b"~\xe1"];
    for seed in seeds {
        let shown: Vec<String> = seed.iter().map(|b| format!("{b:02x}")).collect();
        let shown = if shown.is_empty() { "(empty)".to_string() } else { shown.join(" ") };
        println!("{shown}: {}", fuzz_one(seed));
    }
    println!("seed corpus: {} inputs passed", seeds.len());
}
```

```output
(empty): records=0
01 02 68 69: records=1
01 02 68 69 02 00: records=2
7a: rejected: truncated header at offset 0
7e e1: rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

The original parser panics on the input of one byte. The panic message gives the index and the length.

```rust fails expect=panic id=no-length-check
/// Counts the records of the form [tag][length][value].
fn parse(data: &[u8]) -> usize {
    let mut count = 0;
    let mut i = 0;
    while i < data.len() {
        let length = usize::from(data[i + 1]); // no check that the length byte exists
        count += 1;
        i += 2 + length;
    }
    count
}

fn main() {
    let seeds: [&[u8]; 5] = [b"", b"\x01\x02hi", b"\x01\x02hi\x02\x00", b"z", b"~\xe1"];
    for seed in seeds {
        println!("records={}", parse(seed));
    }
}
```

- Verdict: library. `cargo fuzz init` creates a fuzz target, and `cargo fuzz run` fuzzes it with libFuzzer. The tool needs the nightly compiler. The body of the target is a closure over `&[u8]`, like `fuzz_one`.
- The slice pattern `[tag, length, tail @ ..]` matches only when two bytes exist, so the header check is part of the pattern.
- `tail.get(..n)` returns `None` instead of panicking when the range is past the end.
- Safe Rust checks every index, so the bug is a panic and a fuzzer sees it. Under `cargo fuzz`, a panic is a crash, and the input is saved in the artifacts directory.
