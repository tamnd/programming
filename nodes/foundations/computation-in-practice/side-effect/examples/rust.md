```rust run id=shared-and-mutable-references
fn total(xs: &[i32]) -> i32 {
    xs.iter().sum() // a shared reference: the function can only read
}

fn push_total(xs: &mut Vec<i32>) {
    let t = total(xs);
    xs.push(t); // &mut in the signature says that the caller's vector changes
}

fn main() {
    let mut xs = vec![1, 2, 3];
    println!("total {}", total(&xs));
    push_total(&mut xs);
    println!("after push_total {:?}", xs);

    let mut calls = 0;
    let doubled: Vec<i32> = xs.iter().map(|x| { calls += 1; x * 2 }).collect();
    println!("{:?} after {} calls", doubled, calls);
}
```

```output for=shared-and-mutable-references
total 6
after push_total [1, 2, 3, 6]
[2, 4, 6, 12] after 4 calls
```

```rust fails expect=error:E0596 id=change-through-shared-reference
fn push_total(xs: &Vec<i32>) {
    let t: i32 = xs.iter().sum();
    xs.push(t); // a shared reference does not allow the change
}

fn main() {
    let xs = vec![1, 2, 3];
    push_total(&xs);
    println!("{:?}", xs);
}
```

- Verdict: native. A function can only change the caller's data when it receives a `&mut` reference, and the caller writes `&mut` at the call, so both sides show the effect. A closure that changes a captured variable, such as `calls`, borrows it mutably while the closure lives. The fails block tries to push through a `&` reference, and the compiler reports E0596. Rust does not track other effects, such as I/O, in types.
