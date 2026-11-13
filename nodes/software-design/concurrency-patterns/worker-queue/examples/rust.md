```rust run id=worker-queue
use std::collections::{BTreeMap, HashMap, VecDeque};
use std::sync::{Arc, Condvar, Mutex};
use std::thread;

const TASKS: i32 = 10;
const WORKERS: usize = 3;
const FAILS_ON: i32 = 4; // the first worker that takes this task fails before it does the work

#[derive(Default)]
struct State {
    ready: VecDeque<i32>,
    unacked: HashMap<u32, i32>, // delivery tag -> task
    pending: i32,               // tasks put and not yet acknowledged
    tag: u32,
    deliveries: u32,
    put_back: u32,
}

// Hands out tasks and keeps each one until a worker acknowledges it.
#[derive(Default)]
struct WorkQueue {
    state: Mutex<State>,
    cond: Condvar,
}

impl WorkQueue {
    fn put(&self, task: i32) {
        let mut s = self.state.lock().unwrap();
        s.ready.push_back(task);
        s.pending += 1;
        self.cond.notify_all();
    }

    // Returns (tag, task), or None when every task is acknowledged.
    fn get(&self) -> Option<(u32, i32)> {
        let mut s = self.state.lock().unwrap();
        while s.ready.is_empty() && s.pending > 0 {
            s = self.cond.wait(s).unwrap();
        }
        let task = s.ready.pop_front()?;
        s.tag += 1;
        s.deliveries += 1;
        let tag = s.tag;
        s.unacked.insert(tag, task);
        Some((tag, task))
    }

    fn ack(&self, tag: u32) {
        let mut s = self.state.lock().unwrap();
        s.unacked.remove(&tag);
        s.pending -= 1;
        self.cond.notify_all();
    }

    // The worker went away without an ack, so the task goes back on the queue.
    fn worker_lost(&self, tag: u32) {
        let mut s = self.state.lock().unwrap();
        let task = s.unacked.remove(&tag).unwrap();
        s.ready.push_back(task);
        s.put_back += 1;
        self.cond.notify_all();
    }
}

fn main() {
    let work = Arc::new(WorkQueue::default());
    for task in 1..=TASKS {
        work.put(task);
    }

    // The results, and whether a worker has failed yet.
    let shared = Arc::new(Mutex::new((BTreeMap::new(), false)));
    let threads: Vec<_> = (0..WORKERS)
        .map(|_| {
            let work = Arc::clone(&work);
            let shared = Arc::clone(&shared);
            thread::spawn(move || {
                while let Some((tag, task)) = work.get() {
                    let fail = {
                        let mut g = shared.lock().unwrap();
                        let fail = task == FAILS_ON && !g.1;
                        if fail {
                            g.1 = true;
                        }
                        fail
                    };
                    if fail {
                        work.worker_lost(tag); // this worker stops here
                        return;
                    }
                    shared.lock().unwrap().0.insert(task, task * task); // the work
                    work.ack(tag); // only after the work is done
                }
            })
        })
        .collect();
    for t in threads {
        t.join().unwrap();
    }

    let results = &shared.lock().unwrap().0;
    let all = results.keys().copied().eq(1..=TASKS);
    let s = work.state.lock().unwrap();
    println!("tasks put on the queue: {}", TASKS);
    println!("workers: {}", WORKERS);
    println!("deliveries: {}", s.deliveries);
    println!("tasks put back after a worker failed: {}", s.put_back);
    println!("tasks still not acknowledged: {}", s.pending);
    println!("every task has a result: {}", if all { "True" } else { "False" });
}
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. A `WorkQueue` on `Mutex` and `Condvar`, shared through `Arc`, with `Option` for the delivery.
