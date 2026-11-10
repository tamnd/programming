```rust run id=tree
enum Entry {
    File { name: String, size: u64 },
    Directory { name: String, children: Vec<Entry> },
}

use Entry::{Directory, File};

impl Entry {
    fn size(&self) -> u64 {
        match self {
            File { size, .. } => *size,
            Directory { children, .. } => children.iter().map(Entry::size).sum(),
        }
    }

    fn lines(&self, depth: usize, out: &mut Vec<String>) {
        let indent = "  ".repeat(depth);
        match self {
            File { name, size } => out.push(format!("{indent}{name} {size}")),
            Directory { name, children } => {
                out.push(format!("{indent}{name}/ {}", self.size()));
                for child in children {
                    child.lines(depth + 1, out);
                }
            }
        }
    }
}

fn file(name: &str, size: u64) -> Entry {
    File { name: name.into(), size }
}

fn dir(name: &str, children: Vec<Entry>) -> Entry {
    Directory { name: name.into(), children }
}

fn main() {
    let project = dir("project", vec![file("readme.md", 200), dir("src", vec![file("main.go", 1200), file("util.go", 300)])]);
    let mut out = Vec::new();
    project.lines(0, &mut out);
    println!("{}", out.join("\n"));
}
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. An enum with one variant for the leaf and one for the composite. A `match` must cover every variant, so the compiler finds each operation that a new variant would break.
- A `Directory` owns its children in a `Vec<Entry>`. Ownership makes a cycle impossible here: an entry cannot own itself. A tree with links to the parent needs `Rc` for the children and `Weak` for the parent, as the Rust book shows.
- A trait object form, with `Vec<Box<dyn Entry>>` and a trait for the operations, is the classic form. It lets code outside the crate add kinds of entries, and it makes new operations harder to add.
