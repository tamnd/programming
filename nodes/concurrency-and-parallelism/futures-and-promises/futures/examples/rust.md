```rust run id=lazy
use std::future::Future;
use std::pin::pin;
use std::task::{Context, Poll, Waker};
use std::thread;

fn block_on<F: Future>(future: F) -> F::Output {
    let mut future = pin!(future);
    let mut cx = Context::from_waker(Waker::noop());
    loop {
        if let Poll::Ready(value) = future.as_mut().poll(&mut cx) {
            return value;
        }
        thread::yield_now();
    }
}

async fn price() -> i32 {
    println!("price runs");
    40
}

fn main() {
    let future = price();
    println!("future created");
    println!("{}", block_on(future) + 2);

    let handle = thread::spawn(|| 6 * 7);
    println!("{}", handle.join().unwrap());
}
```

```output for=lazy
future created
price runs
42
42
```

- Verdict: native. An `async fn` returns a value that implements `Future`. Futures in Rust are lazy: the body does not run until something polls the future, usually through `.await`.
- The standard library has the `Future` trait but no executor. Programs use a runtime crate, such as Tokio, to poll futures and wake them when I/O is ready. This example uses a minimal executor that only works for futures that never wait.
- `thread::spawn` returns a `JoinHandle`, which acts as a future for a thread: `join` waits for the result.
