```rust run id=tree
struct Node {
    key: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

#[derive(Default)]
struct Tree {
    root: Option<Box<Node>>,
}

impl Tree {
    fn from_keys(keys: &[i32]) -> Tree {
        let mut tree = Tree::default();
        for &key in keys {
            tree.insert(key);
        }
        tree
    }

    fn insert(&mut self, key: i32) {
        let mut link = &mut self.root;
        while let Some(node) = link {
            link = if key < node.key { &mut node.left } else { &mut node.right };
        }
        *link = Some(Box::new(Node { key, left: None, right: None }));
    }

    fn iter(&self) -> Iter<'_> {
        let mut it = Iter { stack: Vec::new() };
        it.push_left(self.root.as_deref());
        it
    }
}

/// The iterator borrows the tree, so the tree cannot change while it lives.
struct Iter<'a> {
    stack: Vec<&'a Node>,
}

impl<'a> Iter<'a> {
    fn push_left(&mut self, mut node: Option<&'a Node>) {
        while let Some(n) = node {
            self.stack.push(n);
            node = n.left.as_deref();
        }
    }
}

impl<'a> Iterator for Iter<'a> {
    type Item = i32;

    fn next(&mut self) -> Option<i32> {
        let node = self.stack.pop()?;
        self.push_left(node.right.as_deref());
        Some(node.key)
    }
}

impl<'a> IntoIterator for &'a Tree {
    type Item = i32;
    type IntoIter = Iter<'a>;

    fn into_iter(self) -> Iter<'a> {
        self.iter()
    }
}

fn yes_no(ok: bool) -> &'static str {
    if ok { "yes" } else { "no" }
}

fn main() {
    let a = Tree::from_keys(&[5, 3, 8, 1, 4, 9]);
    let b = Tree::from_keys(&[1, 3, 4, 5, 8, 9]);
    let c = Tree::from_keys(&[1, 3, 4, 5, 8]);

    let mut keys = Vec::new();
    for key in &a {
        keys.push(key.to_string());
    }
    println!("in order: {}", keys.join(" "));
    println!("sum: {}", a.iter().sum::<i32>());
    let first: Vec<String> = a.iter().take(2).map(|k| k.to_string()).collect();
    println!("first two: {}", first.join(" "));
    println!("same keys as b: {}", yes_no(a.iter().eq(b.iter())));
    println!("same keys as c: {}", yes_no(a.iter().eq(c.iter())));
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A type implements the trait `Iterator` with one required method, `next`, which returns `Option<Self::Item>`. Every adapter (`take`, `map`, `sum`, `eq`) comes from the trait as a provided method.
- `impl IntoIterator for &Tree` lets `for key in &a` work. The `for` loop calls `into_iter` once and then `next` until `None`.
- `Iter<'a>` holds shared borrows of the nodes. The borrow checker rejects a call of `insert` while an iterator is alive, so the iterator can never see a tree that changed under it.
- `Iterator::eq` compares two iterators element by element and stops at the first difference. It is the same-fringe comparison in one call.
