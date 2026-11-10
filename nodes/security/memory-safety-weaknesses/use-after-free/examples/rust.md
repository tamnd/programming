```rust fails expect=error:E0502 id=invalidated
fn main() {
    let mut names = vec![String::from("alice")];
    let first = &names[0];
    names.push(String::from("bob"));
    println!("first: {first}");
}
```

```output for=invalidated stream=stderr
error[E0502]: cannot borrow `names` as mutable because it is also borrowed as immutable
 --> main.rs:4:5
  |
3 |     let first = &names[0];
  |                  ----- immutable borrow occurs here
4 |     names.push(String::from("bob"));
  |     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ mutable borrow occurs here
5 |     println!("first: {first}");
  |                       ----- immutable borrow later used here

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0502`.
```

This is the C++ bug from the C++ example file. The compiler rejects it. `push` needs a mutable borrow of `names`, and the shared borrow `first` is still live.

```rust fails expect=error:E0505 id=drop-while-borrowed
fn main() {
    let session = String::from("alice");
    let user = &session;
    drop(session);
    println!("{user}");
}
```

```output for=drop-while-borrowed stream=stderr
error[E0505]: cannot move out of `session` because it is borrowed
 --> main.rs:4:10
  |
2 |     let session = String::from("alice");
  |         ------- binding `session` declared here
3 |     let user = &session;
  |                -------- borrow of `session` occurs here
4 |     drop(session);
  |          ^^^^^^^ move out of `session` occurs here
5 |     println!("{user}");
  |                ---- borrow later used here
  |
help: consider cloning the value if the performance cost is acceptable
  |
3 |     let user = &session.clone();
  |                        ++++++++

error: aborting due to 1 previous error

For more information about this error, try `rustc --explain E0505`.
```

`drop` frees the string. The compiler rejects the move into `drop`, because the borrow `user` is used after it.

```rust run id=index
fn main() {
    let mut names = vec![String::from("alice")];
    let first = 0;
    names.push(String::from("bob"));
    println!("first: {}", names[first]);
}
```

```output for=index
first: alice
```

- Verdict: native. The borrow checker proves at compile time that no reference outlives the value it points to. The Rust book says that the compiler guarantees that references will never be dangling.
- Each value has one owner, and the value is freed when the owner goes out of scope, once. A moved value cannot be used again.
- `Rc` and `Arc` share ownership, and `Weak` gives a reference that must be upgraded and checked before use.
- `unsafe` code with raw pointers can still use freed memory. Keep it small, and review each raw pointer for the lifetime of its target.
