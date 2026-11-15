```java run id=graph-in-one-column
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static final class Department {
        final String name;
        final List<Department> children = new ArrayList<>();

        Department(String name, Department... children) {
            this.name = name;
            this.children.addAll(List.of(children));
        }
    }

    record Row(String name, String departments) {}

    // A stand-in for the database: the customer table, with the departments as one text column.
    static final Map<Integer, Row> CUSTOMER = new TreeMap<>();

    // The standard library has no JSON, so a small writer and reader handle this one shape.
    static void write(List<Department> ds, StringBuilder sb) {
        sb.append('[');
        for (int i = 0; i < ds.size(); i++) {
            if (i > 0) sb.append(',');
            Department d = ds.get(i);
            sb.append("{\"name\":\"").append(d.name.replace("\\", "\\\\").replace("\"", "\\\"")).append("\",\"children\":");
            write(d.children, sb);
            sb.append('}');
        }
        sb.append(']');
    }

    static final class Reader {
        final String s;
        int i;

        Reader(String s) {
            this.s = s;
        }

        void expect(String t) {
            if (!s.startsWith(t, i)) throw new IllegalArgumentException("expected " + t + " at " + i);
            i += t.length();
        }

        String string() {
            expect("\"");
            StringBuilder sb = new StringBuilder();
            while (s.charAt(i) != '"') {
                if (s.charAt(i) == '\\') i++;
                sb.append(s.charAt(i++));
            }
            i++;
            return sb.toString();
        }

        List<Department> list() {
            List<Department> out = new ArrayList<>();
            expect("[");
            while (s.charAt(i) != ']') {
                if (!out.isEmpty()) expect(",");
                expect("{\"name\":");
                Department d = new Department(string());
                expect(",\"children\":");
                d.children.addAll(list());
                expect("}");
                out.add(d);
            }
            i++;
            return out;
        }
    }

    // The whole graph of departments is written as one value in one column.
    static int save(int id, String name, List<Department> departments) {
        StringBuilder sb = new StringBuilder();
        write(departments, sb);
        CUSTOMER.put(id, new Row(name, sb.toString()));
        return sb.toString().getBytes(StandardCharsets.UTF_8).length;
    }

    static void paths(List<Department> ds, String prefix, List<String> out) {
        for (Department d : ds) {
            out.add(prefix + d.name);
            paths(d.children, prefix + d.name + "/", out);
        }
    }

    static List<String> ancestors(List<Department> ds, String target, List<String> above) {
        for (Department d : ds) {
            if (d.name.equals(target)) {
                List<String> found = new ArrayList<>(above);
                Collections.reverse(found);
                return found;
            }
            List<String> next = new ArrayList<>(above);
            next.add(d.name);
            List<String> found = ancestors(d.children, target, next);
            if (found != null) return found;
        }
        return null;
    }

    public static void main(String[] args) {
        List<Department> tree = List.of(
                new Department("Sales", new Department("Presales"), new Department("Key accounts")),
                new Department("Support"));
        int size = save(1, "Acme", tree);
        System.out.println("saved Acme: " + size + " bytes in one column");
        System.out.println("column: " + CUSTOMER.get(1).departments());
        Row row = CUSTOMER.get(1);
        List<Department> loaded = new Reader(row.departments()).list();
        List<String> ps = new ArrayList<>();
        paths(loaded, "", ps);
        System.out.println("loaded " + row.name() + ": " + String.join(" | ", ps));
        System.out.println("ancestors of Presales: " + String.join(", ", ancestors(loaded, "Presales", List.of())));
        loaded.get(1).children.add(new Department("Field"));
        size = save(1, row.name(), loaded);
        System.out.println("add Support/Field: " + size + " bytes written again");
        System.out.println("rows in customer: " + CUSTOMER.size());
    }
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

- Verdict: idiom. The Java standard library has no JSON, so a small writer and a recursive reader handle the one shape of the tree. A program with a library such as Jackson would call it in `save` and `load`.
- With Hibernate, an attribute with `@JdbcTypeCode(SqlTypes.JSON)` is saved as one JSON value, and Hibernate calls the JSON library.
