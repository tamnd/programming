```rust run id=sums
use std::thread;

fn main() {
    let handles: Vec<thread::JoinHandle<u64>> = (0..4u64)
        .map(|i| thread::spawn(move || (i * 250..(i + 1) * 250).sum()))
        .collect();
    let results: Vec<u64> = handles.into_iter().map(|h| h.join().unwrap()).collect();
    println!("{results:?}");

    let data = vec![1u64, 2, 3, 4, 5, 6];
    let (left, right) = data.split_at(3);
    let (a, b) = thread::scope(|s| {
        let a = s.spawn(|| left.iter().sum::<u64>());
        let b = s.spawn(|| right.iter().sum::<u64>());
        (a.join().unwrap(), b.join().unwrap())
    });
    println!("{a} + {b} = {}", a + b);
}
```

```output for=sums
[31125, 93625, 156125, 218625]
6 + 15 = 21
```

- Verdict: stdlib. `std::thread::spawn` starts an OS thread and returns a `JoinHandle<T>`. `join()` returns the value of the closure, or an `Err` if the thread panicked.
- A spawned thread can outlive the caller, so its closure must own its data (`move`, `'static`). Dropping the handle detaches the thread.
- `thread::scope` joins every thread it starts before it returns. Its threads may borrow local data, here two halves of `data`.

Without `move`, the closure borrows `name`, and the thread could outlive `main`. The compiler rejects the program with error E0373.

```rust fails expect=compile id=borrow
use std::thread;

fn main() {
    let name = String::from("worker");
    let handle = thread::spawn(|| println!("{name}"));
    handle.join().unwrap();
}
```
