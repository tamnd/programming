```rust run id=invoices
use std::cell::RefCell;

trait Clock {
    fn today(&self) -> i32;
}

trait Mailer {
    fn send(&self, to: &str, text: &str);
}

struct Invoice {
    number: u32,
    customer: &'static str,
    due: i32,
}

// Generic over its collaborators: the compiler makes one copy for each pair of types.
struct ReminderService<C: Clock, M: Mailer> {
    clock: C,
    mailer: M,
}

impl<C: Clock, M: Mailer> ReminderService<C, M> {
    fn new(clock: C, mailer: M) -> Self {
        Self { clock, mailer }
    }

    fn remind(&self, invoice: &Invoice) -> bool {
        let late = self.clock.today() - invoice.due;
        if late <= 0 {
            return false;
        }
        let text = format!("invoice {} is {late} days overdue", invoice.number);
        self.mailer.send(invoice.customer, &text);
        true
    }
}

struct FixedClock(i32);

impl Clock for FixedClock {
    fn today(&self) -> i32 {
        self.0
    }
}

struct ConsoleMailer;

impl Mailer for ConsoleMailer {
    fn send(&self, to: &str, text: &str) {
        println!("to {to}: {text}");
    }
}

#[derive(Default)]
struct RecordingMailer {
    sent: RefCell<Vec<String>>,
}

impl Mailer for &RecordingMailer {
    fn send(&self, to: &str, text: &str) {
        self.sent.borrow_mut().push(format!("to {to}: {text}"));
    }
}

fn main() {
    // The composition root.
    let service = ReminderService::new(FixedClock(40), ConsoleMailer);
    let invoices = [
        Invoice { number: 17, customer: "ana@example.com", due: 31 },
        Invoice { number: 18, customer: "bo@example.com", due: 45 },
    ];
    let sent = invoices.iter().filter(|i| service.remind(i)).count();
    println!("reminders sent: {sent}");

    // The test keeps the recorder and lends it to the service.
    let recorder = RecordingMailer::default();
    ReminderService::new(FixedClock(50), &recorder).remind(&invoices[1]);
    println!("recorded: {}", recorder.sent.borrow()[0]);
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
```

- Verdict: idiom. Traits describe the roles. The service is generic over them, so each call to a collaborator is a static call that the compiler can inline. A `Box<dyn Mailer>` field is the other choice: one service type, and the implementation chosen at run time.
- `send` takes `&self`, so the recording double keeps its list in a `RefCell` to change it through a shared reference.
- The test lends `&recorder` to the service and reads the list afterwards. `impl Mailer for &RecordingMailer` makes the reference itself a mailer, so the service does not take ownership of the recorder.
