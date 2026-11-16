```rust run id=fixedwidth
#[derive(Clone, Copy, Debug, PartialEq)]
struct Ratio {
    n: i64,
    d: i64,
}

fn gcd(mut a: i64, mut b: i64) -> i64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a.abs()
}

fn make(n: i64, d: i64) -> Option<Ratio> {
    if d == 0 {
        return None;
    }
    let (n, d) = if d < 0 { (n.checked_neg()?, d.checked_neg()?) } else { (n, d) };
    let g = gcd(n, d);
    Some(Ratio { n: n / g, d: d / g })
}

fn add(a: Ratio, b: Ratio) -> Option<Ratio> {
    let n = a.n.checked_mul(b.d)?.checked_add(b.n.checked_mul(a.d)?)?;
    let d = a.d.checked_mul(b.d)?;
    make(n, d)
}

fn main() {
    let r = make(6, -8).unwrap();
    println!("make(6, -8): {}/{}", r.n, r.d);
    let s = add(make(1, 10).unwrap(), make(2, 10).unwrap()).unwrap();
    println!("1/10 + 2/10: {}/{}", s.n, s.d);
    println!("make(1, 0): {:?}", make(1, 0));
    let mut h = make(0, 1).unwrap();
    let mut k = 1;
    loop {
        match add(h, make(1, k).unwrap()) {
            Some(next) => h = next,
            None => break,
        }
        k += 1;
    }
    println!("last sum that fits: 1/1 + ... + 1/{} = {}/{}", k - 1, h.n, h.d);
    println!("adding 1/{} overflows i64", k);
}
```

```output for=fixedwidth
make(6, -8): -3/4
1/10 + 2/10: 3/10
make(1, 0): None
last sum that fits: 1/1 + ... + 1/43 = 532145396070491417/122332313750680800
adding 1/44 overflows i64
```

- Verdict: library. The standard library has no rational type, and the `num-rational` crate has `Ratio`, with the aliases `Rational64` and `BigRational`.
- The block uses `i64` parts with checked arithmetic. The sum of 1/k overflows a product of the parts after a few dozen terms, so a long calculation needs bignum parts.
