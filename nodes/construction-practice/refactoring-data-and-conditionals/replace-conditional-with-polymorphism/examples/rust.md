```rust run id=each-case-its-code
// The functional form: the cases are the variants of one enum, and each
// operation is one function with a match. A match that misses a variant
// does not compile.
enum Message {
    Email,
    Sms { text: String, urgent: bool },
    Push { urgent: bool },
}

fn cost(m: &Message) -> usize {
    match m {
        Message::Email => 0,
        Message::Sms { text, .. } => text.len().div_ceil(160) * 7,
        Message::Push { urgent } => usize::from(*urgent),
    }
}

fn delay_seconds(m: &Message) -> u32 {
    match m {
        Message::Email => 60,
        Message::Sms { urgent: true, .. } | Message::Push { .. } => 0,
        Message::Sms { urgent: false, .. } => 30,
    }
}

fn main() {
    let email = Message::Email;
    let sms = Message::Sms {
        text: "x".repeat(161),
        urgent: false,
    };
    let push = Message::Push { urgent: true };
    assert_eq!(cost(&email), 0);
    assert_eq!(cost(&sms), 14);
    assert_eq!(cost(&push), 1);
    assert_eq!(delay_seconds(&email), 60);
    assert_eq!(delay_seconds(&sms), 30);
    assert_eq!(delay_seconds(&push), 0);
    println!("replace conditional with polymorphism: tests pass");
}
```

```output
replace conditional with polymorphism: tests pass
```
