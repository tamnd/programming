```java run id=port
import java.util.List;
import java.util.Map;
import java.util.Optional;

public class Main {
    sealed interface Result<T> permits Ok, Err {}

    record Ok<T>(T value) implements Result<T> {}

    record Err<T>(String message, boolean missing) implements Result<T> {}

    static Optional<String> lookup(Map<String, String> config, String key) {
        return Optional.ofNullable(config.get(key));
    }

    static Result<Integer> portSetting(Map<String, String> config) {
        Optional<String> text = lookup(config, "port");
        if (text.isEmpty()) {
            return new Err<>("missing key \"port\"", true);
        }
        try {
            int n = Integer.parseInt(text.get());
            if (n >= 1 && n <= 65535) {
                return new Ok<>(n);
            }
        } catch (NumberFormatException e) {
            // fall through to the error value
        }
        return new Err<>("invalid port \"" + text.get() + "\"", false);
    }

    public static void main(String[] args) {
        List<Map<String, String>> configs = List.of(Map.of("port", "8080"), Map.of("port", "x"), Map.of());
        for (Map<String, String> config : configs) {
            String line = switch (portSetting(config)) {
                case Ok<Integer>(Integer port) -> "port " + port;
                case Err<Integer>(String message, boolean missing) when missing -> "use the default 80";
                case Err<Integer>(String message, boolean missing) -> "error: " + message;
            };
            System.out.println(line);
        }
    }
}
```

```output
port 8080
error: invalid port "x"
use the default 80
```

- Verdict: idiom. Java reports errors with exceptions. A sealed interface with two records gives a result type, and a `switch` with record patterns takes it apart.
- The `switch` has no `default` branch. The compiler accepts it because the sealed interface permits only `Ok` and `Err`, and it rejects the `switch` if a permitted class is not covered.
- `Optional<T>` from `java.util` is the standard form for a value that may be absent. It carries no reason for the absence.
- `Integer.parseInt` throws `NumberFormatException`, so the code catches the exception at the boundary and turns it into an error value.
