```java run id=one-object-per-row
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class Main {
    static final class Person {
        final long id;
        String name, city;

        Person(long id, String name, String city) {
            this.id = id;
            this.name = name;
            this.city = city;
        }
    }

    record PersonRow(long id, String name, String city) {}

    // A stand-in for the people table, because the JDK has JDBC but no database driver.
    // Each call of select() stands for one SELECT statement.
    static final class Database {
        final List<PersonRow> rows = List.of(new PersonRow(1, "Ann", "Oslo"), new PersonRow(2, "Bo", "Oslo"), new PersonRow(3, "Cy", "Bergen"));
        int selects;

        List<PersonRow> select(Predicate<PersonRow> where) {
            selects++;
            return rows.stream().filter(where).toList();
        }
    }

    static final class PersonMapper {
        private final Database db;
        private final Map<Long, Person> identityMap = new HashMap<>(); // primary key -> the one object for that row

        PersonMapper(Database db) {
            this.db = db;
        }

        Optional<Person> find(long id) {
            var known = identityMap.get(id);
            if (known != null) return Optional.of(known); // no query
            return db.select(r -> r.id() == id).stream().findFirst().map(this::load);
        }

        List<Person> findByCity(String city) {
            // The map is keyed by id, so a query by city must run.
            return db.select(r -> r.city().equals(city)).stream().map(this::load).toList();
        }

        private Person load(PersonRow row) {
            return identityMap.computeIfAbsent(row.id(), id -> new Person(id, row.name(), row.city()));
        }
    }

    public static void main(String[] args) {
        var db = new Database();
        var mapper = new PersonMapper(db); // one mapper, and one map, for each session
        var a = mapper.find(1).orElseThrow();
        var b = mapper.find(1).orElseThrow();
        System.out.println("a == b: " + (a == b) + " | selects: " + db.selects);

        var oslo = mapper.findByCity("Oslo");
        System.out.println("Oslo: " + oslo.stream().map(p -> p.name).collect(Collectors.joining(", "))
                + " | oslo.get(0) == a: " + (oslo.get(0) == a) + " | selects: " + db.selects);

        a.city = "Rome"; // a change in memory, not saved yet
        System.out.println("b.city: " + b.city);
        System.out.println("Oslo again: " + mapper.findByCity("Oslo").stream()
                .map(p -> p.name + " in " + p.city).collect(Collectors.joining(", ")));
        System.out.println("another session: same object: " + (new PersonMapper(db).find(1).orElseThrow() == a));
    }
}
```

```output
a == b: true | selects: 1
Oslo: Ann, Bo | oslo.get(0) == a: true | selects: 2
b.city: Rome
Oslo again: Ann in Rome, Bo in Oslo
another session: same object: false
```

- Verdict: idiom. `computeIfAbsent` makes the object only for a key that is not yet in the map, and `==` on two references compares identity.
- The map belongs to one `PersonMapper`, so the mapper of another session loads a second object for the same row, as the last line shows.
