```java run id=optional
import java.util.Map;
import java.util.Optional;

public class Main {
    static Optional<String> email(Map<String, String> emails, String name) {
        return Optional.ofNullable(emails.get(name));
    }

    public static void main(String[] args) {
        Map<String, String> emails = Map.of("ada", "ada@example.org");

        String raw = emails.get("bob");
        System.out.println(raw == null);

        System.out.println(email(emails, "ada").map(String::length).orElse(0));
        System.out.println(email(emails, "bob").orElse("none"));
        System.out.println(email(emails, "bob").isPresent());
    }
}
```

```output
true
15
none
false
```

- Verdict: native. Every reference type includes `null`. A variable of type `String` can hold `null`, and `Map.get` returns `null` for a missing key.
- Field access, method calls and unboxing on `null` throw `NullPointerException`. The message names the expression that was `null`.
- `java.util.Optional<T>` is a library class for a result that may be absent. `ofNullable` converts a possibly `null` value, and `map`, `orElse` and `orElseThrow` work on the result without a `null` test.
- The API documentation says that `Optional` is primarily for method return types, and that a variable of type `Optional` should never itself be `null`.
