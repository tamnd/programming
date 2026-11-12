```rust run id=reactor
use std::collections::HashMap;

enum Handler {
    Acceptor,
    Echo,
}

fn main() {
    let mut handlers: HashMap<String, Handler> = HashMap::new();
    handlers.insert("listener".to_string(), Handler::Acceptor);
    // ready stands in for select: handle and data pairs in arrival order
    let ready = [
        ("listener", "conn-1"), ("listener", "conn-2"),
        ("conn-1", "hello"), ("conn-2", "hi"),
        ("conn-1", ""), ("conn-2", ""),
    ];
    for (handle, data) in ready {
        match handlers.get(handle) {
            Some(Handler::Acceptor) => {
                println!("accept {}", data);
                handlers.insert(data.to_string(), Handler::Echo);
            }
            Some(Handler::Echo) if !data.is_empty() => println!("{}: {}", handle, data),
            Some(Handler::Echo) => {
                println!("close {}", handle);
                handlers.remove(handle);
            }
            None => {}
        }
    }
}
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: idiom. The handlers are the variants of an enum, and the dispatcher matches on them. A closure that changes the table it is stored in would need shared mutable state, so the enum keeps the borrow simple.
