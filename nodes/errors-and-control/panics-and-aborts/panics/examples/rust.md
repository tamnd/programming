```rust run id=jobs
use std::panic;
use std::thread;

struct Cleanup(usize);

impl Drop for Cleanup {
    fn drop(&mut self) {
        println!("cleanup job {}", self.0);
    }
}

fn run_job(job: usize, items: &[u32]) -> u32 {
    let _guard = Cleanup(job);
    items[job] * 10
}

fn main() {
    panic::set_hook(Box::new(|_| {}));
    let items = [1, 2];
    for job in [0, 5, 1] {
        match panic::catch_unwind(|| run_job(job, &items)) {
            Ok(result) => println!("job {job} -> {result}"),
            Err(payload) => {
                let message = payload.downcast_ref::<String>().map_or("?", |s| s.as_str());
                println!("job {job} failed: {message}");
            }
        }
    }

    let worker = thread::spawn(|| -> u32 { panic!("worker found a broken invariant") });
    println!("worker joined with an error: {}", worker.join().is_err());
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: index out of bounds: the len is 2 but the index is 5
cleanup job 1
job 1 -> 20
worker joined with an error: true
```

- Verdict: native. `panic!` and run-time checks such as an index out of bounds start a panic. By default the panic unwinds the stack and runs the `drop` of each value, so `cleanup job 5` prints before the failure is reported.
- `catch_unwind` stops an unwinding panic and returns it as an `Err`. Its documentation says that it is not for a general try and catch, and that `Result` fits functions that fail on a regular basis. A thread is a second boundary: `join` returns `Err` when the thread panicked.
- The panic hook runs before unwinding. The default hook prints the message and the location to standard error. This program sets an empty hook to keep the output short.
- With `panic = 'abort'` in a Cargo profile, a panic ends the process at once. No `drop` runs, and `catch_unwind` cannot stop it.

A panic that reaches the end of `main` ends the program with exit status 101.

```rust fails expect=exit:101 id=main-panic
fn main() {
    let ports: Vec<u16> = Vec::new();
    println!("first port: {}", ports[0]);
}
```

`std::process::abort` ends the process at once. It runs no destructors, so the `drop` of `Cleanup` does not run.

```rust fails expect=runtime id=abort
struct Cleanup;

impl Drop for Cleanup {
    fn drop(&mut self) {
        println!("cleanup");
    }
}

fn main() {
    let _guard = Cleanup;
    std::process::abort();
}
```
