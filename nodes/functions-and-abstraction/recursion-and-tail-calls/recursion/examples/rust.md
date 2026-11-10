```rust run id=tree
enum Tree {
    Leaf(i32),
    Node(Box<Tree>, Box<Tree>),
}

fn total(tree: &Tree) -> i32 {
    match tree {
        Tree::Leaf(value) => *value,
        Tree::Node(left, right) => total(left) + total(right),
    }
}

fn depth(n: u64) -> u64 {
    if n == 0 { 0 } else { 1 + depth(n - 1) }
}

fn main() {
    let tree = Tree::Node(
        Box::new(Tree::Leaf(1)),
        Box::new(Tree::Node(Box::new(Tree::Leaf(2)), Box::new(Tree::Leaf(3)))),
    );
    println!("{}", total(&tree));
    let worker = std::thread::Builder::new()
        .stack_size(256 * 1024 * 1024)
        .spawn(|| depth(1_000_000))
        .unwrap();
    println!("{}", worker.join().unwrap());
}
```

```output for=tree
6
1000000
```

- Verdict: native. A function can call itself, and each call gets a frame on the stack of its thread.
- A recursive type needs indirection. `Tree` holds its children in `Box`, so the compiler knows the size of `Tree`.
- A spawned thread has a stack of 2 MiB by default on tier 1 platforms. `thread::Builder::stack_size` and the `RUST_MIN_STACK` variable change it. Rust does not decide the size of the main thread's stack.
- The spawned thread above has a stack of 256 MiB, so a depth of a million calls fits.

A deep recursion on the main thread overflows the stack.

```rust fails expect=runtime id=overflow
fn depth(n: u64) -> u64 {
    if n == 0 { 0 } else { 1 + depth(n - 1) }
}

fn main() {
    println!("{}", depth(std::hint::black_box(100_000_000)));
}
```
