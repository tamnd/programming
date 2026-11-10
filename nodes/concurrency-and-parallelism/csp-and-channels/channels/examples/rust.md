```rust run id=mpsc
use std::sync::mpsc;
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();
    let mut handles = Vec::new();
    for id in 1..=2 {
        let tx = tx.clone(); // one Sender for each producer
        handles.push(thread::spawn(move || {
            for i in 1..=3 {
                tx.send(id * 10 * i).unwrap();
            }
        }));
    }
    drop(tx); // the original Sender
    let total: i32 = rx.iter().sum(); // ends when every Sender is dropped
    for h in handles {
        h.join().unwrap();
    }
    println!("total: {total}");
    println!("after hang-up: {:?}", rx.recv());

    let (tx, rx) = mpsc::sync_channel::<&str>(0); // bound 0: a rendezvous
    let sender = thread::spawn(move || {
        tx.send("ping").unwrap(); // blocks until the receive below
    });
    println!("got: {}", rx.recv().unwrap());
    sender.join().unwrap();

    let (_tx, rx) = mpsc::channel::<i32>();
    println!("{:?}", rx.recv_timeout(Duration::from_millis(50)));
}
```

```output for=mpsc
total: 180
after hang-up: Err(RecvError)
got: ping
Err(Timeout)
```

- Verdict: stdlib. `std::sync::mpsc` gives `channel` (unbounded), `sync_channel` (bounded) and cloneable senders with one receiver.
- There is no `close`. The channel hangs up when all `Sender` values are dropped, and `send` and `recv` then return `Err`.
- The module has no `select`. `recv_timeout` and `try_recv` give a receive with a time limit and a receive that does not block.
