```java run id=remove-the-hooks
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    // Before: an interface and an abstract base class, with one subclass.
    interface RowWriter {
        String write(List<List<Object>> rows);
    }

    abstract static class AbstractDelimitedWriter implements RowWriter {
        protected abstract String delimiter();

        public String write(List<List<Object>> rows) {
            return rows.stream()
                    .map(r -> r.stream().map(String::valueOf).collect(Collectors.joining(delimiter())))
                    .collect(Collectors.joining("\n"));
        }
    }

    static final class CsvWriter extends AbstractDelimitedWriter {
        protected String delimiter() { return ","; }
    }

    static String exportBefore(RowWriter writer, List<List<Object>> rows) {
        return writer.write(rows);
    }

    // After: the hierarchy is collapsed into one static method.
    static String toCsv(List<List<Object>> rows) {
        return rows.stream()
                .map(r -> r.stream().map(String::valueOf).collect(Collectors.joining(",")))
                .collect(Collectors.joining("\n"));
    }

    public static void main(String[] args) {
        List<List<Object>> rows = List.of(List.of("id", "name"), List.of(1, "Ada"), List.of(2, "Grace"));
        if (!exportBefore(new CsvWriter(), rows).equals("id,name\n1,Ada\n2,Grace")) throw new AssertionError("before");
        if (!toCsv(rows).equals(exportBefore(new CsvWriter(), rows))) throw new AssertionError("after");
        System.out.println("toCsv: tests pass");
    }
}
```

```output
toCsv: tests pass
```

- Three types, `RowWriter`, `AbstractDelimitedWriter` and `CsvWriter`, hold one decision: the delimiter is a comma. Collapse Hierarchy and Inline Class bring it down to one method.
- If a second format appears, the interface can come back. Then its methods follow the two real formats, not a guess.
