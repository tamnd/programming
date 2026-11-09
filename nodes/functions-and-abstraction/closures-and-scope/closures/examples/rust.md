```rust run id=counter
fn make_counter() -> impl FnMut() -> u32 {
    let mut count = 0;
    move || {
        count += 1;
        count
    }
}

fn main() {
    let mut first = make_counter();
    let mut second = make_counter();
    println!("{} {} {}", first(), first(), first());
    println!("{}", second());
    println!("{} bytes", std::mem::size_of_val(&first));
}
```

```output for=counter
1 2 3
1
4 bytes
```

- Verdict: native. A closure expression makes a value of a unique anonymous type that holds the captures.
- The compiler picks the capture mode of each variable from its use: a shared borrow, a unique borrow or a move. `move` forces a move, which the returned closure needs.
- The traits `Fn`, `FnMut` and `FnOnce` say how a closure may be called. This one changes its state, so it is `FnMut`, and the caller must hold it in a `mut` binding.
- The closure holds one `u32`, so its size is 4 bytes, with no heap allocation. `Box<dyn FnMut() -> u32>` is the form for closures of different types in one collection.

Without `move`, the closure borrows `count`, and the borrow cannot outlive the function. The compiler rejects the program with error E0373, "closure may outlive the current function, but it borrows `count`".

```rust fails expect=compile id=borrow
fn make_counter() -> impl FnMut() -> u32 {
    let mut count = 0;
    || {
        count += 1;
        count
    }
}

fn main() {
    let mut counter = make_counter();
    println!("{}", counter());
}
```
