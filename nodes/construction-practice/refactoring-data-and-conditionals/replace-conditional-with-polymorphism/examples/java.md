```java run id=each-case-its-code
public class Main {
    sealed interface Message permits Email, Sms, Push {}
    record Email(String text) implements Message {}
    record Sms(String text, boolean urgent) implements Message {}
    record Push(boolean urgent) implements Message {}

    // The switch covers every permitted type, so it needs no default.
    // A type added to the permits list and not to the switch fails to compile.
    static int cost(Message m) {
        return switch (m) {
            case Email e -> 0;
            case Sms s -> (s.text().length() + 159) / 160 * 7;
            case Push p -> p.urgent() ? 1 : 0;
        };
    }

    public static void main(String[] args) {
        if (cost(new Email("hi")) != 0) throw new AssertionError("email");
        if (cost(new Sms("x".repeat(161), false)) != 14) throw new AssertionError("sms");
        if (cost(new Push(true)) != 1) throw new AssertionError("push");
        System.out.println("replace conditional with polymorphism: tests pass");
    }
}
```

```output
replace conditional with polymorphism: tests pass
```
