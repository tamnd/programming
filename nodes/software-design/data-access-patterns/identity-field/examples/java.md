```java run id=insert-then-update
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class Main {
    record Row(String name, String email) {}

    // A stand-in for the database: a table keyed by id, and a counter like AUTOINCREMENT.
    static final Map<Long, Row> PERSON = new HashMap<>();
    static long lastRowId = 0;
    // A second table keyed by a meaningful value, the email.
    static final Map<String, String> PERSON_BY_EMAIL = new HashMap<>(Map.of("cy@example.com", "Cy"));

    static class Person {
        Long id; // The Identity Field: null until the row exists.
        String name;
        String email;

        Person(String name, String email, Long id) {
            this.name = name;
            this.email = email;
            this.id = id;
        }

        String showId() {
            return id == null ? "none" : id.toString();
        }
    }

    static int save(Person p) {
        if (p.id == null) {
            lastRowId++;
            PERSON.put(lastRowId, new Row(p.name, p.email));
            p.id = lastRowId;
            return 1;
        }
        if (!PERSON.containsKey(p.id)) {
            return 0;
        }
        PERSON.put(p.id, new Row(p.name, p.email));
        return 1;
    }

    static Person find(long id) {
        Row row = PERSON.get(id);
        return new Person(row.name(), row.email(), id);
    }

    public static void main(String[] args) {
        Person ann = new Person("Ann", "ann@example.com", null);
        Person bo = new Person("Bo", "bo@example.com", null);
        System.out.println("new: Ann id=" + ann.showId() + " | Bo id=" + bo.showId());
        save(ann);
        save(bo);
        System.out.println("saved: Ann id=" + ann.showId() + " | Bo id=" + bo.showId());

        ann.email = "ann@new.example";
        int rows = save(ann);
        Row row = PERSON.get(ann.id);
        System.out.println("update by id: rows " + rows + " | " + ann.id + " " + row.name() + " " + row.email());

        Person b = find(2);
        System.out.println("find(2): id=" + b.id + " " + b.name + " " + b.email);

        Person first = find(1);
        Person second = find(1);
        String objects = first == second ? "same" : "different";
        String ids = Objects.equals(first.id, second.id) ? "equal" : "different";
        System.out.println("two loads of row 1: " + objects + " objects, " + ids + " ids");

        // The same change with a meaningful key: the email is the key.
        Person cy = new Person("Cy", "cy@example.com", null);
        cy.email = "cy@new.example";
        int updated = 0;
        if (PERSON_BY_EMAIL.containsKey(cy.email)) {
            PERSON_BY_EMAIL.put(cy.email, cy.name);
            updated = 1;
        }
        System.out.println("update by email key: rows " + updated);
    }
}
```

```output
new: Ann id=none | Bo id=none
saved: Ann id=1 | Bo id=2
update by id: rows 1 | 1 Ann ann@new.example
find(2): id=2 Bo bo@example.com
two loads of row 1: different objects, equal ids
update by email key: rows 0
```

- Verdict: idiom. The key is a `Long` and not a `long`, so `null` can mean that the object is not saved yet. `==` compares references, and `Objects.equals` compares the key values.
- With JDBC, the new key comes from `Statement.RETURN_GENERATED_KEYS` and `getGeneratedKeys()`. Entity Framework Core and JPA set the key field after the insert.
