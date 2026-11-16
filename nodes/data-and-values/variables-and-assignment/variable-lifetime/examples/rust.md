```rust run id=drop-order
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) {
        println!("drop {}", self.0);
    }
}

fn main() {
    let _a = Noisy("a");
    {
        let _b = Noisy("b");
        println!("inner end");
    }
    let c = Noisy("c");
    drop(c);
    println!("main end");
}
```

```output for=drop-order
inner end
drop b
drop c
main end
drop a
```

- Verdict: native. A value ends when the variable that owns it goes out of scope, and Rust runs `drop` at that point: `_b` at the end of the inner block, and `_a` at the end of `main`. `drop(c)` moves `c` into a function, which ends it early. The compiler also checks that no reference lives longer than its value.
