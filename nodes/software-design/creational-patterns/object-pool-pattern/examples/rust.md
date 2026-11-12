```rust run id=pool
use std::ops::{Deref, DerefMut};
use std::sync::Mutex;

struct Buffer {
    id: usize, // a real buffer would be costly to create
    text: String,
}

struct Pool {
    idle: Mutex<Vec<Buffer>>,
    created: Mutex<usize>,
    max_idle: usize,
}

// A lease gives access to the item and returns it to the pool when dropped.
struct Lease<'a> {
    pool: &'a Pool,
    item: Option<Buffer>,
}

impl Pool {
    fn new(max_idle: usize) -> Pool {
        Pool { idle: Mutex::new(Vec::new()), created: Mutex::new(0), max_idle }
    }

    fn acquire(&self) -> Lease<'_> {
        let item = self.idle.lock().unwrap().pop().unwrap_or_else(|| {
            let mut n = self.created.lock().unwrap();
            *n += 1;
            Buffer { id: *n, text: String::new() }
        });
        Lease { pool: self, item: Some(item) }
    }

    fn release(&self, mut item: Buffer) {
        item.text.clear(); // clear the state before the next user sees it
        let mut idle = self.idle.lock().unwrap();
        if idle.len() < self.max_idle {
            idle.push(item);
        } // else: item is dropped here, the pool is full
    }
}

impl Drop for Lease<'_> {
    fn drop(&mut self) {
        if let Some(item) = self.item.take() {
            self.pool.release(item);
        }
    }
}

impl Deref for Lease<'_> {
    type Target = Buffer;
    fn deref(&self) -> &Buffer {
        self.item.as_ref().unwrap()
    }
}

impl DerefMut for Lease<'_> {
    fn deref_mut(&mut self) -> &mut Buffer {
        self.item.as_mut().unwrap()
    }
}

fn main() {
    let pool = Pool::new(2);
    for i in 1..=3 {
        let mut b = pool.acquire();
        b.text.push_str(&format!("request {i}"));
        println!("request {i} -> buffer #{}: {}", b.id, b.text);
    } // b goes back to the pool here

    {
        let held: Vec<Lease> = (0..3).map(|_| pool.acquire()).collect();
        let mut ids: Vec<usize> = held.iter().map(|b| b.id).collect();
        ids.sort();
        let ids: Vec<String> = ids.iter().map(|id| format!("#{id}")).collect();
        println!("3 at once -> buffers {}", ids.join(" "));
    } // all three go back here
    println!("idle after release: {}", pool.idle.lock().unwrap().len());
    println!("created: {}", pool.created.lock().unwrap());
}
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: idiom. The standard library has no object pool. A guard type with `Drop` returns the item when it goes out of scope, and the borrow checker stops the guard from living longer than the pool.
- The lease owns the item, so no other code can use it while it is out. After the drop, the old lease no longer exists, so use after release is a compile error.
- Crates give pools for connections and objects, and async runtimes have their own.
