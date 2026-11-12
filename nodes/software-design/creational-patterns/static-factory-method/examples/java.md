```java run id=duration
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    static final class Duration {
        private static final Duration ZERO = new Duration(0);
        private static final Pattern TEXT = Pattern.compile("(\\d+)([sm])");
        private final long seconds;

        private Duration(long seconds) { // only the factory methods call it
            this.seconds = seconds;
        }

        static Duration ofSeconds(long seconds) {
            return seconds == 0 ? ZERO : new Duration(seconds); // a cached instance
        }

        static Duration ofMinutes(long minutes) {
            return ofSeconds(minutes * 60);
        }

        static Optional<Duration> parse(String text) {
            Matcher m = TEXT.matcher(text);
            if (!m.matches()) {
                return Optional.empty(); // a constructor cannot return "no object"
            }
            long n = Long.parseLong(m.group(1));
            return Optional.of(ofSeconds(m.group(2).equals("m") ? n * 60 : n));
        }

        @Override
        public String toString() {
            return seconds + " s";
        }
    }

    public static void main(String[] args) {
        System.out.println("ofMinutes(2) = " + Duration.ofMinutes(2));
        System.out.println("ofSeconds(90) = " + Duration.ofSeconds(90));
        System.out.println("zero is cached: " + (Duration.ofSeconds(0) == Duration.ofMinutes(0)));
        System.out.println("parse(\"2m\") = " + Duration.parse("2m").map(Duration::toString).orElse("empty"));
        System.out.println("parse(\"2h\") = " + Duration.parse("2h").map(Duration::toString).orElse("empty"));
    }
}
```

```output
ofMinutes(2) = 120 s
ofSeconds(90) = 90 s
zero is cached: true
parse("2m") = 120 s
parse("2h") = empty
```

- Verdict: idiom. This is the form of Effective Java Item 1: a private constructor and static methods with names such as `of`, `from` and `valueOf`.
- `java.time.Duration` has the same design: `ofSeconds` and `ofMinutes` are static methods, and a zero length returns the constant `Duration.ZERO`.
- Two constructors with the parameter list `(long)` cannot exist in one class. Two static methods with different names can.
