```java run id=query-object-to-sql
import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Main {
    record Player(int id, String name, String country, int goals) {
        Object get(String field) {
            return switch (field) {
                case "id" -> id;
                case "name" -> name;
                case "country" -> country;
                case "goals" -> goals;
                default -> throw new IllegalArgumentException(field);
            };
        }
    }

    // The metadata: the table and, for each field of Player, its column.
    record DataMap(String table, Map<String, String> columns) {}

    // A node of the query object: it can write itself as SQL or test an object.
    interface Condition {
        String sql(Map<String, String> cols, List<Object> params);
        boolean matches(Player p);
    }

    record Criterion(String op, String field, Object value) implements Condition {
        public String sql(Map<String, String> cols, List<Object> params) {
            String col = cols.get(field);
            if (op.equals("starts")) {
                params.add(value + "%");
                return col + " like ?";
            }
            params.add(value);
            return col + " " + op + " ?";
        }

        public boolean matches(Player p) {
            Object v = p.get(field);
            return switch (op) {
                case "=" -> v.equals(value);
                case ">" -> (Integer) v > (Integer) value;
                default -> ((String) v).startsWith((String) value);
            };
        }
    }

    // A group of conditions joined with "or".
    record AnyOf(List<Condition> parts) implements Condition {
        public String sql(Map<String, String> cols, List<Object> params) {
            List<String> s = new ArrayList<>();
            for (Condition c : parts) s.add(c.sql(cols, params));
            return "(" + String.join(" or ", s) + ")";
        }

        public boolean matches(Player p) {
            return parts.stream().anyMatch(c -> c.matches(p));
        }
    }

    // A query on fields of a class. It knows nothing of tables and columns
    // until toSql reads the data map.
    static class Query {
        final List<Condition> criteria = new ArrayList<>();
        String order;

        Query where(Condition c) {
            criteria.add(c);
            return this;
        }

        Query orderBy(String field) {
            order = field;
            return this;
        }

        String toSql(DataMap map, List<Object> params) {
            Map<String, String> cols = map.columns();
            String sql = "select " + String.join(", ", cols.values()) + " from " + map.table();
            if (!criteria.isEmpty()) {
                List<String> s = new ArrayList<>();
                for (Condition c : criteria) s.add(c.sql(cols, params));
                sql += " where " + String.join(" and ", s);
            }
            if (order != null) sql += " order by " + cols.get(order);
            return sql;
        }

        boolean matches(Player p) {
            return criteria.stream().allMatch(c -> c.matches(p));
        }
    }

    static final List<Player> PLAYERS = List.of(
        new Player(1, "Ann", "NZ", 14), new Player(2, "Bo", "NZ", 3), new Player(3, "Cy", "NZ", 21),
        new Player(4, "Dee", "AU", 9), new Player(5, "Eli", "GB", 12));

    static void run(String title, Query q, DataMap map) {
        List<Object> params = new ArrayList<>();
        String sql = q.toSql(map, params);
        System.out.println("query: " + title);
        System.out.println("sql: " + sql);
        String shown = params.stream()
            .map(p -> p instanceof String ? "'" + p + "'" : String.valueOf(p))
            .collect(Collectors.joining(", "));
        System.out.println("params: [" + shown + "]");
        // No database here: the same query object is run on the objects in memory.
        List<Player> found = PLAYERS.stream().filter(q::matches)
            .sorted(Comparator.comparing(p -> (String) p.get(q.order))).toList();
        System.out.println("rows: " + found.stream()
            .map(p -> p.name() + " (" + p.country() + ", " + p.goals() + ")")
            .collect(Collectors.joining(" | ")));
        System.out.println("in memory: " + found.stream().map(Player::name).collect(Collectors.joining(" | ")));
    }

    static Criterion eq(String f, Object v) { return new Criterion("=", f, v); }
    static Criterion gt(String f, Object v) { return new Criterion(">", f, v); }
    static Criterion starts(String f, String v) { return new Criterion("starts", f, v); }

    public static void main(String[] args) {
        Map<String, String> cols = new LinkedHashMap<>();
        cols.put("id", "id");
        cols.put("name", "name");
        cols.put("country", "nation");
        cols.put("goals", "goals_scored");
        DataMap playerMap = new DataMap("player", cols);

        Query q1 = new Query().where(eq("country", "NZ")).where(gt("goals", 10)).orderBy("name");
        Query q2 = new Query().where(new AnyOf(List.of(starts("name", "B"), eq("country", "AU")))).orderBy("name");
        run("from NZ and more than 10 goals", q1, playerMap);
        run("name starts with B or from AU", q2, playerMap);

        // The column nation is renamed. Only the data map changes; the query does not.
        Map<String, String> newCols = new LinkedHashMap<>(cols);
        newCols.put("country", "country_code");
        run("from NZ and more than 10 goals, after the rename", q1, new DataMap("player", newCols));
    }
}
```

```output
query: from NZ and more than 10 goals
sql: select id, name, nation, goals_scored from player where nation = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
query: name starts with B or from AU
sql: select id, name, nation, goals_scored from player where (name like ? or nation = ?) order by name
params: ['B%', 'AU']
rows: Bo (NZ, 3) | Dee (AU, 9)
in memory: Bo | Dee
query: from NZ and more than 10 goals, after the rename
sql: select id, name, country_code, goals_scored from player where country_code = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
```

- Verdict: idiom. `Condition` is an interface with two records that implement it, so `AnyOf` can hold criteria and other groups.
- The example has no database, so the rows come from the same query object run on the objects in memory.
- The Jakarta Persistence Criteria API is the standard query object of Java. It names attributes through a metamodel, so the compiler checks the field names, which the strings here do not.
