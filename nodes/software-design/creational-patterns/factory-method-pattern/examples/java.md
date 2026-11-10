```java run id=export
import java.util.ArrayList;
import java.util.List;

public class Main {
    interface Writer {
        String header();

        String row(String name, int count);
    }

    record CsvWriter() implements Writer {
        public String header() {
            return "name,count";
        }

        public String row(String name, int count) {
            return name + "," + count;
        }
    }

    record TextWriter() implements Writer {
        public String header() {
            return "Inventory";
        }

        public String row(String name, int count) {
            return name + ": " + count;
        }
    }

    record Row(String name, int count) {}

    abstract static class Exporter {
        final String export(List<Row> rows) {
            Writer writer = createWriter(); // the factory method
            List<String> lines = new ArrayList<>();
            lines.add(writer.header());
            for (Row r : rows) {
                lines.add(writer.row(r.name(), r.count()));
            }
            return String.join("\n", lines);
        }

        protected abstract Writer createWriter();
    }

    static final class CsvExporter extends Exporter {
        @Override
        protected Writer createWriter() {
            return new CsvWriter();
        }
    }

    static final class TextExporter extends Exporter {
        @Override
        protected Writer createWriter() {
            return new TextWriter();
        }
    }

    public static void main(String[] args) {
        List<Row> rows = List.of(new Row("pen", 3), new Row("book", 1));
        for (Exporter exporter : List.of(new CsvExporter(), new TextExporter())) {
            System.out.println(exporter.export(rows));
        }
    }
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

- Verdict: idiom. The classic form: an abstract creator with a `protected abstract` factory method. `export` is `final`, so subclasses can change only the creation step.
- A `Supplier<Writer>` field with a constructor reference such as `CsvWriter::new` is the form with no creator subclass.
- A static factory method such as `List.of` is a different technique. It cannot be overridden, so it is not this pattern.
