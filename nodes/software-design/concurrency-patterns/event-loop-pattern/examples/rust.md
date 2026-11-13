```rust run id=event-loop-pattern
use std::cell::{Cell, RefCell};
use std::cmp::Reverse;
use std::collections::{BinaryHeap, HashMap, VecDeque};
use std::rc::Rc;

type Callback = Box<dyn FnOnce()>;

// One thread takes ready callbacks from a queue and runs each one to the end.
#[derive(Default)]
struct EventLoop {
    ready: RefCell<VecDeque<Callback>>, // callbacks to run in this turn or the next
    timers: RefCell<BinaryHeap<Reverse<(u32, u32)>>>, // (due time, order)
    timer_callbacks: RefCell<HashMap<u32, Callback>>, // order -> callback
    now: Cell<u32>, // a counter clock, so the output does not depend on the machine
    order: Cell<u32>,
    turns: Cell<u32>,
}

impl EventLoop {
    fn call_soon(&self, callback: Callback) {
        self.ready.borrow_mut().push_back(callback);
    }

    fn call_later(&self, delay: u32, callback: Callback) {
        let order = self.order.get() + 1;
        self.order.set(order);
        self.timers.borrow_mut().push(Reverse((self.now.get() + delay, order)));
        self.timer_callbacks.borrow_mut().insert(order, callback);
    }

    fn run(&self) {
        while !self.ready.borrow().is_empty() || !self.timers.borrow().is_empty() {
            if self.ready.borrow().is_empty() {
                let Reverse((due, _)) = *self.timers.borrow().peek().unwrap();
                self.now.set(due); // a real loop sleeps or waits for I/O here
            }
            loop {
                let next = self.timers.borrow().peek().copied();
                match next {
                    Some(Reverse((due, order))) if due <= self.now.get() => {
                        self.timers.borrow_mut().pop();
                        let callback = self.timer_callbacks.borrow_mut().remove(&order).unwrap();
                        self.ready.borrow_mut().push_back(callback);
                    }
                    _ => break,
                }
            }
            self.turns.set(self.turns.get() + 1);
            let count = self.ready.borrow().len(); // callbacks added now wait for the next turn
            for _ in 0..count {
                let callback = self.ready.borrow_mut().pop_front().unwrap();
                callback(); // no borrow is held here, so the callback can add callbacks
            }
        }
    }
}

fn main() {
    let event_loop = Rc::new(EventLoop::default());
    let log = Rc::new(RefCell::new(Vec::<String>::new()));
    let say = {
        let (event_loop, log) = (Rc::clone(&event_loop), Rc::clone(&log));
        move |text: &str| -> Callback {
            let (event_loop, log, text) = (Rc::clone(&event_loop), Rc::clone(&log), text.to_string());
            Box::new(move || log.borrow_mut().push(format!("time {}: {}", event_loop.now.get(), text)))
        }
    };

    event_loop.call_later(20, say("timer of 20"));
    event_loop.call_later(10, say("timer of 10"));
    let first = {
        let (event_loop, say) = (Rc::clone(&event_loop), say.clone());
        move || {
            say("first callback")();
            event_loop.call_soon(say("callback that the first callback added"));
        }
    };
    event_loop.call_soon(Box::new(first));
    event_loop.call_soon(say("second callback"));
    event_loop.run();

    println!("order of the callbacks:");
    for line in log.borrow().iter() {
        println!("  {}", line);
    }
    println!("turns of the loop: {}", event_loop.turns.get());
}
```

```output
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

- Verdict: idiom. A small loop with `RefCell` and `Cell` fields shared through `Rc`, so a callback can add callbacks; no borrow is held while a callback runs.
