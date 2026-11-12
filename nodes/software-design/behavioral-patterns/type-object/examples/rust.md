```rust run id=breeds
struct Breed {
    name: String,
    health: u32,
    attack: String,
}

impl Breed {
    fn new_monster(&self) -> Monster<'_> {
        Monster { breed: self, health: self.health }
    }
}

struct Monster<'a> {
    breed: &'a Breed, // the breed must live longer than the monster
    health: u32,      // each monster has its own health
}

impl Monster<'_> {
    fn describe(&self) -> String {
        format!("{}: {} health, attacks with {}", self.breed.name, self.health, self.breed.attack)
    }
}

fn main() {
    // New kinds are data, for example from a file.
    let table = [("goblin", 20, "a dagger"), ("dragon", 200, "fire"), ("wyvern", 120, "its tail")];
    let breeds: Vec<Breed> = table
        .iter()
        .map(|&(n, h, a)| Breed { name: n.to_string(), health: h, attack: a.to_string() })
        .collect();
    for b in &breeds {
        println!("{}", b.new_monster().describe());
    }
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A struct with one value for each kind. The monster borrows its breed.
