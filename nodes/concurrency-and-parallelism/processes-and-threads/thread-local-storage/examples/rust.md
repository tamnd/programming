```rust run id=per-thread-values
use std::cell::{Cell, RefCell};
use std::sync::mpsc;
use std::thread;

thread_local! {
    // Each thread gets its own counter, which starts at 0.
    static COUNTER: Cell<u32> = const { Cell::new(0) };
    static USER: RefCell<Option<String>> = const { RefCell::new(None) };
}

fn bump(times: u32) -> u32 {
    for _ in 0..times {
        COUNTER.set(COUNTER.get() + 1);
    }
    COUNTER.get()
}

fn main() {
    let (one, two) = thread::scope(|s| {
        let one = s.spawn(|| bump(1));
        let two = s.spawn(|| bump(2));
        (one.join().unwrap(), two.join().unwrap())
    });
    println!("main: counter = {}", bump(3));
    println!("thread 1: counter = {one}");
    println!("thread 2: counter = {two}");

    // A worker thread that takes tasks from a channel, as a pool thread does.
    let (send, receive) = mpsc::channel::<Box<dyn FnOnce() + Send>>();
    let worker = thread::spawn(move || {
        for task in receive {
            task();
        }
    });
    let show = |text: &'static str| -> Box<dyn FnOnce() + Send> {
        Box::new(move || {
            let user = USER.with_borrow(|u| u.clone().unwrap_or_else(|| "None".to_string()));
            println!("{text} {user}");
        })
    };
    send.send(Box::new(|| USER.set(Some("alice".to_string())))).unwrap();
    send.send(show("a later task on the same thread sees user:")).unwrap();
    send.send(Box::new(|| USER.set(None))).unwrap();
    send.send(show("after a task removes it, the next task sees user:")).unwrap();
    drop(send);
    worker.join().unwrap();
}
```

```output for=per-thread-values
main: counter = 3
thread 1: counter = 1
thread 2: counter = 2
a later task on the same thread sees user: alice
after a task removes it, the next task sees user: None
```

- Verdict: stdlib. The `thread_local!` macro declares a `LocalKey`. Each thread gets its own value, which it creates on first use.
- `Cell` and `RefCell` give mutable access to the value. A thread-local value is not shared, so it does not need a `Mutex`.
- The worker thread takes boxed closures from a channel and runs them in order, as a pool thread does. The value that the first task sets stays until a task sets it to `None`.
- A reference from `with` cannot leave the closure or outlive the thread.
