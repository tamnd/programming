```rust run id=derive-ord-and-total-cmp
use std::cmp::Ordering;

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
struct Version {
    major: u32,
    minor: u32,
}

fn main() {
    let mut vs = vec![
        Version { major: 1, minor: 10 },
        Version { major: 1, minor: 2 },
        Version { major: 0, minor: 9 },
    ];
    vs.sort();
    let s: Vec<String> = vs.iter().map(|v| format!("{}.{}", v.major, v.minor)).collect();
    println!("{}", s.join(" "));

    let nan = f64::NAN;
    println!("{:?} {:?}", nan.partial_cmp(&1.0), 1.0f64.partial_cmp(&2.0));
    let mut xs = vec![2.0, nan, -1.0];
    xs.sort_by(|a: &f64, b| a.total_cmp(b));
    println!("{:?} {:?}", xs, Ordering::Less.reverse());
}
```

```output for=derive-ord-and-total-cmp
0.9 1.2 1.10
None Some(Less)
[-1.0, 2.0, NaN] Greater
```

- Verdict: stdlib. `Ord` is the trait of a total order and `PartialOrd` the trait of a partial order. `#[derive(PartialOrd, Ord)]` compares the fields in the order of their declaration.
- `Vec::sort` needs `Ord`. `f64` implements only `PartialOrd`: `partial_cmp` returns `None` for `NaN`, so a vector of floats needs `sort_by` with `total_cmp`.
- `Ordering` is an enum with `Less`, `Equal` and `Greater`, and methods such as `reverse` and `then` combine comparisons.
