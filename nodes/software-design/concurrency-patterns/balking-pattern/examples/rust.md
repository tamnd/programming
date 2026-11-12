```rust run id=balking-pattern
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Barrier, Mutex};
use std::thread;

#[derive(Default)]
struct State {
    changed: bool,
    saving: bool,
    saves: usize,
}

struct Document {
    state: Mutex<State>,
    write: Box<dyn Fn() + Send + Sync>, // the slow part of a save
}

impl Document {
    fn edit(&self) {
        self.state.lock().unwrap().changed = true;
    }

    /// Saves the document. Returns false at once if a save cannot start now.
    fn save(&self) -> bool {
        {
            let mut s = self.state.lock().unwrap();
            if s.saving || !s.changed {
                return false; // balk: the object is not in the right state
            }
            s.saving = true;
            s.changed = false;
        }
        (self.write)();
        let mut s = self.state.lock().unwrap();
        s.saving = false;
        s.saves += 1;
        true
    }

    fn saves(&self) -> usize {
        self.state.lock().unwrap().saves
    }
}

fn main() {
    // The first save waits at the start barrier and then at the release barrier.
    let start = Arc::new(Barrier::new(2));
    let release = Arc::new(Barrier::new(2));
    let writes = Arc::new(AtomicUsize::new(0));
    let doc = {
        let (start, release, writes) = (Arc::clone(&start), Arc::clone(&release), Arc::clone(&writes));
        Arc::new(Document {
            state: Mutex::new(State::default()),
            write: Box::new(move || {
                if writes.fetch_add(1, Ordering::SeqCst) == 0 {
                    start.wait();
                    release.wait(); // stay in progress until the other calls end
                }
            }),
        })
    };
    doc.edit();
    let first = {
        let doc = Arc::clone(&doc);
        thread::spawn(move || doc.save())
    };
    start.wait();

    let callers: Vec<_> = (0..7)
        .map(|_| {
            let doc = Arc::clone(&doc);
            thread::spawn(move || doc.save())
        })
        .collect();
    let results: Vec<bool> = callers.into_iter().map(|t| t.join().unwrap()).collect();
    release.wait();
    first.join().unwrap();

    let word = |ran: bool| if ran { "ran" } else { "balked" };
    println!("calls during a save: {}", results.len());
    println!("calls that balked: {}", results.iter().filter(|r| !**r).count());
    println!("saves run: {}", doc.saves());
    println!("save with no changes: {}", word(doc.save()));
    doc.edit();
    println!("save after an edit: {}", word(doc.save()));
    println!("saves run: {}", doc.saves());
}
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: stdlib. A `Mutex<State>` makes the test and the change of the `saving` flag one step.
