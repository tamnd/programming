```rust run id=acceptor
use std::cell::RefCell;
use std::collections::{HashMap, VecDeque};
use std::rc::Rc;

type Channel = Rc<RefCell<VecDeque<String>>>;

trait ServiceHandler {
    fn open(channel: Channel) -> Self;
}

struct StatusHandler { channel: Channel }
impl ServiceHandler for StatusHandler {
    fn open(channel: Channel) -> Self {
        println!("accept -> status handler open");
        StatusHandler { channel }
    }
}
impl StatusHandler {
    fn poll(&self) {
        let message = self.channel.borrow_mut().pop_front().unwrap();
        println!("status: got {}", message);
    }
}

struct CommandHandler { channel: Channel }
impl ServiceHandler for CommandHandler {
    fn open(channel: Channel) -> Self {
        println!("connect done -> command handler open");
        CommandHandler { channel }
    }
}
impl CommandHandler {
    fn send(&self, text: &str) {
        println!("command: send {}", text);
        self.channel.borrow_mut().push_back(text.to_string());
    }
}

struct Acceptor<H: ServiceHandler> { handlers: Vec<H> }
impl<H: ServiceHandler> Acceptor<H> {
    fn accept(&mut self, channel: Channel) {
        self.handlers.push(H::open(channel));
    }
}

fn connect<H: ServiceHandler>(address: &str, listeners: &mut HashMap<String, Acceptor<StatusHandler>>) -> H {
    println!("connect {}", address);
    let channel: Channel = Rc::new(RefCell::new(VecDeque::new()));
    listeners.get_mut(address).unwrap().accept(Rc::clone(&channel));
    H::open(channel) // complete: activate when the connection is up
}

fn main() {
    let mut listeners: HashMap<String, Acceptor<StatusHandler>> = HashMap::new();
    listeners.insert("gateway:9000".to_string(), Acceptor { handlers: Vec::new() });
    println!("listen gateway:9000");
    let command: CommandHandler = connect("gateway:9000", &mut listeners);
    command.send("reset");
    listeners["gateway:9000"].handlers[0].poll();
}
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. The trait has a constructor-like `open`, so the generic acceptor and connector can create a handler of the type parameter. `Rc<RefCell<...>>` lets both handlers share the channel in one thread.
