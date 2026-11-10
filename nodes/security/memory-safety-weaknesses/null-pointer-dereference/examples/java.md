```java fails expect=exception:NullPointerException id=map-get
import java.util.Map;

public class Main {
    record User(int quota) {}

    static final Map<String, User> USERS = Map.of("alice", new User(10), "bob", new User(20));

    public static void main(String[] args) {
        for (String name : new String[] {"alice", "nobody"}) {
            User u = USERS.get(name); // null when the key is missing
            System.out.println(name + ": quota " + u.quota());
        }
    }
}
```

`Map.get` returns `null` for a missing key. The call `u.quota()` on `null` throws `NullPointerException`.

```java run id=optional-result
import java.util.Map;
import java.util.Optional;

public class Main {
    record User(int quota) {}

    static final Map<String, User> USERS = Map.of("alice", new User(10), "bob", new User(20));

    // The return type says that the user can be absent.
    static Optional<User> findUser(String name) {
        return Optional.ofNullable(USERS.get(name));
    }

    public static void main(String[] args) {
        for (String name : new String[] {"alice", "nobody"}) {
            String line = findUser(name)
                    .map(u -> "quota " + u.quota())
                    .orElse("no such user");
            System.out.println(name + ": " + line);
        }
    }
}
```

```output for=optional-result
alice: quota 10
nobody: no such user
```

- Verdict: stdlib. Every reference type includes `null`, and the compiler does not check that the code tests it. `java.util.Optional` marks a result that can be absent.
- Return `Optional` from a lookup, and do not return `null` from a method whose type is `Optional`.
- Use `Objects.requireNonNull` at the start of a method to fail at the boundary, not deep in the code.
- Unboxing a `null` `Integer` to `int` also throws `NullPointerException`.
