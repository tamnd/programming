```rust run id=master-worker-pattern
use std::thread;

const LIMIT: u32 = 20000;
const WORKERS: u32 = 4;

// The work of one worker: count the primes in [lo, hi).
fn count_primes(lo: u32, hi: u32) -> u32 {
    let mut count = 0;
    for n in lo.max(2)..hi {
        let mut d = 2;
        while d * d <= n && n % d != 0 {
            d += 1;
        }
        if d * d > n {
            count += 1;
        }
    }
    count
}

fn main() {
    // 1. Split the work into one part for each worker.
    let parts: Vec<(u32, u32)> = (0..WORKERS)
        .map(|i| (LIMIT * i / WORKERS, LIMIT * (i + 1) / WORKERS))
        .collect();
    // 2. Start the workers in a scope; the scope waits for all of them.
    let results: Vec<u32> = thread::scope(|s| {
        let handles: Vec<_> = parts
            .iter()
            .map(|&(lo, hi)| s.spawn(move || count_primes(lo, hi)))
            .collect();
        handles.into_iter().map(|h| h.join().unwrap()).collect()
    });
    // 3. Combine the partial results.
    let total: u32 = results.iter().sum();

    let shown: Vec<String> = parts.iter().map(|(lo, hi)| format!("({}, {})", lo, hi)).collect();
    println!("parts: [{}]", shown.join(", "));
    println!("primes in each part: {:?}", results);
    println!("primes below {} from the master: {}", LIMIT, total);
    println!("same as one loop: {}", if total == count_primes(0, LIMIT) { "True" } else { "False" });
}
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `std::thread::scope` lets the workers borrow the parts, and the scope ends only when every worker has joined.
