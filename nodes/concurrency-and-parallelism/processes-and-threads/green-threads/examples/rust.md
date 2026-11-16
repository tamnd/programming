```rust run id=hand-executor
use std::collections::VecDeque;
use std::future::Future;
use std::pin::Pin;
use std::task::{Context, Poll, Waker};

// A future that is pending once, so the task gives control back to the scheduler.
struct YieldNow(bool);

impl Future for YieldNow {
    type Output = ();

    fn poll(mut self: Pin<&mut Self>, _cx: &mut Context<'_>) -> Poll<()> {
        if self.0 {
            Poll::Ready(())
        } else {
            self.0 = true;
            Poll::Pending
        }
    }
}

async fn worker(name: &'static str, steps: u32) {
    for i in 0..steps {
        println!("{name} step {i}");
        YieldNow(false).await;
    }
}

fn main() {
    let mut ready: VecDeque<Pin<Box<dyn Future<Output = ()>>>> = VecDeque::new();
    ready.push_back(Box::pin(worker("a", 3)));
    ready.push_back(Box::pin(worker("b", 2)));
    let mut cx = Context::from_waker(Waker::noop());
    while let Some(mut task) = ready.pop_front() {
        if task.as_mut().poll(&mut cx).is_pending() {
            ready.push_back(task);
        }
    }
}
```

```output for=hand-executor
a step 0
b step 0
a step 1
b step 1
a step 2
```

- Verdict: absent. Rust had green threads in its early runtime. RFC 230 removed that runtime, and `std::thread` now starts one OS thread for each thread.
- The alternative is an `async` task: the compiler turns an `async fn` into a state machine, and an executor polls it. Most programs take the executor from a library.
- This example writes the smallest executor by hand: a queue of tasks, polled in turn. `YieldNow` is pending once, so each `await` on it gives control back to the loop.
- `Waker::noop()` is enough here because the loop polls every pending task again. A real executor uses the waker to poll a task only when it can make progress.
