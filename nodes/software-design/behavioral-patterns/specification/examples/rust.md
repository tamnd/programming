```rust run id=containers
struct Container {
    name: &'static str,
    refrigerated: bool,
    clean: bool,
}

// A composite specification as a tree: leaves are tests, other nodes are operators.
enum Spec {
    Refrigerated,
    Clean,
    And(Box<Spec>, Box<Spec>),
    Or(Box<Spec>, Box<Spec>),
    Not(Box<Spec>),
}

impl Spec {
    fn is_satisfied_by(&self, c: &Container) -> bool {
        match self {
            Spec::Refrigerated => c.refrigerated,
            Spec::Clean => c.clean,
            Spec::And(a, b) => a.is_satisfied_by(c) && b.is_satisfied_by(c),
            Spec::Or(a, b) => a.is_satisfied_by(c) || b.is_satisfied_by(c),
            Spec::Not(a) => !a.is_satisfied_by(c),
        }
    }
    fn and(self, other: Spec) -> Spec {
        Spec::And(Box::new(self), Box::new(other))
    }
    fn or(self, other: Spec) -> Spec {
        Spec::Or(Box::new(self), Box::new(other))
    }
    fn not(self) -> Spec {
        Spec::Not(Box::new(self))
    }
}

fn select(containers: &[Container], spec: &Spec) -> String {
    let names: Vec<&str> = containers.iter().filter(|c| spec.is_satisfied_by(c)).map(|c| c.name).collect();
    names.join(", ")
}

fn main() {
    let containers = [
        Container { name: "c1", refrigerated: true, clean: true },
        Container { name: "c2", refrigerated: true, clean: false },
        Container { name: "c3", refrigerated: false, clean: true },
    ];
    println!("meat: {}", select(&containers, &Spec::Refrigerated.and(Spec::Clean)));
    println!("not refrigerated: {}", select(&containers, &Spec::Refrigerated.not()));
    println!("refrigerated or clean: {}", select(&containers, &Spec::Refrigerated.or(Spec::Clean)));
}
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. An enum holds the leaf tests and the composite nodes, and one `match` interprets the tree. This is the Composite Specification form.
