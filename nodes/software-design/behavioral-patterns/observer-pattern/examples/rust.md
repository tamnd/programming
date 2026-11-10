```rust run id=sensor
use std::cell::RefCell;
use std::rc::{Rc, Weak};

trait Observer {
    fn update(&self, value: f64);
}

#[derive(Default)]
struct Sensor {
    observers: RefCell<Vec<Weak<dyn Observer>>>,
}

impl Sensor {
    fn subscribe(&self, observer: &Rc<dyn Observer>) {
        self.observers.borrow_mut().push(Rc::downgrade(observer));
    }

    fn set(&self, value: f64) {
        // Upgrade the live observers first, and drop the dead entries.
        let live: Vec<Rc<dyn Observer>> = {
            let mut observers = self.observers.borrow_mut();
            observers.retain(|weak| weak.strong_count() > 0);
            observers.iter().filter_map(Weak::upgrade).collect()
        };
        for observer in live {
            observer.update(value);
        }
    }

    fn count(&self) -> usize {
        self.observers.borrow().len()
    }
}

struct Display;
struct Alarm;

impl Observer for Display {
    fn update(&self, value: f64) {
        println!("display: {value:.1}");
    }
}

impl Observer for Alarm {
    fn update(&self, value: f64) {
        if value > 30.0 {
            println!("alarm: {value:.1} is above 30");
        }
    }
}

fn main() {
    let sensor = Sensor::default();
    let display: Rc<dyn Observer> = Rc::new(Display);
    let alarm: Rc<dyn Observer> = Rc::new(Alarm);
    sensor.subscribe(&display);
    sensor.subscribe(&alarm);
    sensor.set(21.5);
    sensor.set(30.5);
    drop(display);
    sensor.set(31.0);
    println!("observers: {}", sensor.count());
}
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
observers: 1
```

- Verdict: idiom. Rust has no observer type in the standard library. A list of trait objects or of boxed closures is the usual form.
- The subject holds `Weak` pointers, so it does not keep its observers alive. `drop(display)` frees the display, and the next `set` removes its dead entry. This is the subscription model of the example: an observer lives as long as its owner keeps the `Rc`.
- `set` copies the live observers out of the `RefCell` before it calls them. If it held the borrow during the calls, an observer that subscribes during a notification would make `borrow_mut` panic at run time.
- Threads need `Arc`, `Mutex` and `Send` observers in place of `Rc` and `RefCell`. Channels from `std::sync::mpsc` give the asynchronous form.
