```rust run id=thread-fringe
use std::sync::mpsc::{sync_channel, Receiver, SyncSender};
use std::thread;

enum Tree {
    Leaf(i32),
    Node(Vec<Tree>),
}

use Tree::{Leaf, Node};

// Send each leaf. A send on a channel with bound 0 waits for the receiver,
// so walk pauses there, deep in the recursion. It stops when the receiver is gone.
fn walk(tree: &Tree, tx: &SyncSender<i32>) -> bool {
    match tree {
        Leaf(v) => tx.send(*v).is_ok(),
        Node(children) => children.iter().all(|child| walk(child, tx)),
    }
}

fn start(tree: Tree) -> Receiver<i32> {
    let (tx, rx) = sync_channel(0);
    thread::spawn(move || {
        walk(&tree, &tx);
    });
    rx
}

fn same_fringe(x: Tree, y: Tree) -> bool {
    let (rx, ry) = (start(x), start(y));
    loop {
        match (rx.recv().ok(), ry.recv().ok()) {
            (None, None) => return true,
            (a, b) if a == b => continue,
            _ => return false,
        }
    }
}

fn a() -> Tree {
    Node(vec![Node(vec![Leaf(1), Leaf(2)]), Node(vec![Leaf(3), Node(vec![Leaf(4)])])])
}

fn b() -> Tree {
    Node(vec![Leaf(1), Node(vec![Leaf(2), Node(vec![Leaf(3), Leaf(4)])])])
}

fn c() -> Tree {
    Node(vec![Node(vec![Leaf(1), Leaf(2)]), Node(vec![Leaf(3), Leaf(5)])])
}

fn main() {
    for leaf in start(a()) {
        println!("got {leaf}");
    }
    println!("same fringe a b: {}", same_fringe(a(), b()));
    println!("same fringe a c: {}", same_fringe(a(), c()));
}
```

```output for=thread-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. Rust has no fibers in the standard library. An OS thread with a rendezvous channel gives the same stackful pause, at the cost of a full thread for each walk.
- `sync_channel(0)` makes a channel with no buffer, so each `send` waits until the receiver takes the value.
- When `same_fringe` returns, it drops the receivers. The next `send` in each walk then returns an error, `walk` returns `false`, and the thread ends.
