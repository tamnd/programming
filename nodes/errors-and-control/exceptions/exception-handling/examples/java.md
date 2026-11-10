```java run id=ports
import java.util.List;

public class Main {
    static class ConfigException extends Exception {
        ConfigException(String message, Throwable cause) {
            super(message, cause);
        }
    }

    static int parsePort(String text) throws ConfigException {
        int port;
        try {
            port = Integer.parseInt(text);
        } catch (NumberFormatException e) {
            throw new ConfigException("not a number: " + text, e);
        }
        if (port < 1 || port > 65535) {
            throw new ConfigException("out of range: " + port, null);
        }
        return port;
    }

    public static void main(String[] args) {
        int checked = 0;
        for (String text : List.of("8080", "70000", "abc")) {
            try {
                System.out.println("port " + parsePort(text));
            } catch (ConfigException e) {
                Throwable cause = e.getCause();
                String name = cause == null ? "none" : cause.getClass().getSimpleName();
                System.out.println("error: " + e.getMessage() + " (cause: " + name + ")");
            } finally {
                checked++;
            }
        }
        System.out.println("checked " + checked);
    }
}
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: NumberFormatException)
checked 3
```

- Verdict: native. `throw` throws a `Throwable`, and `try` with `catch` and `finally` handles it.
- `ConfigException` extends `Exception`, so it is checked. A call of `parsePort` must catch it or declare it in a `throws` clause.
- `NumberFormatException` is a subclass of `RuntimeException`, so it is unchecked.
- The second argument of the constructor is the cause, which `getCause()` returns.
- In try-with-resources, an exception from `close()` that follows an earlier exception is added to it as a suppressed exception.
