```rust run id=drop
use std::sync::Mutex;

struct Handle(&'static str);

impl Drop for Handle {
    fn drop(&mut self) {
        println!("release {}", self.0);
    }
}

struct Session {
    _socket: Handle,
    _buffer: Handle,
}

fn parse(input: &str) -> Result<u32, String> {
    let _log = Handle("log");
    let value = input.parse().map_err(|_| format!("not a number: {input}"))?;
    Ok(value)
}

fn main() {
    {
        let _first = Handle("first");
        let _second = Handle("second");
    }
    {
        let _session = Session {
            _socket: Handle("socket"),
            _buffer: Handle("buffer"),
        };
    }
    println!("{:?}", parse("x"));

    let early = Handle("early");
    drop(early);
    println!("after drop");

    let counter = Mutex::new(0);
    {
        let mut guard = counter.lock().unwrap();
        *guard += 1;
    }
    println!("unlocked after scope: {}", counter.try_lock().is_ok());
}
```

```output
release second
release first
release socket
release buffer
release log
Err("not a number: x")
release early
after drop
unlocked after scope: true
```

- Verdict: native. The compiler drops each value at the end of its scope. Local variables drop in the reverse order of declaration.
- The fields of `Session` drop in declaration order, so `socket` is released before `buffer`.
- The `?` operator returns early from `parse`, and `_log` is still dropped.
- `drop(early)` ends a value before its scope does. The `MutexGuard` unlocks the mutex when its block ends, so `try_lock` succeeds.
