```rust run id=null-logger
trait Logger {
    fn log(&self, message: &str);
}

struct ConsoleLogger;

impl Logger for ConsoleLogger {
    fn log(&self, message: &str) {
        println!("log: {message}");
    }
}

struct NullLogger;

impl Logger for NullLogger {
    fn log(&self, _message: &str) {} // do nothing
}

struct Checkout<'a> {
    logger: &'a dyn Logger,
}

impl Checkout<'_> {
    fn pay(&self, amount: u32) -> String {
        self.logger.log(&format!("paid {amount}"));
        format!("receipt: {amount}")
    }
}

fn main() {
    println!("{}", Checkout { logger: &ConsoleLogger }.pay(10));
    println!("{}", Checkout { logger: &NullLogger }.pay(20));
}
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. `NullLogger` is a unit struct that implements the `Logger` trait with an empty method. The other choice is a field of type `Option<&dyn Logger>`.
