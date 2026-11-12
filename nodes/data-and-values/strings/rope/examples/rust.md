```rust run id=rc-sharing-and-split
use std::rc::Rc;

// A persistent rope: nodes never change, and Rc lets versions share them.
enum Rope {
    Leaf(String),
    Node(Rc<Rope>, Rc<Rope>, usize),
}

fn len(r: &Rope) -> usize {
    match r {
        Rope::Leaf(s) => s.len(),
        Rope::Node(_, _, n) => *n,
    }
}

fn leaf(s: &str) -> Rc<Rope> {
    Rc::new(Rope::Leaf(s.to_string()))
}

fn concat(a: Rc<Rope>, b: Rc<Rope>) -> Rc<Rope> {
    if len(&a) == 0 {
        return b;
    }
    if len(&b) == 0 {
        return a;
    }
    let n = len(&a) + len(&b);
    Rc::new(Rope::Node(a, b, n))
}

// split returns the first i bytes and the rest. A leaf is cut only at a char boundary.
fn split(r: &Rc<Rope>, i: usize) -> (Rc<Rope>, Rc<Rope>) {
    if i == 0 {
        return (leaf(""), Rc::clone(r));
    }
    if i >= len(r) {
        return (Rc::clone(r), leaf(""));
    }
    match r.as_ref() {
        Rope::Leaf(s) => {
            assert!(s.is_char_boundary(i));
            (leaf(&s[..i]), leaf(&s[i..]))
        }
        Rope::Node(left, right, _) => {
            let w = len(left);
            if i < w {
                let (a, b) = split(left, i);
                (a, concat(b, Rc::clone(right)))
            } else {
                let (a, b) = split(right, i - w);
                (concat(Rc::clone(left), a), b)
            }
        }
    }
}

fn insert(r: &Rc<Rope>, i: usize, s: &str) -> Rc<Rope> {
    let (a, b) = split(r, i);
    concat(concat(a, leaf(s)), b)
}

fn text(r: &Rope, out: &mut String) {
    match r {
        Rope::Leaf(s) => out.push_str(s),
        Rope::Node(a, b, _) => {
            text(a, out);
            text(b, out);
        }
    }
}

fn show(r: &Rope) -> String {
    let mut s = String::new();
    text(r, &mut s);
    s
}

fn main() {
    let world = leaf("world");
    let hello = leaf("Hello, ");
    let r = concat(Rc::clone(&hello), Rc::clone(&world));
    let r2 = insert(&r, 7, "big ");
    println!("{} | {}", show(&r2), show(&r));

    // Each leaf has three owners: its variable, the old rope and the new rope.
    println!("{} {}", Rc::strong_count(&world), Rc::strong_count(&hello));

    // When the old rope is dropped, the shared leaves stay alive in the new one.
    drop(r);
    println!("{} {}", Rc::strong_count(&world), show(&r2));
}
```

```output for=rc-sharing-and-split
Hello, big world | Hello, world
3 3
2 Hello, big world
```

- Verdict: idiom. The Rust standard library has no rope, and the `ropey` crate is a common choice. The nodes are behind `Rc`, so `insert` reuses the leaves `Hello, ` and `world`, and each leaf has 3 owners. After `drop(r)`, the leaf `world` has 2 owners and the new rope still works. `split` checks `is_char_boundary`, because a byte index can fall inside a UTF-8 character.
