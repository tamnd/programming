```rust run id=frames
fn count(n: i32, addrs: &mut Vec<*const i32>) {
    let here = n * 10; // a local: it lives in the frame of this call
    addrs.push(&here as *const i32);
    println!("enter n={} here={}", n, here);
    if n > 1 {
        count(n - 1, addrs);
    } else {
        let distinct = addrs[0] != addrs[1] && addrs[1] != addrs[2] && addrs[0] != addrs[2];
        let word = if distinct { "different" } else { "shared" };
        println!("{} frames, {} {} addresses", addrs.len(), addrs.len(), word);
    }
    println!("leave n={} here={}", n, here);
}

fn main() {
    let mut addrs = Vec::new();
    count(3, &mut addrs);
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
3 frames, 3 different addresses
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has its own frame, and `here` is in it. The three calls are on the stack at the same time, so the three copies of `here` have three addresses.
- The raw pointers are only compared, never read. After a call returns, its frame is gone and the pointer to its `here` must not be used.
