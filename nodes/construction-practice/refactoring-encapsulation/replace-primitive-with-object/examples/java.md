```java run id=give-it-a-type
import java.util.Locale;
import java.util.Set;

public class Main {
    // A record compares by value. The compact constructor checks and normalizes.
    record Email(String value) {
        Email {
            value = value.strip().toLowerCase(Locale.ROOT);
            if (!value.matches("[^@\\s]+@[^@\\s]+")) throw new IllegalArgumentException("not an email: " + value);
        }
        String domain() { return value.substring(value.indexOf('@') + 1); }
    }

    public static void main(String[] args) {
        Email a = new Email(" Ana@Example.org");
        Email b = new Email("ana@example.org");
        if (!a.equals(b) || a == b) throw new AssertionError("expected equal values, distinct objects");
        if (!new Email("Cy@EXAMPLE.org").domain().equals("example.org")) throw new AssertionError("domain");
        if (Set.of(a).size() != 1 || !Set.of(a).contains(b)) throw new AssertionError("hashCode");
        try {
            new Email("not-an-email");
            throw new AssertionError("expected an exception");
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
        System.out.println(a);
        System.out.println("replace primitive with object: tests pass");
    }
}
```

```output
not an email: not-an-email
Email[value=ana@example.org]
replace primitive with object: tests pass
```
