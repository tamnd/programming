```rust run id=named-steps
struct Request {
    path: &'static str,
    status: u16,
    ms: u32,
}

fn slow_failures(log: &[Request]) -> Vec<&'static str> {
    // Iterators have no sort, so collect the failures, sort, then go on.
    let mut failed: Vec<&Request> = log.iter().filter(|r| r.status >= 500).collect();
    failed.sort_by(|a, b| b.ms.cmp(&a.ms));
    failed.iter().take(3).map(|r| r.path).collect()
}

fn main() {
    let log = [
        Request {
            path: "/a",
            status: 200,
            ms: 40,
        },
        Request {
            path: "/b",
            status: 503,
            ms: 900,
        },
        Request {
            path: "/c",
            status: 500,
            ms: 120,
        },
        Request {
            path: "/d",
            status: 502,
            ms: 2500,
        },
        Request {
            path: "/e",
            status: 500,
            ms: 300,
        },
        Request {
            path: "/f",
            status: 404,
            ms: 10,
        },
    ];
    assert_eq!(slow_failures(&log), vec!["/d", "/b", "/e"]);

    // Laziness: an adapter does nothing until something consumes it.
    let mut calls = 0;
    let lazy = log.iter().map(|r| {
        calls += 1;
        r.ms
    });
    drop(lazy);
    assert_eq!(calls, 0);
    println!("replace loop with pipeline: tests pass");
}
```

```output
replace loop with pipeline: tests pass
```
