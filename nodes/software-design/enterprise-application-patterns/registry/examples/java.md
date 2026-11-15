```java run id=person-registry
import java.util.Map;

public class Main {
    interface PersonFinder {
        String find(int personId);
    }

    static class TablePersonFinder implements PersonFinder {
        private final Map<Integer, String> rows;
        TablePersonFinder(Map<Integer, String> rows) { this.rows = rows; }
        public String find(int personId) { return rows.get(personId); }
    }

    // A well-known object with one explicit method for each service.
    static class Registry {
        private static volatile Registry sole = new Registry(null);
        private final PersonFinder personFinder;

        Registry(PersonFinder personFinder) { this.personFinder = personFinder; }

        // The start of the program, or a test, sets the registry.
        static void install(Registry registry) { sole = registry; }

        static PersonFinder personFinder() {
            PersonFinder f = sole.personFinder;
            if (f == null) throw new IllegalStateException("no person finder is registered");
            return f;
        }
    }

    // Code deep in the program, with no reference to a finder.
    static String greet(int personId) { return "hello, " + Registry.personFinder().find(personId); }

    public static void main(String[] args) {
        Registry.install(new Registry(new TablePersonFinder(Map.of(1, "ann", 2, "bob"))));
        System.out.println(greet(1));
        System.out.println(greet(2));

        Registry.install(new Registry(id -> "test person " + id)); // a test installs a stub
        System.out.println("in a test: " + greet(1));

        Registry.install(new Registry(null));
        try {
            System.out.println(greet(1));
        } catch (IllegalStateException e) {
            System.out.println("with an empty registry: " + e.getMessage());
        }
    }
}
```

```output
hello, ann
hello, bob
in a test: hello, test person 1
with an empty registry: no person finder is registered
```

- Verdict: idiom. A `static volatile` field holds the sole instance, so a new instance that one thread installs is visible to other threads. The static methods are the interface of the registry.
- The stub finder is a lambda, because `PersonFinder` has one abstract method.
