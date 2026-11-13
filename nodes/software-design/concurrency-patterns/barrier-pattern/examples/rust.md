```rust run id=barrier-pattern
use std::sync::atomic::{AtomicI32, Ordering};
use std::sync::Barrier;
use std::thread;

const N: usize = 8;

fn main() {
    let values: Vec<AtomicI32> = (1..=N as i32).map(AtomicI32::new).collect();
    let distances = [1, 2, 4]; // each step adds the value that is d places to the left
    let show = |v: &[AtomicI32]| v.iter().map(|x| x.load(Ordering::SeqCst)).collect::<Vec<_>>();
    println!("input: {:?}", show(&values));
    let trips = AtomicI32::new(0);
    let barrier = Barrier::new(N);

    thread::scope(|s| {
        for i in 0..N {
            let (values, barrier, trips) = (&values, &barrier, &trips);
            s.spawn(move || {
                for d in distances {
                    // Phase 1: read a value that the last step wrote.
                    let add = if i >= d { values[i - d].load(Ordering::SeqCst) } else { 0 };
                    // Nobody writes until every worker has read. One thread is the leader of each trip.
                    if barrier.wait().is_leader() {
                        trips.fetch_add(1, Ordering::SeqCst);
                    }
                    // Phase 2: write the new value.
                    values[i].fetch_add(add, Ordering::SeqCst);
                    // Nobody reads until every worker has written.
                    if barrier.wait().is_leader() {
                        trips.fetch_add(1, Ordering::SeqCst);
                    }
                }
            });
        }
    });

    let result = show(&values);
    let matches = (0..N).all(|i| result[i] == ((i + 1) * (i + 2) / 2) as i32);
    println!("steps: {}", distances.len());
    println!("barrier trips: {}", trips.load(Ordering::SeqCst));
    println!("prefix sums: {:?}", result);
    println!("matches a running total: {}", if matches { "True" } else { "False" });
}
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: stdlib. `std::sync::Barrier`; `wait` returns a result whose `is_leader()` is true for one thread per trip, and that thread counts the trip.
