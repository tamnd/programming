```java run id=error-java
public class Main {
    public static void main(String[] args) {
        for (String text : new String[] {"42", "x42"}) {
            try {
                int n = Integer.parseInt(text);
                System.out.println(text + " -> " + n);
            } catch (NumberFormatException e) {
                System.out.println(text + " -> error (" + e.getClass().getSimpleName() + ")");
            }
        }
    }
}
```

```output for=error-java
42 -> 42
x42 -> error (NumberFormatException)
```

- Verdict: native. Java reports errors with exceptions. `Integer.parseInt` throws `NumberFormatException`, an unchecked exception, so the compiler does not make the caller catch it. A checked exception, such as `IOException`, must be caught or declared.
