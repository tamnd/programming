```rust run id=fork-join
use std::sync::atomic::{AtomicU64, AtomicUsize, Ordering};
use std::thread;

const THRESHOLD: u64 = 10_000; // a range this small is summed directly
static LEAVES: AtomicUsize = AtomicUsize::new(0);
static LARGEST: AtomicU64 = AtomicU64::new(0);

/// Returns the sum of lo, lo + 1, ..., hi - 1.
fn solve(lo: u64, hi: u64) -> u64 {
    if hi - lo <= THRESHOLD {
        LEAVES.fetch_add(1, Ordering::SeqCst);
        LARGEST.fetch_max(hi - lo, Ordering::SeqCst);
        return (lo..hi).sum();
    }
    let mid = (lo + hi) / 2;
    thread::scope(|s| {
        let left = s.spawn(|| solve(lo, mid)); // fork: a scoped thread solves the left half
        let right = solve(mid, hi); // this thread solves the right half
        left.join().unwrap() + right // join: wait for the left half
    })
}

fn main() {
    let n: u64 = 1_000_000;
    let total = solve(1, n + 1);
    let text = |b: bool| if b { "True" } else { "False" };
    println!("sum: {}", total);
    println!("leaf tasks: {}", LEAVES.load(Ordering::SeqCst));
    println!("largest leaf, at most 10000: {}", text(LARGEST.load(Ordering::SeqCst) <= THRESHOLD));
    println!("matches n(n+1)/2: {}", text(total == n * (n + 1) / 2));
}
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: stdlib. `std::thread::scope` spawns a scoped thread for the left half and joins it before the scope ends.
