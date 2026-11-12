```rust run id=strategy
use std::cell::RefCell;
use std::collections::HashMap;
use std::sync::Mutex;
use std::thread;

type Files = HashMap<String, String>;

// The locking strategy: run a closure with access to the guarded data.
trait Strategy<T> {
    fn with<R>(&self, f: impl FnOnce(&mut T) -> R) -> R;
}

// A lock for one thread: no locking, only a run-time borrow check.
struct NullMutex<T>(RefCell<T>);

impl<T> Strategy<T> for NullMutex<T> {
    fn with<R>(&self, f: impl FnOnce(&mut T) -> R) -> R {
        f(&mut self.0.borrow_mut())
    }
}

impl<T> Strategy<T> for Mutex<T> {
    fn with<R>(&self, f: impl FnOnce(&mut T) -> R) -> R {
        f(&mut self.lock().unwrap())
    }
}

struct FileCache<L: Strategy<(Files, usize)>> {
    lock: L, // the locking strategy is a parameter
}

impl<L: Strategy<(Files, usize)>> FileCache<L> {
    fn find(&self, path: &str) -> String {
        self.lock.with(|(files, lookups)| {
            *lookups += 1;
            files
                .entry(path.to_string())
                .or_insert_with(|| format!("mapped {path}"))
                .clone()
        })
    }

    fn counts(&self) -> (usize, usize) {
        self.lock.with(|(files, lookups)| (files.len(), *lookups))
    }
}

fn main() {
    let single = FileCache { lock: NullMutex(RefCell::new((Files::new(), 0))) };
    for path in ["/a.html", "/b.html", "/a.html"] {
        single.find(path);
    }
    let (files, lookups) = single.counts();
    println!("Null_Mutex cache: {files} files, {lookups} lookups");

    let multi = FileCache { lock: Mutex::new((Files::new(), 0)) };
    let paths = ["/a.html", "/b.html", "/c.html"];
    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for i in 0..1000 {
                    multi.find(paths[i % 3]);
                }
            });
        }
    });
    let (files, lookups) = multi.counts();
    println!("Thread_Mutex cache: {files} files, {lookups} lookups");
}
```

```output
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

- Verdict: idiom. This is the parameterized type form with a trait. `RefCell` is not `Sync`, so the compiler does not let threads share the `NullMutex` cache.
