```java run id=date-range
public class Main {
    // Before: each period is two loose numbers.
    static boolean overlapsBefore(int aStart, int aEnd, int bStart, int bEnd) {
        return aStart <= bEnd && bStart <= aEnd;
    }

    static boolean containsBefore(int start, int end, int day) {
        return start <= day && day <= end;
    }

    // After: the clump is a type, and the rules live in it.
    record DateRange(int start, int end) {
        DateRange {
            if (end < start) {
                throw new IllegalArgumentException("range " + start + ".." + end + " ends before it starts");
            }
        }

        boolean contains(int day) {
            return start <= day && day <= end;
        }

        boolean overlaps(DateRange other) {
            return start <= other.end && other.start <= end;
        }
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        check(overlapsBefore(1, 5, 4, 9), "before overlap");
        check(!overlapsBefore(1, 4, 5, 9), "before gap");
        check(!overlapsBefore(1, 5, 9, 4), "before reversed"); // no one is told
        System.out.println("before: a reversed period gives a wrong answer");

        DateRange week = new DateRange(1, 5);
        check(week.overlaps(new DateRange(4, 9)), "overlap");
        check(!week.overlaps(new DateRange(6, 9)), "gap");
        check(week.contains(5) && !week.contains(6), "contains");
        check(containsBefore(1, 5, 5) == week.contains(5), "same result");
        check(week.equals(new DateRange(1, 5)), "equal by value");
        try {
            new DateRange(9, 4);
            throw new AssertionError("a reversed range was accepted");
        } catch (IllegalArgumentException expected) {
            // the check in the compact constructor
        }
        System.out.println("after: tests pass");
    }
}
```

```output
before: a reversed period gives a wrong answer
after: tests pass
```

- The compact constructor `DateRange { ... }` runs before the fields are set, so it is the one place to reject a reversed range.
- A record gets `equals`, `hashCode` and the accessors `start()` and `end()` from its components.
