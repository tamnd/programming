```rust run id=settings
use std::sync::{LazyLock, Mutex};
use std::thread;

struct Settings {
    level: String,
}

// Initialized on first access; other threads block until it is done.
static SETTINGS: LazyLock<Mutex<Settings>> = LazyLock::new(|| {
    println!("creating the settings");
    Mutex::new(Settings {
        level: "info".to_string(),
    })
});

fn address() -> usize {
    let settings: &Mutex<Settings> = &SETTINGS;
    settings as *const Mutex<Settings> as usize
}

fn main() {
    println!("main: {}", SETTINGS.lock().unwrap().level);
    let worker = thread::spawn(|| {
        println!("worker sets debug");
        SETTINGS.lock().unwrap().level = "debug".to_string();
        address()
    });
    let seen = worker.join().unwrap();
    println!("main: {}", SETTINGS.lock().unwrap().level);
    println!("same instance: {}", seen == address());
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: stdlib. `std::sync::LazyLock` runs its closure on the first access and stores the value. A `static` of that type is the single instance.
- A `static` must be `Sync`, so mutable state needs a lock such as `Mutex`. A `static mut` would compile only with `unsafe` at every use.
- `OnceLock` is the variant that is set once at run time with a value that the program computes, such as configuration read in `main`.
- A `static` is never dropped, so its destructor does not run at exit.
