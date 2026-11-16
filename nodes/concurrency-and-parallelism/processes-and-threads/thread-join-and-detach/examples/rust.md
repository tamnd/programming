```rust run id=join-and-drop
use std::sync::mpsc;
use std::thread;

fn main() {
    let handle = thread::spawn(|| 6 * 7);
    let answer = handle.join().expect("the thread panicked");
    println!("joined: {answer}");

    std::panic::set_hook(Box::new(|_| {}));
    let failed = thread::spawn(|| -> i32 { panic!("bad input") });
    println!("join of a panicked thread is Err: {}", failed.join().is_err());
    let _ = std::panic::take_hook();

    let mut data = vec![1, 2, 3];
    thread::scope(|s| {
        s.spawn(|| data.push(4));
    });
    println!("after the scope: {data:?}");

    let (keep_open, never) = mpsc::channel::<()>();
    let waiting = thread::spawn(move || {
        let _ = never.recv();
    });
    println!("waiting thread finished: {}", waiting.is_finished());
    drop(waiting);
    println!("main returns; the dropped handle detached the thread");
    drop(keep_open);
}
```

```output for=join-and-drop
joined: 42
join of a panicked thread is Err: true
after the scope: [1, 2, 3, 4]
waiting thread finished: false
main returns; the dropped handle detached the thread
```

- Verdict: stdlib. `JoinHandle::join` waits for the thread and returns its result, or an `Err` if the thread panicked. A dropped `JoinHandle` detaches the thread.
- The example sets an empty panic hook so that the panic prints nothing, then restores the default hook with `take_hook`.
- `thread::scope` joins every thread that it started before it returns. So the scoped thread can borrow `data` mutably, and the main thread reads it after the scope.
- The waiting thread blocks on a channel whose sender stays open, so it is still running. `is_finished` does not block. After `drop`, nothing can join the thread, and the program does not wait for it when `main` returns.
