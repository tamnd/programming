```java run id=recoverable-java
public class Main {
    static int port;
    static String note;

    static void portOrDefault(String text, int fallback) {
        // A bad fallback is a bug in the caller: stop.
        if (fallback < 1 || fallback > 65535) {
            throw new AssertionError("fallback out of range");
        }
        // Bad text is an expected error: recover.
        try {
            port = Integer.parseInt(text);
            note = "";
        } catch (NumberFormatException e) {
            port = fallback;
            note = " (not a number)";
        }
    }

    public static void main(String[] args) {
        for (String text : new String[] {"8080", "x"}) {
            portOrDefault(text, 80);
            System.out.println(text + " -> " + port + note);
        }
    }
}
```

```output for=recoverable-java
8080 -> 8080
x -> 80 (not a number)
```

- Verdict: native. Java splits `Throwable` into `Exception`, for errors that a program may recover from, and `Error`, for serious problems that a program should not catch. `AssertionError` is an `Error`. `NumberFormatException` is an `Exception` that the code catches.
