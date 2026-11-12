```rust run id=consultants
use std::cell::RefCell;
use std::rc::Rc;

type Handler = Box<dyn Fn(&str)>;

#[derive(Default)]
struct Signal(Vec<Handler>);

impl Signal {
    fn add(&mut self, h: impl Fn(&str) + 'static) {
        self.0.push(Box::new(h));
    }
    fn fire(&self, name: &str) {
        for h in &self.0 {
            h(name);
        }
    }
}

struct Consultant {
    name: String,
    home: String,
    available_at_home: bool,
    location: String,
    location_changed: Signal,
    availability_changed: Signal,
}

impl Consultant {
    fn new(name: &str, home: &str, available_at_home: bool, location: &str) -> Self {
        Consultant {
            name: name.into(),
            home: home.into(),
            available_at_home,
            location: location.into(),
            location_changed: Signal::default(),
            availability_changed: Signal::default(),
        }
    }
    fn available_at(&self, place: &str) -> bool {
        if place == self.home { self.available_at_home } else { true }
    }
    fn move_to(&mut self, place: &str) {
        println!("{} moves to {}", self.name, place);
        if self.location != place {
            self.location_changed.fire(&self.name);
            if self.available_at(place) != self.available_at(&self.location) {
                self.availability_changed.fire(&self.name);
            }
            self.location = place.into();
        }
    }
}

#[derive(Default)]
struct EventAggregator {
    changed: Signal,
    location: Signal,
    availability: Signal,
}

fn listen(agg: &Rc<RefCell<EventAggregator>>, c: &mut Consultant) {
    let a = Rc::clone(agg);
    c.location_changed.add(move |n| {
        let a = a.borrow();
        a.location.fire(n); // the specific event
        a.changed.fire(n); // and the general one
    });
    let a = Rc::clone(agg);
    c.availability_changed.add(move |n| {
        let a = a.borrow();
        a.availability.fire(n);
        a.changed.fire(n);
    });
}

fn main() {
    let agg = Rc::new(RefCell::new(EventAggregator::default()));
    let mut alice = Consultant::new("alice", "London", false, "London");
    let mut bob = Consultant::new("bob", "Paris", true, "Lyon");
    listen(&agg, &mut alice);
    listen(&agg, &mut bob);

    agg.borrow_mut().changed.add(|n| println!("screen: {} changed", n));
    agg.borrow_mut().availability.add(|n| println!("phone: availability of {} changed", n));

    alice.move_to("Leeds");
    bob.move_to("Paris");
}
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. Vectors of boxed closures are the events. The consultants share the aggregator through `Rc<RefCell<...>>`.
