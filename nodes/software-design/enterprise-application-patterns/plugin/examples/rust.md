```rust run id=key-generator-plugin
use std::collections::HashMap;

trait KeyGenerator {
    fn next_key(&mut self) -> i64;
}

// For unit tests: a counter in memory.
struct CounterKeyGenerator {
    last: i64,
}

impl KeyGenerator for CounterKeyGenerator {
    fn next_key(&mut self) -> i64 {
        self.last += 1;
        self.last
    }
}

// For production: it stands for a database sequence.
struct SequenceKeyGenerator {
    last: i64,
}

impl KeyGenerator for SequenceKeyGenerator {
    fn next_key(&mut self) -> i64 {
        self.last += 1;
        self.last
    }
}

type Maker = fn() -> Box<dyn KeyGenerator>;

// Rust has no class loading by name, so the program keeps a table of named constructors.
fn plugins() -> HashMap<&'static str, Maker> {
    let mut m: HashMap<&'static str, Maker> = HashMap::new();
    m.insert("CounterKeyGenerator", || Box::new(CounterKeyGenerator { last: 0 }));
    m.insert("SequenceKeyGenerator", || Box::new(SequenceKeyGenerator { last: 1000 }));
    m
}

struct PluginFactory<'a> {
    config: HashMap<&'a str, &'a str>,
}

impl<'a> PluginFactory<'a> {
    fn get(&self, iface: &str) -> Result<(Box<dyn KeyGenerator>, &'a str), String> {
        let name = self.config[iface];
        match plugins().get(name) {
            Some(make) => Ok((make(), name)),
            None => Err(format!("no plugin named {}", name)),
        }
    }
}

fn main() {
    // One configuration for each deployment. Only the configuration names the types.
    let configs = [
        ("test", "CounterKeyGenerator"),
        ("production", "SequenceKeyGenerator"),
        ("broken", "FileKeyGenerator"),
    ];
    for (name, class) in configs {
        let factory = PluginFactory { config: HashMap::from([("KeyGenerator", class)]) };
        match factory.get("KeyGenerator") {
            Err(e) => println!("{}: error: {}", name, e),
            Ok((mut keygen, plugin)) => {
                let keys: Vec<String> = (0..3).map(|_| keygen.next_key().to_string()).collect();
                println!("{}: keys {} from {}", name, keys.join(", "), plugin);
            }
        }
    }
}
```

```output
test: keys 1, 2, 3 from CounterKeyGenerator
production: keys 1001, 1002, 1003 from SequenceKeyGenerator
broken: error: no plugin named FileKeyGenerator
```

- Verdict: idiom. Rust has no class loading by name, so the program keeps a table of named constructors, and the configuration chooses one. Cargo features or a dynamic library are the other choices.
- `Maker` is a function pointer type. Closures with no captures convert to it.
