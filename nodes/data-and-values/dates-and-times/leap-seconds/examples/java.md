```java run id=parsed-leap-second
import java.time.Duration;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.time.temporal.TemporalAccessor;

public class Main {
    public static void main(String[] args) {
        // ISO_INSTANT accepts the second 60 and gives 23:59:59 of the same day.
        Instant leap = Instant.parse("2016-12-31T23:59:60Z");
        System.out.println(leap);
        TemporalAccessor parsed = DateTimeFormatter.ISO_INSTANT.parse("2016-12-31T23:59:60Z");
        System.out.println(parsed.query(DateTimeFormatter.parsedLeapSecond()));

        // Other parsers reject it.
        try {
            LocalDateTime.parse("2016-12-31T23:59:60");
        } catch (DateTimeParseException e) {
            System.out.println(e.getClass().getSimpleName());
        }

        // The Java Time-Scale has 86400 seconds in each day.
        Instant a = Instant.parse("2016-12-31T23:59:59Z");
        Instant b = Instant.parse("2017-01-01T00:00:00Z");
        System.out.println(Duration.between(a, b).getSeconds() + " " + leap.equals(a));
    }
}
```

```output
2016-12-31T23:59:59Z
true
DateTimeParseException
1 true
```

- Verdict: stdlib. The Java Time-Scale has exactly 86400 seconds in each day. `Instant.parse` accepts the second 60 and returns 23:59:59, and `DateTimeFormatter.parsedLeapSecond()` tells the program that the text had a leap second. Other parsers, such as `LocalDateTime.parse`, reject it.
