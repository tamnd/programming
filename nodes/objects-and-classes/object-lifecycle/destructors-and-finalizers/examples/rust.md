```rust run id=drop-order
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) {
        println!("drop {}", self.0);
    }
}

struct Pair {
    first: Noisy,
    second: Noisy,
}

fn main() {
    let _a = Noisy("a");
    let _b = Noisy("b"); // locals drop in reverse order: b, then a
    {
        let p = Pair {
            first: Noisy("p.first"),
            second: Noisy("p.second"),
        };
        println!("end of block with {} and {}", p.first.0, p.second.0);
    } // fields drop in declaration order
    let v = vec![Noisy("v0"), Noisy("v1")];
    drop(v); // drop early; elements drop in order
    let c = Noisy("c");
    std::mem::forget(c); // never dropped: forgetting is safe, only a leak
    println!("end of main");
}
```

```output for=drop-order
end of block with p.first and p.second
drop p.first
drop p.second
drop v0
drop v1
end of main
drop b
drop a
```

```rust fails expect=error:E0040 id=explicit-drop-call
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) {
        println!("drop {}", self.0);
    }
}

fn main() {
    let a = Noisy("a");
    a.drop(); // a second drop would run at the end of the scope
}
```

- Verdict: native. A type that implements `Drop` has a destructor, and the compiler inserts the call where the value's owner goes away. Local variables drop in the reverse order of their declaration, the fields of a struct in the order of their declaration, and the elements of a `Vec` from first to last. `drop(v)` moves the value into a function that ends at once, which drops it early. `std::mem::forget` skips the destructor; Rust treats a leak as safe, so code must not rely on a destructor to keep memory safe.
- The failing block calls the `drop` method directly. The compiler rejects this with error E0040, because the value would be dropped again at the end of its scope. Call `drop(a)` instead, which takes the value by move.
