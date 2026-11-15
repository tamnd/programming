```rust run id=exclusive-inclusive-ranges-and-windows
fn main() {
    // 0..n excludes n, 0..=n includes it.
    let a: Vec<u32> = (0..5).collect();
    let b: Vec<u32> = (0..=5).collect();
    println!("{:?} len {}", a, a.len());
    println!("{:?} len {}", b, b.len());

    // The length of start..end is end - start.
    let r = 3..8;
    println!("{} {}", r.len(), r.end - r.start);

    // get returns None past the end; indexing would panic.
    let v = vec![10, 20, 30];
    println!("{:?} {:?}", v.get(2), v.get(3));
    println!("{:?}", v.last());

    // n elements have n - 1 windows of two.
    let temps = [12, 15, 11, 18];
    let rises: Vec<i32> = temps.windows(2).map(|w| w[1] - w[0]).collect();
    println!("{:?}", rises);

    // A slice of a slice is half-open too.
    let s = "hamburger";
    println!("{}", &s[4..8]);
}
```

```output for=exclusive-inclusive-ranges-and-windows
[0, 1, 2, 3, 4] len 5
[0, 1, 2, 3, 4, 5] len 6
5 5
Some(30) None
Some(30)
[3, -4, 7]
urge
```

- Verdict: native. Rust has both forms in the syntax: `0..5` has five values and `0..=5` has six, so the reader sees the choice. `v.get(3)` returns `None`, where `v[3]` would panic with an index-out-of-bounds message. `windows(2)` gives the three pairs of neighbors of four values, with no index arithmetic.
