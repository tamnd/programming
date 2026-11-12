```rust run id=simple-factory
trait Notifier {
    fn name(&self) -> &'static str;
    fn send(&self, user: &str, text: &str) -> String;
}

struct EmailNotifier;
struct SmsNotifier;
struct PushNotifier;

impl Notifier for EmailNotifier {
    fn name(&self) -> &'static str { "EmailNotifier" }
    fn send(&self, user: &str, text: &str) -> String {
        format!("mail to {user}@example.com: {text}")
    }
}

impl Notifier for SmsNotifier {
    fn name(&self) -> &'static str { "SmsNotifier" }
    fn send(&self, user: &str, text: &str) -> String {
        format!("text to the phone of {user}: {text}")
    }
}

impl Notifier for PushNotifier {
    fn name(&self) -> &'static str { "PushNotifier" }
    fn send(&self, user: &str, text: &str) -> String {
        format!("push to the app of {user}: {text}")
    }
}

// The one place that knows the concrete types.
fn make_notifier(channel: &str) -> Result<Box<dyn Notifier>, String> {
    match channel {
        "email" => Ok(Box::new(EmailNotifier)),
        "sms" => Ok(Box::new(SmsNotifier)),
        "push" => Ok(Box::new(PushNotifier)),
        _ => Err(format!("unknown channel: {channel}")),
    }
}

fn main() {
    for channel in ["email", "sms", "push", "fax"] {
        match make_notifier(channel) {
            Ok(n) => println!("{}: {}", n.name(), n.send("ann", "order shipped")),
            Err(e) => println!("{e}"),
        }
    }
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A function returns `Box<dyn Notifier>`, a trait object, because the concrete type is chosen at run time. An unknown parameter is an `Err`.
- When the set of kinds is closed, an `enum` with one variant per kind and a `match` in each method is the alternative. It needs no heap allocation and no trait object.
