```java run id=either
public class Main {
    static String describe(Object value) {
        return switch (value) {
            case Integer n -> "number " + n + ", doubled " + (n * 2);
            case String s -> "text " + s + ", upper " + s.toUpperCase();
            default -> throw new IllegalArgumentException("not a number or a string");
        };
    }

    public static void main(String[] args) {
        for (Object v : new Object[] {21, "ok"}) {
            System.out.println(describe(v));
        }
    }
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
```

- Verdict: idiom. Java has no union types. The parameter has the type `Object`, and a `switch` with type patterns finds the actual type.
- The compiler cannot know that only `Integer` and `String` arrive, so the `switch` needs a `default` branch.
- A sealed interface with one record for each case gives a closed set that the compiler checks. That is a sum type.
