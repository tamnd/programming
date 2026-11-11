```rust run id=rc-parent
use std::cell::RefCell;
use std::collections::HashMap;
use std::rc::Rc;

#[derive(Clone)]
enum Slot {
    Int(i64),
    Method(Rc<dyn Fn(&Obj) -> String>),
}

// An object: shared, mutable slots and an optional parent.
#[derive(Clone)]
struct Obj(Rc<Inner>);

struct Inner {
    parent: Option<Obj>,
    slots: RefCell<HashMap<String, Slot>>,
}

impl Obj {
    fn new(parent: Option<&Obj>) -> Obj {
        Obj(Rc::new(Inner { parent: parent.cloned(), slots: RefCell::new(HashMap::new()) }))
    }

    fn set(&self, name: &str, v: Slot) -> &Obj {
        self.0.slots.borrow_mut().insert(name.to_string(), v);
        self
    }

    fn get(&self, name: &str) -> Slot {
        let mut o = Some(self);
        while let Some(x) = o {
            if let Some(v) = x.0.slots.borrow().get(name) {
                return v.clone();
            }
            o = x.0.parent.as_ref();
        }
        panic!("no slot {name}")
    }

    fn int(&self, name: &str) -> i64 {
        match self.get(name) {
            Slot::Int(n) => n,
            Slot::Method(_) => panic!("{name} is a method"),
        }
    }

    fn send(&self, name: &str) -> String {
        match self.get(name) {
            Slot::Method(f) => f(self),
            Slot::Int(_) => panic!("{name} is not a method"),
        }
    }
}

fn main() {
    let traits = Obj::new(None);
    traits.set("describe", Slot::Method(Rc::new(|o| format!("({}, {})", o.int("x"), o.int("y")))));
    let origin = Obj::new(Some(&traits));
    origin.set("x", Slot::Int(0)).set("y", Slot::Int(0));
    let p = Obj::new(Some(&origin));
    p.set("x", Slot::Int(3)); // p delegates to origin

    println!("{} {}", origin.send("describe"), p.send("describe"));
    traits.set("describe", Slot::Method(Rc::new(|o| format!("<{} {}>", o.int("x"), o.int("y")))));
    println!("{} {}", origin.send("describe"), p.send("describe"));
}
```

```output for=rc-parent
(0, 0) (3, 0)
<0 0> <3 0>
```

- Verdict: absent. Rust types and their methods are fixed at compile time, and the language has no inheritance between structs. A prototype object needs shared ownership of the parent, `Rc`, and slots that can change after the object is shared, `RefCell`, with an enum for the kinds of value a slot holds. The run block builds this, and `Clone` on `Obj` copies only the pointer, so clones of the handle see the same object.
- Ordinary Rust code gets the reuse that prototypes give through traits with default methods and through composition, which the compiler checks.
