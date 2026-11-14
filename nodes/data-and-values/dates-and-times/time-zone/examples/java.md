```java run id=zone-id-and-rules
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;

public class Main {
  public static void main(String[] args) {
    ZoneId ny = ZoneId.of("America/New_York");
    Instant winter = Instant.parse("2024-01-09T12:00:00Z");
    Instant summer = Instant.parse("2024-07-09T12:00:00Z");
    System.out.println(winter.atZone(ny) + " " + summer.atZone(ny));
    System.out.println(ny.getRules().getOffset(winter) + " " + ny.getRules().getOffset(summer));

    // A gap: the local time moves forward by the length of the gap.
    System.out.println(ZonedDateTime.of(LocalDateTime.of(2024, 3, 10, 2, 30), ny));

    // An overlap: the earlier offset by default, and the later one on request.
    ZonedDateTime o = ZonedDateTime.of(LocalDateTime.of(2024, 11, 3, 1, 30), ny);
    System.out.println(o + " " + o.withLaterOffsetAtOverlap());

    // A region has rules that change. An offset has one fixed rule.
    System.out.println(ny.getRules().isFixedOffset() + " " + ZoneOffset.ofHours(-5).getRules().isFixedOffset());
  }
}
```

```output
2024-01-09T07:00-05:00[America/New_York] 2024-07-09T08:00-04:00[America/New_York]
-05:00 -04:00
2024-03-10T03:30-04:00[America/New_York]
2024-11-03T01:30-04:00[America/New_York] 2024-11-03T01:30-05:00[America/New_York]
false true
```

- Verdict: stdlib. ZoneId names a region of the IANA database, and its subclass ZoneOffset is a fixed offset. ZoneRules gives the offset at an instant and the transitions, and ZonedDateTime resolves gaps and overlaps with documented rules.
