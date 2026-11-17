```rust run id=deep
use std::thread;

fn depth(n: u64) -> u64 {
    if n == 0 { 0 } else { 1 + depth(n - 1) } // one frame for each step
}

fn looped(mut n: u64) -> u64 {
    let mut count = 0;
    while n > 0 { // one frame for the whole loop
        n -= 1;
        count += 1;
    }
    count
}

fn main() {
    println!("depth(100) = {}", depth(100));
    let big = thread::Builder::new()
        .stack_size(256 * 1024 * 1024) // 256 MiB for this thread
        .spawn(|| depth(1_000_000))
        .unwrap();
    println!("depth(1000000) on a 256 MiB stack = {}", big.join().unwrap());
    println!("loop(1000000) = {}", looped(1_000_000));
}
```

```output for=deep
depth(100) = 100
depth(1000000) on a 256 MiB stack = 1000000
loop(1000000) = 1000000
```

- Verdict: native. Rust does not set the stack size of the main thread, and the platform decides it. A thread made with `thread::Builder::stack_size` gets the size that it asks for, so the deep call runs there.
