```java run id=java-time-types
import java.time.Duration;
import java.time.Instant;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.Period;
import java.time.ZoneId;
import java.time.ZonedDateTime;

public class Main {
  public static void main(String[] args) {
    // An Instant is a point on the time-line.
    Instant i = Instant.parse("2024-03-09T12:00:00Z");
    System.out.println(i + " " + i.getEpochSecond());

    // A LocalDate has no time and no zone. Plus 1 month clamps to the end of February.
    LocalDate jan31 = LocalDate.of(2024, 1, 31);
    System.out.println(jan31.plusMonths(1));

    // Daylight saving time starts in New York at 02:00 on 2024-03-10.
    ZonedDateTime ny = LocalDateTime.of(2024, 3, 9, 12, 0).atZone(ZoneId.of("America/New_York"));
    // A Period of 1 day keeps the wall clock time. A Duration of 24 hours does not.
    System.out.println(ny.plus(Period.ofDays(1)));
    System.out.println(ny.plus(Duration.ofHours(24)));

    System.out.println(Period.between(jan31, LocalDate.of(2024, 3, 1)) + " " + Duration.ofMinutes(90));
  }
}
```

```output
2024-03-09T12:00:00Z 1709985600
2024-02-29
2024-03-10T12:00-04:00[America/New_York]
2024-03-10T13:00-04:00[America/New_York]
P1M1D PT1H30M
```

- Verdict: stdlib. The package java.time has Instant, LocalDate, LocalTime, LocalDateTime, ZonedDateTime, OffsetDateTime, Duration and Period. All of them are immutable. The older java.util.Date and Calendar are mutable, and new code does not use them.
