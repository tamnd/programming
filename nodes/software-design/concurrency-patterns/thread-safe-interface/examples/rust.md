```rust run id=interface
use std::collections::HashMap;
use std::sync::Mutex;
use std::thread;

// The state and its implementation methods. They take `&mut self`,
// so they can run only while the caller holds the guard.
struct Inner {
    files: HashMap<String, String>,
    acquires: usize,
}

impl Inner {
    fn find_i(&mut self, path: &str) -> String {
        match self.check_cache_i(path) {
            Some(file) => file,
            None => {
                self.bind_i(path); // the lock is already held
                self.check_cache_i(path).unwrap()
            }
        }
    }

    fn check_cache_i(&self, path: &str) -> Option<String> {
        self.files.get(path).cloned()
    }

    fn bind_i(&mut self, path: &str) {
        self.files.insert(path.to_string(), format!("mapped {path}"));
    }
}

pub struct FileCache {
    lock: Mutex<Inner>,
}

impl FileCache {
    // Interface methods: take the lock, then call an implementation method.
    pub fn find(&self, path: &str) -> String {
        let mut inner = self.lock.lock().unwrap();
        inner.acquires += 1;
        inner.find_i(path)
    }

    pub fn bind(&self, path: &str) {
        let mut inner = self.lock.lock().unwrap();
        inner.acquires += 1;
        inner.bind_i(path);
    }

    fn counts(&self) -> (usize, usize) {
        let inner = self.lock.lock().unwrap();
        (inner.files.len(), inner.acquires)
    }
}

fn main() {
    let cache = FileCache { lock: Mutex::new(Inner { files: HashMap::new(), acquires: 0 }) };
    cache.bind("/index.html");
    let paths = ["/a.html", "/b.html", "/c.html"];
    thread::scope(|s| {
        for _ in 0..4 {
            s.spawn(|| {
                for i in 0..1000 {
                    cache.find(paths[i % 3]);
                }
            });
        }
    });
    println!("find /a.html: {}", cache.find("/a.html"));
    let (files, acquires) = cache.counts();
    println!("files cached: {files}");
    println!("lock acquisitions: {acquires}");
}
```

```output
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

- Verdict: idiom. The implementation methods are on `Inner`, and the only way to reach an `Inner` is through the `Mutex` guard. So the split between the two kinds of method is checked by the compiler.
