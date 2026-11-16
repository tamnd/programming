```java run id=format-and-formatted
import java.util.List;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        String name = "Ada";
        double price = 4.5;
        List<String> items = List.of("tea", "milk");

        System.out.println(String.format(Locale.ROOT, "%s pays %.2f for %d items.", name, price * 2, items.size()));
        System.out.println("%s has %d items.".formatted(name, items.size()));
        System.out.println(name + " pays " + price * 2 + " for " + items.size() + " items.");
        System.out.println("{name} and ${name} are text.");
    }
}
```

```output for=format-and-formatted
Ada pays 9.00 for 2 items.
Ada has 2 items.
Ada pays 9.0 for 2 items.
{name} and ${name} are text.
```

- Verdict: idiom. Java has no string interpolation, so braces and `${...}` in a string literal are text. `String.format` and the method `formatted` take a format string with `%s`, `%d` and `%.2f` and the values after it. The operator `+` joins strings and converts the other values with their default text, so `9.0` has no format.
