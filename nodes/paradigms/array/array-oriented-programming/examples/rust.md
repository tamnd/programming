```rust run id=iterators
fn main() {
    let a = [3, 1, 4, 1, 5];
    let b = [2, 7, 1, 8, 2];

    let sum: Vec<i32> = a.iter().zip(&b).map(|(x, y)| x + y).collect();
    let dot: i32 = a.iter().zip(&b).map(|(x, y)| x * y).sum();
    let scaled = a.map(|x| x * 10); // a fixed-size array maps to an array
    let scan: Vec<i32> = a
        .iter()
        .scan(0, |acc, &x| {
            *acc += x;
            Some(*acc)
        })
        .collect();

    println!("{sum:?} {dot} {scaled:?}");
    println!("{scan:?}");
}
```

```output for=iterators
[5, 8, 5, 9, 7] 35 [30, 10, 40, 10, 50]
[3, 4, 8, 9, 14]
```

- Verdict: idiom. Iterator adaptors such as `zip`, `map`, `scan` and `sum` express whole-array operations, and the compiler turns simple chains into vectorized loops. The arithmetic operators do not work on arrays. The ndarray crate gives n-dimensional arrays with broadcasting, in the style of NumPy.
