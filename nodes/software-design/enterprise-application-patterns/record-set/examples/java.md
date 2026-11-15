```java run id=account-record-set
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Main {
    // Rows and named columns in memory, in the shape of the result of a query.
    static class RecordSet {
        final List<String> columns;
        final List<Object[]> rows = new ArrayList<>();
        final List<String> states = new ArrayList<>();

        RecordSet(List<String> columns, List<Object[]> loaded) {
            this.columns = List.copyOf(columns);
            for (Object[] r : loaded) {
                rows.add(r.clone());
                states.add("unchanged");
            }
        }

        Object get(int i, String column) { return rows.get(i)[columns.indexOf(column)]; }

        void set(int i, String column, Object value) {
            rows.get(i)[columns.indexOf(column)] = value;
            if (states.get(i).equals("unchanged")) states.set(i, "modified");
        }

        void add(Object... values) {
            rows.add(values);
            states.add("added");
        }

        List<Integer> where(Predicate<Function<String, Object>> test) {
            return IntStream.range(0, rows.size()).filter(i -> test.test(c -> get(i, c))).boxed().toList();
        }
    }

    static String money(long c) { return String.format("%d.%02d", c / 100, c % 100); }

    public static void main(String[] args) {
        // The columns and rows as a JDBC ResultSet gives them for:
        // select id, name, balance from account order by id
        RecordSet rs = new RecordSet(List.of("id", "name", "balance"),
                Arrays.asList(new Object[] {1, "Ann", 12000L}, new Object[] {2, "Bob", 1550L}));

        System.out.println("columns: " + String.join(", ", rs.columns));
        rs.set(1, "balance", (Long) rs.get(1, "balance") + 1000);
        rs.add(3, "Cy", 0L);
        for (int i = 0; i < rs.rows.size(); i++) {
            System.out.println(rs.get(i, "id") + " | " + rs.get(i, "name") + " | "
                    + money((Long) rs.get(i, "balance")) + " | " + rs.states.get(i));
        }
        List<Integer> rich = rs.where(col -> (Long) col.apply("balance") > 2000);
        System.out.println("balance over 20.00: "
                + rich.stream().map(i -> (String) rs.get(i, "name")).collect(Collectors.joining(", ")));
        System.out.println("rows to save: " + rs.states.stream().filter(s -> !s.equals("unchanged")).count());
    }
}
```

```output
columns: id, name, balance
1 | Ann | 120.00 | unchanged
2 | Bob | 25.50 | modified
3 | Cy | 0.00 | added
balance over 20.00: Ann, Bob
rows to save: 2
```

- Verdict: stdlib. `javax.sql.rowset.CachedRowSet` is a record set of the standard library: it keeps the rows of a `ResultSet` in memory and can write the changes back. This example writes a small class, because the test image has no database driver.
- The cells are `Object`, so the code casts them, as JDBC code does with `getObject`.
