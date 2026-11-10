```java run id=from-environment
import java.util.Map;
import java.util.function.Function;

public class Main {
    record Config(String user, String password) {
        // The generated toString of a record prints every field, so replace it.
        @Override
        public String toString() {
            return "user=" + user + " password=<set>";
        }
    }

    // lookup is System::getenv in production.
    static Config loadConfig(Function<String, String> lookup) {
        String password = lookup.apply("EXAMPLE_DB_PASSWORD");
        if (password == null || password.isBlank()) {
            throw new IllegalStateException("EXAMPLE_DB_PASSWORD is not set");
        }
        String user = lookup.apply("EXAMPLE_DB_USER");
        return new Config(user == null ? "app" : user, password);
    }

    public static void main(String[] args) {
        try {
            loadConfig(System::getenv);
        } catch (IllegalStateException e) {
            System.out.println("refused to start: " + e.getMessage());
        }

        Map<String, String> deployment = Map.of("EXAMPLE_DB_PASSWORD", "example-not-a-real-password");
        System.out.println(loadConfig(deployment::get));
    }
}
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
user=app password=<set>
```

`loadConfig` takes a `Function`, so production passes `System::getenv` and a test passes `Map::get`. The record overrides `toString`, so a log line shows that the password is set but not its value.

- Verdict: stdlib. `System.getenv(name)` returns `null` for a variable that is not set. The task [[read-an-environment-variable]] covers the details.
- A string literal in Java code is in the constant pool of the class file. The `javap` tool and any decompiler show it.
- Do not put secrets in `application.properties` or other resources in the JAR. Frameworks can read a property from an environment variable or a secret store at run time.
- Some APIs take a password as a `char[]`, so that the program can clear the array after use. A `String` stays in memory until the garbage collector frees it.
