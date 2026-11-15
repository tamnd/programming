```rust run id=graph-in-one-column
use std::collections::BTreeMap;

struct Department {
    name: String,
    children: Vec<Department>,
}

fn dept(name: &str, children: Vec<Department>) -> Department {
    Department { name: name.to_string(), children }
}

// The standard library has no JSON, so a small writer and reader handle this one shape.
fn write(ds: &[Department], out: &mut String) {
    out.push('[');
    for (i, d) in ds.iter().enumerate() {
        if i > 0 {
            out.push(',');
        }
        out.push_str("{\"name\":\"");
        out.push_str(&d.name.replace('\\', "\\\\").replace('"', "\\\""));
        out.push_str("\",\"children\":");
        write(&d.children, out);
        out.push('}');
    }
    out.push(']');
}

struct Reader<'a> {
    s: &'a [u8],
    i: usize,
}

impl Reader<'_> {
    fn expect(&mut self, t: &str) {
        assert!(self.s[self.i..].starts_with(t.as_bytes()), "expected {} at {}", t, self.i);
        self.i += t.len();
    }

    fn string(&mut self) -> String {
        self.expect("\"");
        let mut out = Vec::new();
        while self.s[self.i] != b'"' {
            if self.s[self.i] == b'\\' {
                self.i += 1;
            }
            out.push(self.s[self.i]);
            self.i += 1;
        }
        self.i += 1;
        String::from_utf8(out).expect("utf-8")
    }

    fn list(&mut self) -> Vec<Department> {
        let mut out = Vec::new();
        self.expect("[");
        while self.s[self.i] != b']' {
            if !out.is_empty() {
                self.expect(",");
            }
            self.expect("{\"name\":");
            let name = self.string();
            self.expect(",\"children\":");
            let children = self.list();
            self.expect("}");
            out.push(Department { name, children });
        }
        self.i += 1;
        out
    }
}

// A stand-in for the database: the customer table, with the departments as one text column.
struct Db {
    customer: BTreeMap<i64, (String, String)>,
}

impl Db {
    // The whole graph of departments is written as one value in one column.
    fn save(&mut self, id: i64, name: &str, departments: &[Department]) -> usize {
        let mut lob = String::new();
        write(departments, &mut lob);
        let size = lob.len();
        self.customer.insert(id, (name.to_string(), lob));
        size
    }

    fn load(&self, id: i64) -> (String, Vec<Department>) {
        let (name, lob) = &self.customer[&id];
        (name.clone(), Reader { s: lob.as_bytes(), i: 0 }.list())
    }
}

fn paths(ds: &[Department], prefix: &str, out: &mut Vec<String>) {
    for d in ds {
        out.push(format!("{}{}", prefix, d.name));
        paths(&d.children, &format!("{}{}/", prefix, d.name), out);
    }
}

fn ancestors(ds: &[Department], target: &str, above: &[String]) -> Option<Vec<String>> {
    for d in ds {
        if d.name == target {
            return Some(above.iter().rev().cloned().collect());
        }
        let mut next = above.to_vec();
        next.push(d.name.clone());
        if let Some(found) = ancestors(&d.children, target, &next) {
            return Some(found);
        }
    }
    None
}

fn main() {
    let mut db = Db { customer: BTreeMap::new() };
    let tree = vec![
        dept("Sales", vec![dept("Presales", vec![]), dept("Key accounts", vec![])]),
        dept("Support", vec![]),
    ];
    let size = db.save(1, "Acme", &tree);
    println!("saved Acme: {} bytes in one column", size);
    println!("column: {}", db.customer[&1].1);
    let (name, mut loaded) = db.load(1);
    let mut ps = Vec::new();
    paths(&loaded, "", &mut ps);
    println!("loaded {}: {}", name, ps.join(" | "));
    let up = ancestors(&loaded, "Presales", &[]).expect("Presales");
    println!("ancestors of Presales: {}", up.join(", "));
    loaded[1].children.push(dept("Field", vec![]));
    let size = db.save(1, &name, &loaded);
    println!("add Support/Field: {} bytes written again", size);
    println!("rows in customer: {}", db.customer.len());
}
```

```output
saved Acme: 136 bytes in one column
column: [{"name":"Sales","children":[{"name":"Presales","children":[]},{"name":"Key accounts","children":[]}]},{"name":"Support","children":[]}]
loaded Acme: Sales | Sales/Presales | Sales/Key accounts | Support
ancestors of Presales: Sales
add Support/Field: 166 bytes written again
rows in customer: 1
```

- Verdict: idiom. The Rust standard library has no JSON, so a small writer and a byte reader handle the one shape of the tree. A program with the `serde_json` crate would derive the code instead.
- `load` returns the name and the tree by value, so the program can change the tree and save it again.
