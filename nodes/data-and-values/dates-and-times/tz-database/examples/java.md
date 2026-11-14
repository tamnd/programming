```java run id=zone-rules
import java.time.Instant;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.zone.ZoneRules;

public class Main {
    public static void main(String[] args) {
        ZoneId chicago = ZoneId.of("America/Chicago");
        ZoneRules rules = chicago.getRules();
        // The rules hold the whole history of the zone: local mean time, a year of EST, and today's rules.
        for (String iso : new String[] {"1880-01-01T12:00:00Z", "1936-06-01T12:00:00Z", "2024-07-01T12:00:00Z"}) {
            Instant t = Instant.parse(iso);
            System.out.println(ZonedDateTime.ofInstant(t, chicago).toLocalDateTime() + " " + rules.getOffset(t));
        }
        // The next change after a given instant, as the tz data gives it.
        System.out.println(rules.nextTransition(Instant.parse("2024-01-01T00:00:00Z")));
        // US/Central is a link to America/Chicago in the tz data.
        Instant t = Instant.parse("2024-07-01T12:00:00Z");
        System.out.println(ZoneId.of("US/Central").getRules().getOffset(t).equals(rules.getOffset(t)));
        System.out.println(ZoneId.getAvailableZoneIds().contains("America/Chicago"));
    }
}
```

```output
1880-01-01T06:09:24 -05:50:36
1936-06-01T07:00 -05:00
2024-07-01T07:00 -05:00
Transition[Gap at 2024-03-10T02:00-06:00 to -05:00]
true
true
```

- Verdict: stdlib. The JDK compiles the tz data into its own file, so the version of the data changes with updates of the JDK, not of the system. `ZoneRules` gives the offset for an instant and the next or previous transition. `ZoneId.of` accepts link names such as `US/Central`.
