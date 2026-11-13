```rust run id=scheduler-pattern
use std::sync::atomic::{AtomicI32, Ordering};
use std::sync::{Condvar, Mutex};
use std::thread;
use std::time::Duration;

struct State {
    busy: bool,
    waiting: Vec<(i32, i32, String)>, // entries (priority, arrival, name)
    arrivals: i32,
    chosen: i32, // the arrival number of the chosen entry
}

// Decides which waiting thread uses the printer next.
struct Scheduler {
    state: Mutex<State>,
    cond: Condvar,
}

impl Scheduler {
    fn new() -> Self {
        Scheduler {
            state: Mutex::new(State { busy: false, waiting: Vec::new(), arrivals: 0, chosen: 0 }),
            cond: Condvar::new(),
        }
    }

    fn enter(&self, name: &str, priority: i32) {
        let mut s = self.state.lock().unwrap();
        if !s.busy && s.waiting.is_empty() {
            s.busy = true;
            return;
        }
        s.arrivals += 1;
        let arrival = s.arrivals;
        s.waiting.push((priority, arrival, name.to_string()));
        self.cond.notify_all();
        // Sleep until the scheduler chooses this thread.
        let mut s = self.cond.wait_while(s, |s| s.chosen != arrival).unwrap();
        s.chosen = 0;
    }

    fn done(&self) {
        let mut s = self.state.lock().unwrap();
        if s.waiting.is_empty() {
            s.busy = false;
            return;
        }
        // The policy: the lowest priority number first, then the first to arrive.
        let best = (0..s.waiting.len()).min_by_key(|&i| (s.waiting[i].0, s.waiting[i].1)).unwrap();
        let entry = s.waiting.remove(best);
        s.chosen = entry.1; // the printer stays busy and passes to that thread
        self.cond.notify_all();
    }

    fn wait_until_waiting(&self, n: usize) -> usize {
        let s = self.state.lock().unwrap();
        let (s, _) = self.cond.wait_timeout_while(s, Duration::from_secs(5), |s| s.waiting.len() < n).unwrap();
        s.waiting.len()
    }
}

static ON_PRINTER: AtomicI32 = AtomicI32::new(0);
static ORDER: Mutex<(Vec<String>, i32)> = Mutex::new((Vec::new(), 0)); // (order, most on printer)

fn use_printer(name: &str) {
    let now = ON_PRINTER.fetch_add(1, Ordering::SeqCst) + 1;
    {
        let mut o = ORDER.lock().unwrap();
        o.1 = o.1.max(now);
        o.0.push(name.to_string());
    }
    ON_PRINTER.fetch_sub(1, Ordering::SeqCst);
}

fn main() {
    let scheduler = Scheduler::new();
    scheduler.enter("first", 0); // the first job takes the free printer
    use_printer("first");
    let jobs = [("report", 2), ("invoice", 1), ("photo", 3), ("label", 1)];
    thread::scope(|scope| {
        for (i, &(name, priority)) in jobs.iter().enumerate() {
            let scheduler = &scheduler;
            scope.spawn(move || {
                scheduler.enter(name, priority);
                use_printer(name);
                scheduler.done();
            });
            scheduler.wait_until_waiting(i + 1); // start the jobs in a known order
        }
        println!("jobs waiting when the first job ends: {}", scheduler.wait_until_waiting(jobs.len()));
        scheduler.done();
    });

    let o = ORDER.lock().unwrap();
    println!("print order: {}", o.0.join(", "));
    println!("jobs on the printer at once, at most: {}", o.1);
}
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A struct with `Mutex` and `Condvar`; `enter` uses `wait_while` until `done` chooses its arrival number.
