```java run id=around
import java.util.List;
import java.util.function.Function;

public class Main {
    static <T> T withConnection(String name, Function<String, T> block) {
        System.out.println("open " + name);
        try {
            return block.apply(name); // the caller's work
        } finally {
            System.out.println("close " + name); // runs even if block throws
        }
    }

    public static void main(String[] args) {
        int result = withConnection("db", conn -> {
            List<String> rows = List.of("a", "b", "c");
            System.out.println("query: " + rows.size() + " rows");
            return rows.size();
        });
        System.out.println("result: " + result);
    }
}
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: native. A method takes the block as a lambda. For an `AutoCloseable` resource, try-with-resources does the same work.
