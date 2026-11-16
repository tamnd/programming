```rust run id=tree
struct Node {
    name: String,
    size: u32,
    children: Vec<Node>,
}

fn file(name: &str, size: u32) -> Node {
    Node {
        name: name.to_string(),
        size,
        children: Vec::new(),
    }
}

fn dir(name: &str, children: Vec<Node>) -> Node {
    Node {
        name: name.to_string(),
        size: 0,
        children,
    }
}

fn show(node: &Node, depth: usize) {
    let indent = "  ".repeat(depth);
    if node.children.is_empty() {
        println!("{indent}{} {}", node.name, node.size);
        return;
    }
    println!("{indent}{}/", node.name);
    for child in &node.children {
        show(child, depth + 1);
    }
}

fn count_files(node: &Node) -> usize {
    if node.children.is_empty() {
        return 1;
    }
    node.children.iter().map(count_files).sum()
}

fn total_size(node: &Node) -> u32 {
    node.size + node.children.iter().map(total_size).sum::<u32>()
}

fn height(node: &Node) -> usize {
    node.children.iter().map(|child| 1 + height(child)).max().unwrap_or(0)
}

fn main() {
    let root = dir(
        "project",
        vec![
            file("README.md", 120),
            dir("src", vec![file("main.py", 300), file("util.py", 80)]),
            dir("tests", vec![file("test_main.py", 150)]),
        ],
    );
    show(&root, 0);
    println!(
        "files {}, total size {}, height {}",
        count_files(&root),
        total_size(&root),
        height(&root)
    );
}
```

```output for=tree
project/
  README.md 120
  src/
    main.py 300
    util.py 80
  tests/
    test_main.py 150
files 4, total size 650, height 2
```

- Verdict: idiom. A struct can hold a `Vec` of its own type, because the vector stores the children on the heap.
- Each node owns its children. Dropping the root drops the whole tree.
