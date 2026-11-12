```java run id=parameter-object
public class Main {
    // Before: six positional parameters.
    static long stayPriceBefore(int startDay, int endDay, long nightlyRate, long breakfastRate,
                                int guests, boolean withBreakfast) {
        int nights = endDay - startDay;
        long price = nights * nightlyRate;
        if (withBreakfast) price += (long) nights * guests * breakfastRate;
        return price;
    }

    // After: a date range, the room as a whole, and a stay that holds the rest.
    record DateRange(int start, int end) {
        DateRange {
            if (end < start) throw new IllegalArgumentException("end before start");
        }
        int nights() { return end - start; }
    }

    record Room(long nightlyRate, long breakfastRate) {}

    record Stay(DateRange range, int guests, boolean breakfast) {}

    static long stayPrice(Stay stay, Room room) {
        int n = stay.range().nights();
        long breakfast = stay.breakfast() ? (long) n * stay.guests() * room.breakfastRate() : 0;
        return n * room.nightlyRate() + breakfast;
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        check(stayPriceBefore(10, 13, 8000, 1500, 2, true) == 33000, "before");
        check(stayPriceBefore(10, 13, 1500, 8000, 2, true) == 52500, "swapped, and no error");
        System.out.println("stayPriceBefore: tests pass");

        Room room = new Room(8000, 1500);
        Stay stay = new Stay(new DateRange(10, 13), 2, true);
        check(stayPrice(stay, room) == 33000, "after");
        check(stayPrice(new Stay(stay.range(), 2, false), room) == 24000, "no breakfast");
        System.out.println("stayPrice: tests pass");
    }
}
```

```output
stayPriceBefore: tests pass
stayPrice: tests pass
```

- A record declares its fields, its constructor and its accessors in one line. The compact constructor of `DateRange` checks the range once, where the old version had no place to put the check.
- `stayPrice(room, stay)`, with the arguments swapped, does not compile, because a `Room` is not a `Stay`. The swap of two `long` values in the old version compiles.
- Java has no named arguments, so `new Room(8000, 1500)` still depends on the order. The risk is now in one constructor call, not in each call to the price function.
