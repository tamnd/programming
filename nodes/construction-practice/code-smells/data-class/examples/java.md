```java run id=move-the-rules-in
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;
import java.util.List;

public class Main {
    // Before: a data class, and the rules in the code that uses it.
    static final class SubscriptionBefore {
        private LocalDate start, end;
        SubscriptionBefore(LocalDate start, LocalDate end) { this.start = start; this.end = end; }
        LocalDate getStart() { return start; }
        LocalDate getEnd() { return end; }
        void setEnd(LocalDate end) { this.end = end; }
    }

    static boolean billingActive(SubscriptionBefore s, LocalDate day) {
        return !day.isBefore(s.getStart()) && day.isBefore(s.getEnd());
    }

    static long reminderDaysLeft(SubscriptionBefore s, LocalDate day) {
        return Math.max(0, ChronoUnit.DAYS.between(day, s.getEnd()));
    }

    // After: a record that checks its rule and owns its behaviour.
    record Subscription(LocalDate start, LocalDate end) {
        Subscription {
            if (!end.isAfter(start)) throw new IllegalArgumentException("end must be after start");
        }

        boolean isActive(LocalDate day) { return !day.isBefore(start) && day.isBefore(end); }
        long daysLeft(LocalDate day) { return Math.max(0, ChronoUnit.DAYS.between(day, end)); }
        Subscription renewed(int days) { return new Subscription(start, end.plusDays(days)); }
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        LocalDate jan1 = LocalDate.of(2026, 1, 1), feb1 = LocalDate.of(2026, 2, 1);
        SubscriptionBefore old = new SubscriptionBefore(jan1, feb1);
        Subscription sub = new Subscription(jan1, feb1);
        for (LocalDate day : List.of(LocalDate.of(2025, 12, 31), LocalDate.of(2026, 1, 20), feb1)) {
            check(sub.isActive(day) == billingActive(old, day), "active " + day);
            check(sub.daysLeft(day) == reminderDaysLeft(old, day), "days left " + day);
        }
        old.setEnd(old.getEnd().plusDays(30));
        Subscription next = sub.renewed(30);
        check(next.daysLeft(feb1) == reminderDaysLeft(old, feb1) && next.daysLeft(feb1) == 30, "renewed");
        check(sub.daysLeft(feb1) == 0, "old value unchanged");
        try {
            new Subscription(feb1, jan1);
            throw new AssertionError("the rule must be checked");
        } catch (IllegalArgumentException expected) {
        }
        System.out.println("subscription: tests pass");
    }
}
```

```output
subscription: tests pass
```

- The compact constructor `Subscription { ... }` runs before the fields are set, so every record of this type has an end after its start.
- A record's fields are final and it has no setters, so `renewed` returns a record and leaves the original as it was.
- `LocalDate` is itself an immutable value type, so `plusDays` also returns a value.
