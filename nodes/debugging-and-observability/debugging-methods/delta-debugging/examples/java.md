```java run id=ddmin
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.function.Predicate;

public class Main {
    static int tests = 0;

    // Returns a 1-minimal part of chars on which fails is still true.
    static String ddmin(String chars, Predicate<String> fails) {
        int n = 2;
        while (chars.length() >= 2) {
            int size = chars.length();
            List<String> subsets = new ArrayList<>();
            List<String> complements = new ArrayList<>();
            for (int i = 0; i < n; i++) {
                int lo = i * size / n, hi = (i + 1) * size / n;
                subsets.add(chars.substring(lo, hi));
                complements.add(chars.substring(0, lo) + chars.substring(hi));
            }
            Optional<String> smaller = subsets.stream().filter(fails).findFirst();
            if (smaller.isPresent()) { // reduce to subset
                chars = smaller.get();
                n = 2;
                System.out.println("subset     " + chars.length());
                continue;
            }
            smaller = complements.stream().filter(fails).findFirst();
            if (smaller.isPresent()) { // reduce to complement
                chars = smaller.get();
                n = Math.max(n - 1, 2);
                System.out.println("complement " + chars.length());
                continue;
            }
            if (n == size) { // each single character was removed
                break;
            }
            n = Math.min(size, 2 * n); // increase granularity
        }
        return chars;
    }

    // Stands for one run of the browser: it crashes on a <SELECT ...> tag.
    static boolean browserCrashes(String text) {
        tests++;
        int start = text.indexOf("<SELECT");
        return start >= 0 && text.indexOf('>', start) >= 0;
    }

    public static void main(String[] args) {
        String page = "<td align=left><SELECT NAME=\"priority\" MULTIPLE SIZE=7>";
        String minimal = ddmin(page, Main::browserCrashes);
        System.out.printf("%d characters reduced to %s in %d tests%n", page.length(), minimal, tests);
    }
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

The program counts the tests in a static field. A lambda cannot change a local variable of the method that creates it, so the counter cannot be a local.

```java fails expect=compile id=local-counter
import java.util.function.Predicate;

public class Main {
    public static void main(String[] args) {
        int tests = 0;
        Predicate<String> crashes = text -> {
            tests++;
            return text.contains("<SELECT");
        };
        System.out.println(crashes.test("<SELECT>") + " after " + tests + " tests");
    }
}
```

```output for=local-counter stream=stderr
Main.java:7: error: local variables referenced from a lambda expression must be final or effectively final
            tests++;
            ^
1 error
```

- Verdict: idiom. The Java class library has no delta debugging.
- `Predicate<String>` is the type of the test. `Main::browserCrashes` is a method reference that fits it.
- `stream().filter(fails).findFirst()` is lazy. It tests the parts in order and stops at the first part that fails.
- A lambda can read only local variables that are final or effectively final. A counter must be a field, an array element or an `AtomicInteger`.
