```rust run id=load
use std::future::{Future, poll_fn};
use std::pin::pin;
use std::sync::{Arc, Mutex};
use std::task::{Context, Poll, Wake, Waker};
use std::thread::{self, Thread};
use std::time::Duration;

// The standard library has no executor. This one runs a single future
// on the current thread and parks the thread while the future waits.
struct Unparker(Thread);

impl Wake for Unparker {
    fn wake(self: Arc<Self>) {
        self.0.unpark();
    }
}

fn block_on<F: Future>(future: F) -> F::Output {
    let mut future = pin!(future);
    let waker = Waker::from(Arc::new(Unparker(thread::current())));
    let mut cx = Context::from_waker(&waker);
    loop {
        if let Poll::Ready(value) = future.as_mut().poll(&mut cx) {
            return value;
        }
        thread::park();
    }
}

// A timer future. On the first poll it starts a thread that sleeps
// and then calls the waker.
fn sleep(delay: Duration) -> impl Future<Output = ()> {
    let done = Arc::new(Mutex::new(false));
    let mut started = false;
    poll_fn(move |cx| {
        if *done.lock().unwrap() {
            return Poll::Ready(());
        }
        if !started {
            started = true;
            let (done, waker) = (Arc::clone(&done), cx.waker().clone());
            thread::spawn(move || {
                thread::sleep(delay);
                *done.lock().unwrap() = true;
                waker.wake();
            });
        }
        Poll::Pending
    })
}

// Poll two futures until both are ready.
async fn join<A: Future, B: Future>(a: A, b: B) -> (A::Output, B::Output) {
    let (mut a, mut b) = (pin!(a), pin!(b));
    let (mut ra, mut rb) = (None, None);
    poll_fn(|cx| {
        if ra.is_none()
            && let Poll::Ready(v) = a.as_mut().poll(cx)
        {
            ra = Some(v);
        }
        if rb.is_none()
            && let Poll::Ready(v) = b.as_mut().poll(cx)
        {
            rb = Some(v);
        }
        if ra.is_some() && rb.is_some() {
            Poll::Ready((ra.take().unwrap(), rb.take().unwrap()))
        } else {
            Poll::Pending
        }
    })
    .await
}

async fn load(name: &str, ms: u64) -> &str {
    println!("start {name}");
    sleep(Duration::from_millis(ms)).await;
    println!("end {name}");
    name
}

fn main() {
    let results = block_on(join(load("slow", 200), load("fast", 100)));
    println!("{}, {}", results.0, results.1);
}
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. An `async fn` returns a value that implements `Future`, and `.await` suspends the async function until that future is ready.
- A future does nothing until something polls it. The standard library defines `Future`, `Waker` and `Context`, but it has no executor and no timer. This example writes a small executor (`block_on`), a timer (`sleep`) and `join`, so it needs no library. Programs usually take these from an async runtime library.
- The executor parks the thread while the future is pending. The timer thread calls the waker, which unparks it, and the executor polls again.
- The compiler warns when a future is created and never used:

```rust wrong id=unused-future
async fn save(record: &str) {
    println!("saved {record}");
}

fn main() {
    save("order 1");
    println!("done");
}
```

```output for=unused-future
done
```

The body of `save` never runs. The compiler warning is "unused implementer of `Future` that must be used", with the note "futures do nothing unless you `.await` or poll them".
