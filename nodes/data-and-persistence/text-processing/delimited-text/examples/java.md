```java run id=split-fields
import java.util.Arrays;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args) {
        // split takes a regular expression and drops trailing empty strings.
        System.out.println(Arrays.toString("a::b:".split(":")));
        // A negative limit keeps them.
        System.out.println(Arrays.toString("a::b:".split(":", -1)));

        // "|" is a regular expression that matches the empty string.
        System.out.println(Arrays.toString("a|b".split("|")));
        // Pattern.quote makes it a plain character.
        System.out.println(Arrays.toString("a|b".split(Pattern.quote("|"))));

        // A limit of 2 keeps the rest of the line in the last field.
        System.out.println(Arrays.toString("k=v=w".split("=", 2)));
    }
}
```

```output for=split-fields
[a, , b]
[a, , b, ]
[a, |, b]
[a, b]
[k, v=w]
```

- Verdict: stdlib. `String.split` takes a regular expression, and with no limit it drops the empty strings at the end, so a record with empty last fields comes back shorter. A negative limit keeps them. A separator such as `|` or `.` must be quoted with `Pattern.quote`.
