```rust run id=spawner
use std::collections::BTreeMap;

trait Prototype {
    fn clone_box(&self) -> Box<dyn Prototype>;
    fn describe(&self) -> String;
    fn add_item(&mut self, _item: &str) {}
}

#[derive(Clone)]
struct Monster {
    name: String,
    health: u32,
    items: Vec<String>,
}

#[derive(Clone)]
struct Trap {
    name: String,
    damage: u32,
}

impl Prototype for Monster {
    fn clone_box(&self) -> Box<dyn Prototype> {
        Box::new(self.clone()) // the derived Clone clones the Vec and each String
    }
    fn describe(&self) -> String {
        format!("{}: health {}, items {}", self.name, self.health, self.items.join(", "))
    }
    fn add_item(&mut self, item: &str) {
        self.items.push(item.to_string());
    }
}

impl Prototype for Trap {
    fn clone_box(&self) -> Box<dyn Prototype> {
        Box::new(self.clone())
    }
    fn describe(&self) -> String {
        format!("{}: damage {}", self.name, self.damage)
    }
}

fn main() {
    let mut registry: BTreeMap<&str, Box<dyn Prototype>> = BTreeMap::new();
    registry.insert(
        "goblin",
        Box::new(Monster { name: "goblin".into(), health: 30, items: vec!["dagger".into()] }),
    );
    registry.insert("spikes", Box::new(Trap { name: "spikes".into(), damage: 5 }));
    let spawn = |key: &str| registry[key].clone_box();

    let mut first = spawn("goblin");
    first.add_item("torch");
    let second = spawn("goblin");
    let trap = spawn("spikes");
    for thing in [&first, &second, &trap] {
        println!("{}", thing.describe());
    }
    println!("prototype {}", registry["goblin"].describe());
}
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
```

- Verdict: native for values, idiom for trait objects. `#[derive(Clone)]` writes a `clone` that clones each field, and `Vec<String>` clones its elements, so the copy owns all its data.
- `Clone` cannot be a supertrait of a trait that is used as `dyn Prototype`. `Clone` requires `Sized`, and a trait with `Sized` as a supertrait is not dyn compatible. The `clone_box` method returns a `Box<dyn Prototype>` instead. The node page shows the compile error.
- A deep copy is the default only for owned data. A field of type `Rc<T>` or `Arc<T>` clones the pointer and shares the value, so a derived `Clone` on such a struct shares that state with the original.
- The borrow checker stops the shared-list bug of other languages: two owners cannot both change one `Vec` without `RefCell` or a lock.
