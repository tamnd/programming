```rust run id=thread-pool
use std::collections::HashSet;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::mpsc;
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;

type Job = Box<dyn FnOnce(usize) + Send + 'static>;

struct ThreadPool {
    workers: Vec<thread::JoinHandle<()>>,
    sender: Option<mpsc::Sender<Job>>,
}

impl ThreadPool {
    fn new(size: usize) -> ThreadPool {
        let (sender, receiver) = mpsc::channel::<Job>();
        let receiver = Arc::new(Mutex::new(receiver));
        let workers = (0..size)
            .map(|id| {
                let receiver = Arc::clone(&receiver);
                thread::spawn(move || loop {
                    let message = receiver.lock().unwrap().recv();
                    match message {
                        Ok(job) => job(id),
                        Err(_) => break, // the pool was dropped
                    }
                })
            })
            .collect();
        ThreadPool { workers, sender: Some(sender) }
    }

    fn execute<F: FnOnce(usize) + Send + 'static>(&self, f: F) {
        self.sender.as_ref().unwrap().send(Box::new(f)).unwrap();
    }
}

impl Drop for ThreadPool {
    fn drop(&mut self) {
        drop(self.sender.take());
        for worker in self.workers.drain(..) {
            worker.join().unwrap();
        }
    }
}

fn main() {
    let running = Arc::new(AtomicUsize::new(0));
    let most_running = Arc::new(AtomicUsize::new(0));
    let workers_used = Arc::new(Mutex::new(HashSet::new()));
    let (results_tx, results_rx) = mpsc::channel();
    {
        let pool = ThreadPool::new(4);
        for n in 0..100u64 {
            let (running, most_running) = (Arc::clone(&running), Arc::clone(&most_running));
            let workers_used = Arc::clone(&workers_used);
            let results_tx = results_tx.clone();
            pool.execute(move |id| {
                let now = running.fetch_add(1, Ordering::SeqCst) + 1;
                most_running.fetch_max(now, Ordering::SeqCst);
                workers_used.lock().unwrap().insert(id);
                thread::sleep(Duration::from_millis(1));
                running.fetch_sub(1, Ordering::SeqCst);
                results_tx.send(n * n).unwrap();
            });
        }
    } // dropping the pool waits for the workers
    drop(results_tx);
    let results: Vec<u64> = results_rx.iter().collect();
    let cap = |b: bool| if b { "True" } else { "False" };
    println!("tasks run: {}", results.len());
    println!("sum of results: {}", results.iter().sum::<u64>());
    println!("workers used, at most 4: {}", cap(workers_used.lock().unwrap().len() <= 4));
    println!("most tasks running at once, at most 4: {}", cap(most_running.load(Ordering::SeqCst) <= 4));
}
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: idiom. `ThreadPool` follows the Rust book: workers share the receiving end of an `mpsc` channel behind `Arc<Mutex<_>>`, and dropping the pool closes the channel and joins the workers.
