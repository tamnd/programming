```java run id=instant-epoch-seconds
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneOffset;

public class Main {
  public static void main(String[] args) {
    // An Instant stores seconds since the epoch and a nanosecond part.
    Instant t = Instant.ofEpochSecond(1709985600L);
    System.out.println(t);
    System.out.println(t.getEpochSecond() + " " + t.toEpochMilli());
    System.out.println(Instant.ofEpochMilli(1709985600123L));

    // Fields in UTC to seconds since the epoch.
    LocalDateTime noon = LocalDateTime.of(2024, 3, 9, 12, 0);
    System.out.println(noon.toEpochSecond(ZoneOffset.UTC));

    // The last second of a signed 32-bit count, and what comes after it.
    System.out.println(Instant.ofEpochSecond(Integer.MAX_VALUE));
    int wrapped = Integer.MAX_VALUE + 1;
    System.out.println(wrapped + " " + Instant.ofEpochSecond(wrapped));
  }
}
```

```output
2024-03-09T12:00:00Z
1709985600 1709985600000
2024-03-09T12:00:00.123Z
1709985600
2038-01-19T03:14:07Z
-2147483648 1901-12-13T20:45:52Z
```

- Verdict: stdlib. Instant.ofEpochSecond, Instant.ofEpochMilli, getEpochSecond and toEpochMilli convert between an Instant and a Unix time. System.currentTimeMillis gives the current time in milliseconds as a long.
