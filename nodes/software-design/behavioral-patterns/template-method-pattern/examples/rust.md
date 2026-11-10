```rust run id=loader
#[derive(Debug)]
struct Item(String, u32);

trait Loader {
    // A required method: each implementation must give it.
    fn parse(&self, line: &str) -> Item;

    // A hook: a default implementation that an implementation may replace.
    fn accept(&self, _record: &Item) -> bool {
        true
    }

    // The template: a provided method that calls the others.
    fn load(&self, lines: &[&str]) -> Vec<Item> {
        lines
            .iter()
            .map(|line| line.trim())
            .filter(|line| !line.is_empty() && !line.starts_with('#'))
            .map(|line| self.parse(line))
            .filter(|record| self.accept(record))
            .collect()
    }
}

fn split(line: &str, sep: char) -> Item {
    let (name, count) = line.split_once(sep).expect("a separator");
    Item(name.to_string(), count.parse().expect("a count"))
}

struct Csv;
struct KeyValue;

impl Loader for Csv {
    fn parse(&self, line: &str) -> Item {
        split(line, ',')
    }
}

impl Loader for KeyValue {
    fn parse(&self, line: &str) -> Item {
        split(line, '=')
    }

    fn accept(&self, record: &Item) -> bool {
        record.1 > 0
    }
}

fn show(label: &str, records: &[Item]) {
    let parts: Vec<String> = records.iter().map(|Item(name, count)| format!("{name}:{count}")).collect();
    println!("{label}: {}", parts.join(" "));
}

fn main() {
    show("csv", &Csv.load(&["# stock", "pen,3", "", "book,1"]));
    show("kv", &KeyValue.load(&["pen=3", "# sold out", "cup=0"]));
}
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: native. A trait's provided methods are template methods. A default method can call the required methods of the same trait, even though the trait does not implement them. `Iterator` in the standard library is the main case: an implementation gives `next`, and `map`, `filter`, `collect` and dozens of other methods are defaults built on it.
- Rust has no `final` for trait methods. An implementation may replace `load` too. A free function generic over `T: Loader` is the form that no implementation can replace.
- There is no inheritance of state. The trait cannot hold fields, so data that the template needs must come through methods or arguments.
