```java run id=redacting-logger
interface Value {
    String value();
}

interface Sensitive {}

record Username(String value) implements Value {}

record Password(String value) implements Value, Sensitive {}

record ApiKey(String value) implements Value, Sensitive {}

public class Main {
    static void log(String label, Value item) {
        String shown = item instanceof Sensitive ? "[redacted]" : item.value();
        System.out.println(label + ": " + shown);
    }

    public static void main(String[] args) {
        log("user", new Username("ann"));
        log("password", new Password("hunter2"));
        log("api key", new ApiKey("k-123"));
    }
}
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. `Sensitive` declares no methods, as `java.io.Serializable` does, and `instanceof` tests it.
- A method can also take a `Sensitive` parameter, so that the compiler rejects an unmarked argument.
