```java run id=null-logger
interface Logger {
    void log(String message);
}

class ConsoleLogger implements Logger {
    public void log(String message) {
        System.out.println("log: " + message);
    }
}

class NullLogger implements Logger {
    public void log(String message) {} // do nothing
}

class Checkout {
    private final Logger logger;

    Checkout(Logger logger) {
        this.logger = logger;
    }

    String pay(int amount) {
        logger.log("paid " + amount); // no check for null
        return "receipt: " + amount;
    }
}

public class Main {
    public static void main(String[] args) {
        System.out.println(new Checkout(new ConsoleLogger()).pay(10));
        System.out.println(new Checkout(new NullLogger()).pay(20));
    }
}
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. `NullLogger` implements the `Logger` interface with an empty method.
