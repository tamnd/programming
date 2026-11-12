```java run id=one-place
import java.util.List;

public class Main {
    static final class Section {
        private final String name;
        private long priceCents; // moved here from Seat
        Section(String name, long priceCents) { this.name = name; this.priceCents = priceCents; }
        long priceCents() { return priceCents; }
        void setPriceCents(long c) { priceCents = c; }
    }

    record Seat(String row, int number, Section section) {
        long priceCents() { return section.priceCents(); }
    }

    public static void main(String[] args) {
        Section stalls = new Section("stalls", 4500);
        List<Seat> seats = List.of(new Seat("A", 1, stalls), new Seat("A", 2, stalls), new Seat("B", 7, stalls));
        stalls.setPriceCents(4900);
        for (Seat s : seats) if (s.priceCents() != 4900) throw new AssertionError(s);
        System.out.println("move field: tests pass");
    }
}
```

```output
move field: tests pass
```
