```rust run id=executor-policies
use std::future::Future;
use std::pin::{pin, Pin};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};
use std::task::{Context, Poll, Wake, Waker};
use std::thread::{self, Thread};
use std::time::Duration;

// A future that a helper thread completes after a delay.
struct Delay {
    state: Arc<Mutex<(bool, Option<Waker>)>>,
    started: bool,
}

impl Delay {
    fn new() -> Self {
        Delay { state: Arc::new(Mutex::new((false, None))), started: false }
    }
}

impl Future for Delay {
    type Output = &'static str;

    fn poll(mut self: Pin<&mut Self>, cx: &mut Context<'_>) -> Poll<Self::Output> {
        let mut state = self.state.lock().unwrap();
        if state.0 {
            return Poll::Ready("done");
        }
        state.1 = Some(cx.waker().clone());
        drop(state);
        if !self.started {
            self.started = true;
            let shared = Arc::clone(&self.state);
            thread::spawn(move || {
                thread::sleep(Duration::from_millis(20));
                let mut state = shared.lock().unwrap();
                state.0 = true;
                if let Some(waker) = state.1.take() {
                    waker.wake();
                }
            });
        }
        Poll::Pending
    }
}

// The waker of this executor sets a flag and unparks the thread that runs block_on.
struct ThreadWaker {
    thread: Thread,
    woken: AtomicBool,
}

impl Wake for ThreadWaker {
    fn wake(self: Arc<Self>) {
        self.woken.store(true, Ordering::Release);
        self.thread.unpark();
    }
}

// A minimal executor: poll the future, and sleep until its waker is called.
fn block_on<F: Future>(future: F) -> (F::Output, u32) {
    let signal = Arc::new(ThreadWaker { thread: thread::current(), woken: AtomicBool::new(false) });
    let waker = Waker::from(Arc::clone(&signal));
    let mut cx = Context::from_waker(&waker);
    let mut future = pin!(future);
    let mut polls = 0;
    loop {
        polls += 1;
        if let Poll::Ready(value) = future.as_mut().poll(&mut cx) {
            return (value, polls);
        }
        while !signal.woken.swap(false, Ordering::Acquire) {
            thread::park();
        }
    }
}

fn main() {
    let future = Delay::new();
    println!("created the future; nothing has run yet");
    let (value, polls) = block_on(future);
    println!("result: {value}");
    println!("polls: {polls}");
}
```

```output for=executor-policies
created the future; nothing has run yet
result: done
polls: 2
```

- Verdict: library. The standard library has the `Future` trait and the `Waker` type, but no executor. Programs choose one from a crate, such as Tokio.
- A Rust future does nothing until an executor polls it. `block_on` polls the future once, and the future returns `Pending`.
- The future gives the waker to its helper thread. When the thread calls `wake`, the executor polls again and gets the result. So there are two polls.
- The `woken` flag makes the loop ignore a `park` that returns without a wake.
