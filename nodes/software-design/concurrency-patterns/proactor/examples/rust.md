```rust run id=proactor
use std::collections::HashMap;
use std::sync::mpsc;
use std::thread;

type Handler = fn(&str, usize);

fn on_read(name: &str, size: usize) {
    println!("{} done: {} bytes", name, size);
}

fn main() {
    let files: HashMap<&str, &str> = HashMap::from([("a.txt", "hello"), ("b.txt", "abc")]);
    let (op_tx, op_rx) = mpsc::channel::<(&'static str, Handler)>();
    let (done_tx, done_rx) = mpsc::channel::<(Handler, &'static str, usize)>();
    let processor = thread::spawn(move || {
        for (name, handler) in op_rx {
            done_tx.send((handler, name, files[name].len())).unwrap();
        }
    });
    for name in ["a.txt", "b.txt"] {
        println!("start read {}", name);
        op_tx.send((name, on_read)).unwrap();
    }
    drop(op_tx);
    for (handler, name, size) in done_rx {
        handler(name, size); // the Completion Dispatcher
    }
    processor.join().unwrap();
}
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: stdlib. `std::sync::mpsc` channels join the threads, and the loop over the completion channel ends when the processor drops its sender.
