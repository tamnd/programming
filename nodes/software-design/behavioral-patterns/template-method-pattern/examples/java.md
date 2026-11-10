```java run id=loader
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    record Item(String name, int count) {
        @Override
        public String toString() {
            return name + ":" + count;
        }
    }

    abstract static class Loader {
        // The template method. final stops a subclass from changing the steps.
        public final List<Item> load(List<String> lines) {
            List<Item> records = new ArrayList<>();
            for (String raw : lines) {
                String line = raw.strip();
                if (line.isEmpty() || line.startsWith("#")) {
                    continue;
                }
                Item record = parse(line);
                if (accept(record)) {
                    records.add(record);
                }
            }
            return records;
        }

        protected abstract Item parse(String line);

        protected boolean accept(Item record) {
            return true;
        }
    }

    static final class CsvLoader extends Loader {
        @Override
        protected Item parse(String line) {
            String[] parts = line.split(",");
            return new Item(parts[0], Integer.parseInt(parts[1]));
        }
    }

    static final class KeyValueLoader extends Loader {
        @Override
        protected Item parse(String line) {
            String[] parts = line.split("=");
            return new Item(parts[0], Integer.parseInt(parts[1]));
        }

        @Override
        protected boolean accept(Item record) {
            return record.count() > 0;
        }
    }

    static void show(String label, List<Item> records) {
        System.out.println(label + ": " + records.stream().map(Item::toString).collect(Collectors.joining(" ")));
    }

    public static void main(String[] args) {
        show("csv", new CsvLoader().load(List.of("# stock", "pen,3", "", "book,1")));
        show("kv", new KeyValueLoader().load(List.of("pen=3", "# sold out", "cup=0")));
    }
}
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. The template method is `public final`, the primitive operation is `protected abstract`, and the hook is `protected` with a default. Each modifier states one rule of the pattern, and the compiler enforces it.
- Java methods can be overridden unless they are `final`. A template method that is not `final` can be replaced by a subclass, which breaks the fixed order of steps.
- `java.io.InputStream` and `java.util.AbstractList` in the standard library have this form (see the node).
