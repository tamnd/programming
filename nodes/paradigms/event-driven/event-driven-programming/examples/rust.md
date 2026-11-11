```rust run id=channel
use std::sync::mpsc;
use std::thread;

enum Event {
    Key(char),
    Resize(u16, u16),
    Quit,
}

fn main() {
    let (tx, rx) = mpsc::channel();
    let producer = thread::spawn(move || {
        for e in [Event::Key('a'), Event::Resize(80, 24), Event::Key('q'), Event::Quit] {
            tx.send(e).unwrap();
        }
    });

    let mut typed = String::new();
    for event in rx {
        match event {
            Event::Key(c) => typed.push(c),
            Event::Resize(w, h) => println!("resize to {w}x{h}"),
            Event::Quit => break,
        }
    }
    producer.join().unwrap();
    println!("typed {typed:?}");
}
```

```output for=channel
resize to 80x24
typed "aq"
```

- Verdict: idiom. The standard library has channels and threads but no event loop. An enum of events and a `match` in a loop give a typed dispatch, and the compiler checks that each kind of event has a handler.
- Async Rust is event-driven under the surface: runtimes such as Tokio poll futures when the operating system reports that a socket or timer is ready. The runtimes are crates, not part of the standard library.
