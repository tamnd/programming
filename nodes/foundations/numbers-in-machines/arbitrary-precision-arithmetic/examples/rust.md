```rust run id=bignum
const BASE: u64 = 1_000_000_000;

fn mul_small(digits: &mut Vec<u32>, m: u32) {
    let mut carry: u64 = 0;
    for d in digits.iter_mut() {
        let t = *d as u64 * m as u64 + carry;
        *d = (t % BASE) as u32;
        carry = t / BASE;
    }
    while carry > 0 {
        digits.push((carry % BASE) as u32);
        carry /= BASE;
    }
}

fn to_text(digits: &[u32]) -> String {
    let mut s = digits[digits.len() - 1].to_string();
    for d in digits.iter().rev().skip(1) {
        s.push_str(&format!("{:09}", d));
    }
    s
}

fn main() {
    let mut f: u64 = 1;
    let mut n: u64 = 1;
    while let Some(next) = f.checked_mul(n + 1) {
        f = next;
        n += 1;
    }
    println!("largest n! in u64: {}! = {}", n, f);
    let mut g: u128 = 1;
    let mut m: u128 = 1;
    while let Some(next) = g.checked_mul(m + 1) {
        g = next;
        m += 1;
    }
    println!("largest n! in u128: {}! = {}", m, g);

    let mut fact = vec![1u32];
    for i in 1..=30 {
        mul_small(&mut fact, i);
    }
    println!("30!: {}", to_text(&fact));
    let mut p = vec![1u32];
    for _ in 0..100 {
        mul_small(&mut p, 2);
    }
    println!("2^100: {}", to_text(&p));
    let mut h = vec![1u32];
    for i in 1..=100 {
        mul_small(&mut h, i);
    }
    println!("decimal digits of 100!: {}", to_text(&h).len());
}
```

```output for=bignum
largest n! in u64: 20! = 2432902008176640000
largest n! in u128: 34! = 295232799039604140847618609643520000000
30!: 265252859812191058636308480000000
2^100: 1267650600228229401496703205376
decimal digits of 100!: 158
```

- Verdict: library. The standard library stops at `u128` and `i128`, and the `num-bigint` crate gives `BigUint` and `BigInt`.
- `checked_mul` returns `None` when the product does not fit, so the loops find the largest factorial in each type.
- The block keeps a number as a `Vec<u32>` of base 10^9 digits, with the lowest digit first. That base makes the printing simple.
