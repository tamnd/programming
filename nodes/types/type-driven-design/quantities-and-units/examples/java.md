```java run id=duration-and-unit-records
import java.time.Duration;
import java.time.temporal.ChronoUnit;

public class Main {
    record Metres(double value) {
        Metres plus(Metres o) { return new Metres(value + o.value); }
    }

    record Feet(double value) {
        Metres toMetres() { return new Metres(value * 0.3048); }
    }

    static Metres climb(Metres altitude, Metres by) { return altitude.plus(by); }

    public static void main(String[] args) {
        Duration timeout = Duration.ofSeconds(90).plusMillis(500);
        System.out.println(timeout + " = " + timeout.toMillis() + " ms");
        System.out.println("2 min / 30 s = " + Duration.ofMinutes(2).dividedBy(Duration.ofSeconds(30)));
        System.out.println("one day in hours: " + ChronoUnit.DAYS.getDuration().toHours());
        // climb(new Metres(1000), new Feet(500)) does not compile
        System.out.println(climb(new Metres(1000), new Feet(500).toMetres()));
    }
}
```

```output for=duration-and-unit-records
PT1M30.5S = 90500 ms
2 min / 30 s = 4
one day in hours: 24
Metres[value=1152.4]
```

- Verdict: library. `java.time.Duration` is a unit type for time in the standard library, and `ChronoUnit` names the units. For other quantities, a record per unit makes the compiler reject a `Feet` where a `Metres` is expected, and the conversion is an explicit method. The Units of Measurement API, JSR 385, with the Indriya implementation, gives general quantities and dimensions.
