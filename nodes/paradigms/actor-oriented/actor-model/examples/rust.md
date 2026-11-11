```rust run id=thread-actor
use std::sync::mpsc::{channel, Sender};
use std::thread;

enum Msg {
    Deposit(u32),
    Withdraw(u32),
    Balance(Sender<u32>),
}

fn spawn_account() -> (Sender<Msg>, thread::JoinHandle<()>) {
    let (tx, rx) = channel();
    let handle = thread::spawn(move || {
        let mut balance = 0;
        for msg in rx {
            match msg {
                Msg::Deposit(n) => balance += n,
                Msg::Withdraw(n) if n <= balance => balance -= n,
                Msg::Withdraw(_) => {}
                Msg::Balance(reply) => reply.send(balance).unwrap(),
            }
        }
    });
    (tx, handle)
}

fn main() {
    let (account, handle) = spawn_account();
    let clients: Vec<_> = (0..4)
        .map(|_| {
            let account = account.clone();
            thread::spawn(move || {
                for _ in 0..25 {
                    account.send(Msg::Deposit(10)).unwrap();
                }
            })
        })
        .collect();
    for c in clients {
        c.join().unwrap();
    }

    account.send(Msg::Withdraw(1)).unwrap();
    let (reply_tx, reply_rx) = channel();
    account.send(Msg::Balance(reply_tx)).unwrap();
    println!("balance {}", reply_rx.recv().unwrap());
    drop(account);
    handle.join().unwrap();
}
```

```output for=thread-actor
balance 999
```

```rust fails expect=compile id=rc-message
use std::rc::Rc;
use std::sync::mpsc::channel;
use std::thread;

fn main() {
    let (tx, rx) = channel::<Rc<Vec<i32>>>();
    thread::spawn(move || {
        let shared = rx.recv().unwrap();
        println!("{}", shared.len());
    });
    tx.send(Rc::new(vec![1, 2, 3])).unwrap();
}
```

- Verdict: idiom. The standard library has threads and channels but no actor type. The type system enforces the main rule of actors: a message must be `Send`, so a value that would let two threads share memory without synchronization cannot be sent. The second block fails with error E0277, because `Rc` is not `Send`. Crates such as Actix, Ractor and Kameo give actor systems on top of async runtimes.
- When the last `Sender` is dropped, the `for` loop in the actor ends, and the thread returns. The `drop(account)` in `main` stops the actor.
