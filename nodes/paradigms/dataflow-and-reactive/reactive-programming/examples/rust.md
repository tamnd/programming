```rust run id=signal
use std::cell::RefCell;
use std::rc::Rc;

struct Signal<T> {
    value: RefCell<T>,
    subscribers: RefCell<Vec<Box<dyn Fn(&T)>>>,
}

impl<T: Clone + 'static> Signal<T> {
    fn new(value: T) -> Rc<Self> {
        Rc::new(Signal { value: RefCell::new(value), subscribers: RefCell::new(Vec::new()) })
    }

    fn get(&self) -> T {
        self.value.borrow().clone()
    }

    fn set(&self, v: T) {
        *self.value.borrow_mut() = v.clone();
        for s in self.subscribers.borrow().iter() {
            s(&v);
        }
    }

    fn subscribe(&self, f: impl Fn(&T) + 'static) {
        self.subscribers.borrow_mut().push(Box::new(f));
    }

    fn map<U: Clone + 'static>(self: &Rc<Self>, f: impl Fn(&T) -> U + 'static) -> Rc<Signal<U>> {
        let out = Signal::new(f(&self.get()));
        let target = Rc::clone(&out);
        self.subscribe(move |v| target.set(f(v)));
        out
    }
}

fn main() {
    let celsius = Signal::new(20.0_f64);
    let fahrenheit = celsius.map(|c| c * 9.0 / 5.0 + 32.0);
    let label = fahrenheit.map(|f| format!("{f:.1} F"));
    label.subscribe(|s| println!("display: {s}"));

    println!("start: {}", label.get());
    celsius.set(25.0);
    celsius.set(-40.0);
}
```

```output for=signal
start: 68.0 F
display: 77.0 F
display: -40.0 F
```

- Verdict: idiom. The standard library has no observables or signals. Iterators are pull-based dataflow, and `Rc` with `RefCell` lets callbacks share a value on one thread, as here. A subscriber that calls `set` on the same signal would panic, because `subscribers` is already borrowed. Crates such as `futures` give streams, and user interface crates such as Leptos and Dioxus give signals.
