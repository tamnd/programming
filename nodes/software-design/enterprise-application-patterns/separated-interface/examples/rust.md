```rust run id=notifier-interface
use std::cell::RefCell;

// ---- module domain: it defines the trait that it needs, and uses nothing below.
mod domain {
    pub trait Notifier {
        fn send(&self, customer: &str, text: &str);
    }

    pub struct OrderService<'a> {
        pub notifier: &'a dyn Notifier,
    }

    impl OrderService<'_> {
        pub fn confirm(&self, number: u32, customer: &str) {
            self.notifier.send(customer, &format!("order {} is confirmed", number));
        }
    }
}

// ---- module infrastructure: it uses the domain and implements its trait.
mod infrastructure {
    use crate::domain::Notifier;

    pub struct EmailNotifier;

    impl Notifier for EmailNotifier {
        fn send(&self, customer: &str, text: &str) {
            println!("email to {}: {}", customer, text);
        }
    }
}

// ---- module tests: another implementation of the same trait.
struct RecordingNotifier {
    sent: RefCell<Vec<String>>,
}

impl domain::Notifier for RecordingNotifier {
    fn send(&self, customer: &str, text: &str) {
        self.sent.borrow_mut().push(format!("{}: {}", customer, text));
    }
}

// ---- main: the only code that knows both the domain and the implementations.
fn main() {
    let email = infrastructure::EmailNotifier;
    domain::OrderService { notifier: &email }.confirm(7, "ann");

    let recorder = RecordingNotifier { sent: RefCell::new(Vec::new()) };
    domain::OrderService { notifier: &recorder }.confirm(8, "bob");
    println!("the test notifier recorded: {}", recorder.sent.borrow().join("; "));
}
```

```output
email to ann: order 7 is confirmed
the test notifier recorded: bob: order 8 is confirmed
```

- Verdict: native. The trait `Notifier` is in the module `domain`, and the module `infrastructure` uses it with `use crate::domain::Notifier`. `domain` does not use `infrastructure`. In a larger program, the two modules are two crates.
- `RecordingNotifier` keeps the messages in a `RefCell`, because `send` takes `&self`.
