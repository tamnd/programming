```java run id=propagate-java
public class Main {
    static class ConfigException extends Exception {
        ConfigException(String message, Throwable cause) {
            super(message, cause);
        }
    }

    static int parsePort(String text) {
        if (!text.matches("[0-9]+")) {
            throw new IllegalArgumentException("not a number: " + text);
        }
        return Integer.parseInt(text);
    }

    // The checked exception must be declared, so each caller sees it.
    static int loadConfig(String line) throws ConfigException {
        String value = line.substring(line.indexOf('=') + 1);
        try {
            return parsePort(value);
        } catch (IllegalArgumentException e) {
            throw new ConfigException("config line '" + line + "'", e);
        }
    }

    public static void main(String[] args) {
        for (String line : new String[] {"port=8080", "port=x"}) {
            try {
                System.out.println("port " + loadConfig(line));
            } catch (ConfigException e) {
                System.out.println("error: " + e.getMessage() + ": " + e.getCause().getMessage());
            }
        }
    }
}
```

```output for=propagate-java
port 8080
error: config line 'port=x': not a number: x
```

- Verdict: native. An exception goes up the stack to the nearest matching `catch`. A checked exception such as `ConfigException` must appear in the `throws` clause of each method that lets it pass. The constructor argument `cause` keeps the original exception.
