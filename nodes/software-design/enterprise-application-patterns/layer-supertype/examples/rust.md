```rust run id=domain-object-supertype
// Rust has no inheritance. The layer supertype is a trait with default methods,
// which every domain type of the layer implements.
trait DomainObject {
    const KIND: &'static str;
    fn id(&self) -> u32;
    fn name(&self) -> &str;
    fn rules(&self) -> Vec<(bool, &'static str)>; // each type gives its own rules

    fn check(&self) -> String {
        let problems: Vec<&str> = self.rules().into_iter().filter(|(ok, _)| !ok).map(|(_, m)| m).collect();
        if problems.is_empty() { "valid".to_string() } else { format!("invalid: {}", problems.join(", ")) }
    }

    fn same<O: DomainObject>(&self, other: &O) -> bool {
        Self::KIND == O::KIND && self.id() == other.id()
    }

    fn describe(&self) -> String {
        let name = if self.name().is_empty() { "(no name)" } else { self.name() };
        format!("{} {} {}: {}", Self::KIND, self.id(), name, self.check())
    }
}

struct Customer {
    id: u32,
    name: String,
}

impl DomainObject for Customer {
    const KIND: &'static str = "customer";
    fn id(&self) -> u32 { self.id }
    fn name(&self) -> &str { &self.name }
    fn rules(&self) -> Vec<(bool, &'static str)> { vec![(!self.name.is_empty(), "name is empty")] }
}

struct Product {
    id: u32,
    name: String,
    price_cents: i64,
}

impl DomainObject for Product {
    const KIND: &'static str = "product";
    fn id(&self) -> u32 { self.id }
    fn name(&self) -> &str { &self.name }
    fn rules(&self) -> Vec<(bool, &'static str)> {
        vec![(!self.name.is_empty(), "name is empty"), (self.price_cents >= 0, "price is negative")]
    }
}

fn customer(id: u32, name: &str) -> Customer { Customer { id, name: name.to_string() } }
fn product(id: u32, name: &str, price_cents: i64) -> Product { Product { id, name: name.to_string(), price_cents } }
fn yes_no(b: bool) -> &'static str { if b { "yes" } else { "no" } }

fn main() {
    println!("{}", customer(1, "ann").describe());
    println!("{}", product(2, "pen", 150).describe());
    println!("{}", product(3, "", -5).describe());
    println!("customer 1 equals customer 1 loaded again: {}", yes_no(customer(1, "ann").same(&customer(1, "ann"))));
    println!("customer 1 equals product 1: {}", yes_no(customer(1, "ann").same(&product(1, "ann", 0))));
}
```

```output
customer 1 ann: valid
product 2 pen: valid
product 3 (no name): invalid: name is empty, price is negative
customer 1 equals customer 1 loaded again: yes
customer 1 equals product 1: no
```

- Verdict: idiom. Rust has no inheritance. The layer supertype is the trait `DomainObject`, with the associated constant `KIND`, required methods for the data of each type, and default methods `check`, `same` and `describe` for the shared behaviour.
- A trait cannot hold fields, so each type keeps its own `id` field and returns it from `id`.
