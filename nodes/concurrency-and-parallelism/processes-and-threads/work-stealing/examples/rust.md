```rust run id=work-stealing-model
use std::collections::VecDeque;

// A step-by-step model of a work-stealing scheduler with two workers.
// Each worker has a deque. The front is the top (oldest task); the back is the bottom (newest task).
// A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
fn main() {
    const WORKERS: usize = 2;
    let mut deques: Vec<VecDeque<(u32, u32)>> = vec![VecDeque::new(); WORKERS];
    let mut leaves = [0; WORKERS];
    let mut total = 0;

    deques[0].push_back((0, 8));
    let mut round = 0;
    while deques.iter().any(|d| !d.is_empty()) {
        round += 1;
        for w in 0..WORKERS {
            let ((lo, hi), how, source) = if let Some(task) = deques[w].pop_back() {
                (task, "pops", String::new()) // the owner takes the newest task (LIFO)
            } else {
                let victim = (0..WORKERS).find(|&v| v != w && !deques[v].is_empty());
                let Some(victim) = victim else {
                    println!("round {round}: w{w} is idle");
                    continue;
                };
                let task = deques[victim].pop_front().unwrap(); // a thief takes the oldest task (FIFO)
                (task, "steals", format!(" from w{victim}"))
            };
            if hi - lo > 2 {
                let mid = (lo + hi) / 2;
                deques[w].push_back((lo, mid));
                deques[w].push_back((mid, hi));
                println!("round {round}: w{w} {how} [{lo}, {hi}){source} and splits it");
            } else {
                total += (lo..hi).sum::<u32>();
                leaves[w] += 1;
                println!("round {round}: w{w} {how} [{lo}, {hi}){source} and runs it");
            }
        }
    }

    let parts: Vec<String> = leaves.iter().enumerate().map(|(w, n)| format!("w{w} {n}")).collect();
    println!("leaves run: {}", parts.join(", "));
    println!("sum: {total}");
}
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: library. The standard library has no work-stealing pool. Rayon and the multi-threaded Tokio runtime are work-stealing schedulers from crates.
- This file is a model of the rules, with two `VecDeque` values in place of two threads. `pop_back` is the owner's LIFO pop, and `pop_front` is the thief's FIFO steal.
- With Rayon, `rayon::join(a, b)` runs `a` on the current thread and lets an idle worker steal `b`.
