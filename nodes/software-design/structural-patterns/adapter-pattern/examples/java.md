```java run id=thermometer
import java.util.Locale;

public class Main {
    interface Thermometer {
        double celsius();
    }

    static void report(String name, Thermometer sensor) {
        System.out.println(String.format(Locale.ROOT, "%s: %.1f C", name, sensor.celsius()));
    }

    record RoomSensor(double value) implements Thermometer {
        public double celsius() {
            return value;
        }
    }

    // The vendor class. The program cannot change it.
    static final class LegacyThermometer {
        private final int tenths;

        LegacyThermometer(int tenths) {
            this.tenths = tenths;
        }

        int fahrenheitTenths() {
            return tenths;
        }
    }

    static final class LegacyAdapter implements Thermometer {
        private final LegacyThermometer legacy;

        LegacyAdapter(LegacyThermometer legacy) {
            this.legacy = legacy;
        }

        public double celsius() {
            return (legacy.fahrenheitTenths() / 10.0 - 32) * 5 / 9;
        }
    }

    public static void main(String[] args) {
        report("lab", new RoomSensor(21.5));
        report("patient", new LegacyAdapter(new LegacyThermometer(986)));
        LegacyThermometer hall = new LegacyThermometer(704);
        report("hall", () -> (hall.fahrenheitTenths() / 10.0 - 32) * 5 / 9);
    }
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. `LegacyAdapter` is an object adapter: it implements the target interface and holds the adaptee.
- `Thermometer` has one abstract method, so a lambda can implement it. The third call uses a lambda as the adapter.
- `10.0` makes the division use `double`. With `10`, Java divides integers and drops the tenths.
- `Locale.ROOT` fixes the decimal point. The default locale can print a comma.
