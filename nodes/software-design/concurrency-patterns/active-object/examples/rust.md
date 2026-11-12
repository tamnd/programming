```rust run id=active
use std::sync::mpsc::{channel, Receiver, Sender};
use std::thread::{self, JoinHandle};

// The servant: only the scheduler thread owns it, so it has no lock.
struct AccountServant {
    total: i32,
}

// A method request: a call to run on the servant.
type Request = Box<dyn FnOnce(&mut AccountServant) + Send>;

// The proxy: each method makes a request and returns a future at once.
struct AccountProxy {
    activation: Option<Sender<Request>>,
    gate: Sender<()>, // lets the demo hold the scheduler
    scheduler: Option<JoinHandle<()>>,
}

impl AccountProxy {
    fn new() -> AccountProxy {
        let (activation, queue) = channel::<Request>();
        let (gate, gate_rx) = channel::<()>();
        let scheduler = thread::spawn(move || {
            let mut servant = AccountServant { total: 0 };
            let _ = gate_rx.recv();
            for request in queue {
                request(&mut servant);
            }
        });
        AccountProxy { activation: Some(activation), gate, scheduler: Some(scheduler) }
    }

    fn send(&self, call: impl FnOnce(&mut AccountServant) -> i32 + Send + 'static) -> Receiver<i32> {
        let (future, result) = channel();
        let request: Request = Box::new(move |s| {
            let _ = future.send(call(s));
        });
        self.activation.as_ref().unwrap().send(request).unwrap();
        result
    }

    fn deposit(&self, amount: i32) -> Receiver<i32> {
        self.send(move |s| {
            s.total += amount;
            s.total
        })
    }

    fn balance(&self) -> Receiver<i32> {
        self.send(|s| s.total)
    }
}

impl Drop for AccountProxy {
    fn drop(&mut self) {
        self.activation.take(); // closing the queue ends the scheduler loop
        self.scheduler.take().unwrap().join().unwrap();
    }
}

fn main() {
    let account = AccountProxy::new();
    let per_client: Vec<Vec<Receiver<i32>>> = thread::scope(|s| {
        let handles: Vec<_> = (0..4)
            .map(|_| s.spawn(|| (0..250).map(|_| account.deposit(1)).collect::<Vec<_>>()))
            .collect();
        handles.into_iter().map(|h| h.join().unwrap()).collect()
    });
    let futures: Vec<&Receiver<i32>> = per_client.iter().flatten().collect();
    let done = futures.iter().filter(|f| f.try_recv().is_ok()).count();
    println!("deposits queued: {}", futures.len());
    println!("deposits done before the scheduler starts: {done}");
    account.gate.send(()).unwrap();
    println!("balance after 1000 deposits: {}", account.balance().recv().unwrap());
}
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: idiom. An `mpsc` channel of boxed closures is the activation queue, and a one-value channel is the future. The servant moves into the scheduler thread, so no other thread can reach it.
