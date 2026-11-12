```rust run id=mpsc-and-ownership
use std::sync::mpsc;
use std::thread;

enum Message {
    Add(i64),
    Get(mpsc::Sender<i64>),
    Stop,
}

fn main() {
    // A thread owns the total and changes it only when a message arrives.
    let (tx, rx) = mpsc::channel::<Message>();
    let worker = thread::spawn(move || {
        let mut total = 0;
        for msg in rx {
            match msg {
                Message::Add(n) => total += n,
                Message::Get(reply) => reply.send(total).unwrap(),
                Message::Stop => break,
            }
        }
    });

    // Each sender is a clone of tx: many producers, one consumer.
    let senders: Vec<_> = (0..4)
        .map(|_| {
            let tx = tx.clone();
            thread::spawn(move || {
                for _ in 0..1000 {
                    tx.send(Message::Add(1)).unwrap();
                }
            })
        })
        .collect();
    for s in senders {
        s.join().unwrap();
    }
    let (reply_tx, reply_rx) = mpsc::channel();
    tx.send(Message::Get(reply_tx)).unwrap();
    println!("total {}", reply_rx.recv().unwrap());
    tx.send(Message::Stop).unwrap();
    worker.join().unwrap();

    // send moves the value, so the sender cannot use it after the send.
    let (stx, srx) = mpsc::channel();
    let text = String::from("hello");
    stx.send(text).unwrap();
    // println!("{text}"); does not compile, because text was moved.
    println!("{}", srx.recv().unwrap());

    // A bounded channel refuses a message when it is full.
    let (btx, brx) = mpsc::sync_channel::<i32>(1);
    println!("{:?} {}", btx.try_send(1), btx.try_send(2).is_err());
    drop(btx);
    // When every sender is gone, the receiver gets the rest and then stops.
    println!("{:?}", brx.iter().collect::<Vec<_>>());
}
```

```output for=mpsc-and-ownership
total 4000
hello
Ok(()) true
[1]
```

- Verdict: stdlib. `mpsc::channel` gives one receiver and a sender that each producer clones. A thread owns the total and matches on an enum of messages, and a `Get` message carries its own reply sender. `send` moves the `String`, so the sender cannot use it. `try_send` on a full `sync_channel(1)` fails, and the receiver stops after the last sender is dropped.
