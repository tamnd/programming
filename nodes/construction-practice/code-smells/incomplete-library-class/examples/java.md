```java run id=write-it-once
import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.temporal.TemporalAdjuster;
import java.util.List;

public class Main {
    // Before: each client writes the missing operation inline.
    static LocalDate dueDateBefore(LocalDate invoiceDate) {
        LocalDate d = invoiceDate.plusDays(1);
        while (d.getDayOfWeek() == DayOfWeek.SATURDAY || d.getDayOfWeek() == DayOfWeek.SUNDAY) {
            d = d.plusDays(1);
        }
        return d;
    }

    static LocalDate shipDateBefore(LocalDate orderDate) {
        LocalDate d = orderDate.plusDays(1);
        while (d.getDayOfWeek() == DayOfWeek.SUNDAY) { // forgot Saturday
            d = d.plusDays(1);
        }
        return d;
    }

    // After, a foreign method: a static method in a utility class, with the
    // library object as its first parameter. It belongs on LocalDate, which is final.
    static final class Dates {
        private Dates() {}

        static boolean isWeekend(LocalDate d) {
            DayOfWeek w = d.getDayOfWeek();
            return w == DayOfWeek.SATURDAY || w == DayOfWeek.SUNDAY;
        }

        static LocalDate nextWorkday(LocalDate d) {
            LocalDate next = d.plusDays(1);
            while (isWeekend(next)) {
                next = next.plusDays(1);
            }
            return next;
        }

        // The library has a hook for this: a TemporalAdjuster, which LocalDate.with accepts.
        static final TemporalAdjuster NEXT_WORKDAY = t -> nextWorkday(LocalDate.from(t));
    }

    public static void main(String[] args) {
        List<LocalDate[]> cases = List.of(
                new LocalDate[] {LocalDate.of(2026, 11, 13), LocalDate.of(2026, 11, 16)},
                new LocalDate[] {LocalDate.of(2026, 11, 14), LocalDate.of(2026, 11, 16)},
                new LocalDate[] {LocalDate.of(2026, 11, 17), LocalDate.of(2026, 11, 18)});
        for (LocalDate[] c : cases) {
            check(Dates.nextWorkday(c[0]).equals(c[1]), "nextWorkday " + c[0]);
            check(Dates.nextWorkday(c[0]).equals(dueDateBefore(c[0])), "same as before " + c[0]);
            check(c[0].with(Dates.NEXT_WORKDAY).equals(c[1]), "adjuster " + c[0]);
        }
        check(shipDateBefore(LocalDate.of(2026, 11, 13)).equals(LocalDate.of(2026, 11, 14)), "the copy ships on a Saturday");
        System.out.println("nextWorkday: tests pass");
    }

    static void check(boolean ok, String what) {
        if (!ok) {
            throw new AssertionError(what);
        }
    }
}
```

```output
nextWorkday: tests pass
```

`LocalDate` is a final class, so a local extension by subclass is not possible, and a wrapper would need a conversion at each call into `java.time`. The library gives a better place: `LocalDate.with` takes a `TemporalAdjuster`, an interface with one method, so the missing operation can be a value that the library itself knows how to apply. The class `TemporalAdjusters` holds the adjusters that come with the library, such as `next(DayOfWeek)`.
