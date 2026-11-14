```java run id=date-time-formatter
import java.time.Duration;
import java.time.LocalDate;
import java.time.OffsetDateTime;
import java.time.Period;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        LocalDate d = LocalDate.of(2024, 12, 30);
        System.out.println(d.format(DateTimeFormatter.ISO_LOCAL_DATE));
        System.out.println(d.format(DateTimeFormatter.BASIC_ISO_DATE));
        System.out.println(d.format(DateTimeFormatter.ISO_WEEK_DATE));
        System.out.println(d.format(DateTimeFormatter.ISO_ORDINAL_DATE));
        // The pattern letter Y is the week-based year. The letter u is the year.
        System.out.println(d.format(DateTimeFormatter.ofPattern("YYYY-MM-dd", Locale.ROOT)) + " "
                + d.format(DateTimeFormatter.ofPattern("uuuu-MM-dd", Locale.ROOT)));

        OffsetDateTime t = OffsetDateTime.parse("2024-07-09T14:30:05+02:00");
        System.out.println(t.toInstant());
        // ZonedDateTime reads and writes a zone name in brackets after the offset.
        System.out.println(ZonedDateTime.parse("2024-07-09T14:30:05+02:00[Europe/Paris]"));
        // Durations: Duration has hours, minutes and seconds, and Period has years, months and days.
        System.out.println(Duration.parse("PT1H30M").toMinutes() + " " + Period.parse("P1Y2M10D"));
    }
}
```

```output
2024-12-30
20241230
2025-W01-1
2024-365
2025-12-30 2024-12-30
2024-07-09T12:30:05Z
2024-07-09T14:30:05+02:00[Europe/Paris]
90 P1Y2M10D
```

- Verdict: stdlib. `DateTimeFormatter` has constants for the calendar, basic, week and ordinal forms, and `toString()` of each `java.time` type writes ISO 8601. `Duration` and `Period` parse durations. The pattern `YYYY` gives the week-based year, which is 2025 for 2024-12-30.
