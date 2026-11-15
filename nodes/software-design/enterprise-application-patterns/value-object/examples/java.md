```java run id=date-range-value
import java.time.LocalDate;
import java.util.HashMap;
import java.util.Map;

public class Main {
    // A record gets equals, hashCode and toString from its components, and its fields are final.
    record DateRange(LocalDate start, LocalDate end) {
        DateRange {
            if (end.isBefore(start)) throw new IllegalArgumentException("the end is before the start");
        }

        boolean contains(LocalDate day) {
            return !day.isBefore(start) && !day.isAfter(end);
        }

        DateRange extendTo(LocalDate newEnd) {
            return new DateRange(start, newEnd); // a new value; this does not change
        }

        @Override
        public String toString() {
            return start + ".." + end;
        }
    }

    static String yn(boolean b) { return b ? "yes" : "no"; }

    public static void main(String[] args) {
        DateRange a = new DateRange(LocalDate.of(2026, 3, 1), LocalDate.of(2026, 3, 10));
        DateRange b = new DateRange(LocalDate.of(2026, 3, 1), LocalDate.of(2026, 3, 10));
        System.out.println("a equals b: " + yn(a.equals(b)));
        System.out.println("a and b are the same object: " + yn(a == b));

        DateRange c = a.extendTo(LocalDate.of(2026, 3, 15));
        System.out.println("a: " + a);
        System.out.println("c: " + c);

        Map<DateRange, String> rooms = new HashMap<>();
        rooms.put(a, "room 4");
        System.out.println("the room for b: " + rooms.get(b));
        System.out.println("a contains 2026-03-05: " + yn(a.contains(LocalDate.of(2026, 3, 5))));
        System.out.println("a contains 2026-03-12: " + yn(a.contains(LocalDate.of(2026, 3, 12))));

        try {
            new DateRange(LocalDate.of(2026, 3, 10), LocalDate.of(2026, 3, 1));
        } catch (IllegalArgumentException e) {
            System.out.println("error: " + e.getMessage());
        }
    }
}
```

```output
a equals b: yes
a and b are the same object: no
a: 2026-03-01..2026-03-10
c: 2026-03-01..2026-03-15
the room for b: room 4
a contains 2026-03-05: yes
a contains 2026-03-12: no
error: the end is before the start
```

- Verdict: native. A record has final fields and gets `equals`, `hashCode` and `toString` from its components. The compact constructor checks the rule of the value.
- `==` compares references, so the program compares values with `equals`. `LocalDate` is itself a value object.
