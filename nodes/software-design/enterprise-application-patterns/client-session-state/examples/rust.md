```rust run id=cookie-cart
// The Rust standard library has no SHA-256 or HMAC, so this file has small versions of both.
// A real program would use a tested crate, such as hmac and sha2.

const K: [u32; 64] = [
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
];

fn sha256(input: &[u8]) -> [u8; 32] {
    let mut h: [u32; 8] = [
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
    ];
    let mut msg = input.to_vec();
    msg.push(0x80);
    while msg.len() % 64 != 56 {
        msg.push(0);
    }
    msg.extend_from_slice(&((input.len() as u64) * 8).to_be_bytes());
    for block in msg.chunks(64) {
        let mut w = [0u32; 64];
        for i in 0..16 {
            w[i] = u32::from_be_bytes([block[4 * i], block[4 * i + 1], block[4 * i + 2], block[4 * i + 3]]);
        }
        for i in 16..64 {
            let s0 = w[i - 15].rotate_right(7) ^ w[i - 15].rotate_right(18) ^ (w[i - 15] >> 3);
            let s1 = w[i - 2].rotate_right(17) ^ w[i - 2].rotate_right(19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16].wrapping_add(s0).wrapping_add(w[i - 7]).wrapping_add(s1);
        }
        let [mut a, mut b, mut c, mut d, mut e, mut f, mut g, mut hh] = h;
        for i in 0..64 {
            let s1 = e.rotate_right(6) ^ e.rotate_right(11) ^ e.rotate_right(25);
            let ch = (e & f) ^ (!e & g);
            let t1 = hh.wrapping_add(s1).wrapping_add(ch).wrapping_add(K[i]).wrapping_add(w[i]);
            let s0 = a.rotate_right(2) ^ a.rotate_right(13) ^ a.rotate_right(22);
            let maj = (a & b) ^ (a & c) ^ (b & c);
            let t2 = s0.wrapping_add(maj);
            hh = g;
            g = f;
            f = e;
            e = d.wrapping_add(t1);
            d = c;
            c = b;
            b = a;
            a = t1.wrapping_add(t2);
        }
        for (x, y) in h.iter_mut().zip([a, b, c, d, e, f, g, hh]) {
            *x = x.wrapping_add(y);
        }
    }
    let mut out = [0u8; 32];
    for (i, x) in h.iter().enumerate() {
        out[4 * i..4 * i + 4].copy_from_slice(&x.to_be_bytes());
    }
    out
}

fn hmac_sha256(key: &[u8], data: &[u8]) -> [u8; 32] {
    let mut k = [0u8; 64];
    if key.len() > 64 {
        k[..32].copy_from_slice(&sha256(key));
    } else {
        k[..key.len()].copy_from_slice(key);
    }
    let mut inner: Vec<u8> = k.iter().map(|b| b ^ 0x36).collect();
    inner.extend_from_slice(data);
    let mut outer: Vec<u8> = k.iter().map(|b| b ^ 0x5c).collect();
    outer.extend_from_slice(&sha256(&inner));
    sha256(&outer)
}

const SECRET: &[u8] = b"server secret, never sent to the client";

fn sign(data: &str) -> String {
    hmac_sha256(SECRET, data.as_bytes()).iter().map(|b| format!("{:02x}", b)).collect()
}

// Compares in a time that does not depend on where the first difference is.
fn same(a: &str, b: &str) -> bool {
    a.len() == b.len() && a.bytes().zip(b.bytes()).fold(0, |acc, (x, y)| acc | (x ^ y)) == 0
}

// Serializes the session state into the value of a cookie: the data, a bar and the signature.
fn encode(cart: &[(String, u32)]) -> String {
    let data: Vec<String> = cart.iter().map(|(item, qty)| format!("{}:{}", item, qty)).collect();
    let data = data.join(",");
    let tag = sign(&data);
    format!("{}|{}", data, tag)
}

// Checks the signature and gives back the state. A changed cookie gives an error.
fn decode(cookie: &str) -> Result<Vec<(String, u32)>, String> {
    let (data, tag) = cookie.rsplit_once('|').ok_or("no signature")?;
    if !same(tag, &sign(data)) {
        return Err("bad signature".to_string());
    }
    let mut cart = Vec::new();
    for part in data.split(',').filter(|p| !p.is_empty()) {
        let (item, qty) = part.split_once(':').ok_or("bad item")?;
        cart.push((item.to_string(), qty.parse::<u32>().map_err(|e| e.to_string())?));
    }
    Ok(cart)
}

// One request. The server keeps nothing between requests: the state comes in and goes out in the cookie.
fn handle_add(cookie: Option<&str>, item: &str) -> Result<String, String> {
    let mut cart = match cookie {
        Some(c) => decode(c)?,
        None => Vec::new(),
    };
    match cart.iter_mut().find(|(i, _)| i == item) {
        Some(line) => line.1 += 1,
        None => cart.push((item.to_string(), 1)),
    }
    Ok(encode(&cart))
}

fn show(cookie: &str) -> String {
    let (data, tag) = cookie.rsplit_once('|').unwrap();
    format!("{}|{}... ({} bytes)", data, &tag[..8], cookie.len())
}

fn main() {
    let mut cookie: Option<String> = None;
    for item in ["pen", "pen", "ink"] {
        let next = handle_add(cookie.as_deref(), item).unwrap();
        println!("add {}: cookie {}", item, show(&next));
        cookie = Some(next);
    }
    let cookie = cookie.unwrap();
    let tampered = cookie.replace("ink:1", "ink:9");
    if let Err(e) = handle_add(Some(&tampered), "pen") {
        println!("a client changes ink:1 to ink:9: {}", e);
    }
    let cart: Vec<String> = decode(&cookie).unwrap().iter().map(|(i, q)| format!("{} {}", i, q)).collect();
    println!("cart: {}", cart.join(", "));
}
```

```output
add pen: cookie pen:1|2c06beca... (70 bytes)
add pen: cookie pen:2|bdba40d9... (70 bytes)
add ink: cookie pen:2,ink:1|8bdcc52b... (76 bytes)
a client changes ink:1 to ink:9: bad signature
cart: pen 2, ink 1
```

- Verdict: library. The Rust standard library has no SHA-256 and no HMAC, so the file has short versions of both, and `same` compares the signatures in constant time. A real program would use tested crates, such as `hmac` and `sha2`.
- `decode` returns a `Result`, and the `?` operator passes a bad signature back to the caller.
