```rust run id=packed
fn to_packed(n: i64) -> Vec<u8> {
    let mut nibbles: Vec<u8> = n.unsigned_abs().to_string().bytes().map(|c| c - b'0').collect();
    nibbles.push(if n < 0 { 0xD } else { 0xC });
    if nibbles.len() % 2 == 1 {
        nibbles.insert(0, 0);
    }
    nibbles.chunks(2).map(|p| p[0] << 4 | p[1]).collect()
}

fn from_packed(b: &[u8]) -> Option<i64> {
    let mut v: i64 = 0;
    for (i, &x) in b.iter().enumerate() {
        let (hi, lo) = (x >> 4, x & 0x0F);
        let last = i == b.len() - 1;
        if hi > 9 || (!last && lo > 9) {
            return None;
        }
        v = v * 10 + hi as i64;
        if !last {
            v = v * 10 + lo as i64;
        } else if lo == 0xD {
            v = -v;
        }
    }
    Some(v)
}

fn main() {
    let p = to_packed(-12345);
    let hex: Vec<String> = p.iter().map(|b| format!("{:02X}", b)).collect();
    println!("packed -12345: {}", hex.join(" "));
    println!("back: {:?}", from_packed(&p));
    println!("1A 3C: {:?}", from_packed(&[0x1A, 0x3C]));
    println!("0x45 as binary: {}, as BCD: {}", 0x45, (0x45 >> 4) * 10 + (0x45 & 0x0F));
}
```

```output for=packed
packed -12345: 12 34 5D
back: Some(-12345)
1A 3C: None
0x45 as binary: 69, as BCD: 45
```

- Verdict: idiom. Rust has no BCD type. `chunks(2)` joins each pair of nibbles into a byte.
- `from_packed` returns `None` for an invalid nibble, so the caller must handle bad data.
