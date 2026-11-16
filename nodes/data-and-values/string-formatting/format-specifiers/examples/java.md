```java run id=formatter
import java.util.IllegalFormatException;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        double x = 3.14159;
        Locale l = Locale.ROOT;
        System.out.println(String.format(l, "[%8.2f] [%-8.2f] [%08.2f] [%+.1f]", x, x, x, x));
        System.out.println(String.format(l, "[%6s] [%-6s] [%,d] [%x] [%#o]", "abc", "abc", 1234567, 255, 8));
        System.out.println(String.format(l, "%2$s %1$s %2$s", "a", "b"));
        try {
            String.format(l, "%d", "three");
        } catch (IllegalFormatException e) {
            System.out.println(e.getClass().getSimpleName());
        }
    }
}
```

```output for=formatter
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [abc   ] [1,234,567] [ff] [010]
b a b
IllegalFormatConversionException
```

- Verdict: stdlib. `java.util.Formatter` reads the specifiers of C, plus `%,d` for digit groups and `%2$s` for an argument by position. A wrong type throws a subclass of `IllegalFormatException` at run time. The formats use the default locale, so the program passes `Locale.ROOT`.
