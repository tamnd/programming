```java run id=period-and-duration
import java.time.Duration;
import java.time.LocalDate;
import java.time.Period;
import java.time.ZoneId;
import java.time.ZonedDateTime;

public class Main {
    public static void main(String[] args) {
        // plusMonths clamps the day to the last valid day of the month.
        LocalDate jan31 = LocalDate.of(2024, 1, 31);
        System.out.println(jan31.plusMonths(1));
        System.out.println(jan31.plusMonths(1).plusMonths(1) + " " + jan31.plusMonths(2));
        System.out.println(LocalDate.of(2024, 2, 29).plusYears(1));

        // Period adds a conceptual day; Duration adds 24 hours.
        ZonedDateTime t = ZonedDateTime.of(2024, 3, 9, 18, 0, 0, 0, ZoneId.of("America/New_York"));
        System.out.println(t.plus(Period.ofDays(1)));
        System.out.println(t.plus(Duration.ofDays(1)));

        // A period between two dates counts calendar fields.
        System.out.println(Period.between(LocalDate.of(2024, 1, 31), LocalDate.of(2024, 3, 1)));
    }
}
```

```output
2024-02-29
2024-03-29 2024-03-31
2025-02-28
2024-03-10T18:00-04:00[America/New_York]
2024-03-10T19:00-04:00[America/New_York]
P1M1D
```

- Verdict: stdlib. `plusMonths` and `plusYears` clamp the day to the last valid day of the month. A `Period` adds calendar fields and keeps the local time, and a `Duration` adds exact seconds. `Period.between` gives years, months and days.
