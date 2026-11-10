```rust run id=export
trait Writer {
    fn header(&self) -> String;
    fn row(&self, name: &str, count: u32) -> String;
}

struct CsvWriter;

impl Writer for CsvWriter {
    fn header(&self) -> String {
        "name,count".to_string()
    }
    fn row(&self, name: &str, count: u32) -> String {
        format!("{name},{count}")
    }
}

struct TextWriter;

impl Writer for TextWriter {
    fn header(&self) -> String {
        "Inventory".to_string()
    }
    fn row(&self, name: &str, count: u32) -> String {
        format!("{name}: {count}")
    }
}

trait Exporter {
    type Product: Writer;

    // The factory method: each implementation must supply it.
    fn create_writer(&self) -> Self::Product;

    // A default method that calls the factory method.
    fn export(&self, rows: &[(&str, u32)]) -> String {
        let writer = self.create_writer();
        let mut lines = vec![writer.header()];
        lines.extend(rows.iter().map(|(name, count)| writer.row(name, *count)));
        lines.join("\n")
    }
}

struct CsvExporter;

impl Exporter for CsvExporter {
    type Product = CsvWriter;
    fn create_writer(&self) -> CsvWriter {
        CsvWriter
    }
}

struct TextExporter;

impl Exporter for TextExporter {
    type Product = TextWriter;
    fn create_writer(&self) -> TextWriter {
        TextWriter
    }
}

fn main() {
    let rows = [("pen", 3), ("book", 1)];
    println!("{}", CsvExporter.export(&rows));
    println!("{}", TextExporter.export(&rows));
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. Rust has no inheritance, but a trait with a default method gives the same shape. The default `export` calls the required `create_writer`, and each type supplies it.
- The associated type `Product` fixes the product type for each exporter at compile time, so the call needs no allocation and no vtable.
- An associated type makes `Exporter` unusable as `dyn Exporter` without naming `Product`. A list of mixed exporters needs `fn create_writer(&self) -> Box<dyn Writer>` instead.
- `CsvWriter::new()` style functions are not the pattern. They are static, inherent methods, the Rust convention for constructors.
