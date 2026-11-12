```java run id=validated-records
import java.util.List;

public class Main {
    // A refinement checked once, in the compact constructor
    record Percent(int value) {
        Percent {
            if (value < 0 || value > 100)
                throw new IllegalArgumentException(value + " is not in 0..100");
        }
    }

    // Non-empty by construction: first() needs no check and cannot fail
    record NonEmptyList<T>(T head, List<T> tail) {
        T first() { return head; }
        int size() { return 1 + tail.size(); }
    }

    static double discount(double price, Percent pct) {
        return price - price * pct.value() / 100.0;
    }

    public static void main(String[] args) {
        System.out.println(discount(80, new Percent(25)));
        var xs = new NonEmptyList<>("a", List.of("b", "c"));
        System.out.println("first " + xs.first() + ", size " + xs.size());
        try {
            new Percent(120);
        } catch (IllegalArgumentException e) {
            System.out.println("IllegalArgumentException: " + e.getMessage());
        }
    }
}
```

```output for=validated-records
60.0
first a, size 3
IllegalArgumentException: 120 is not in 0..100
```

- Verdict: idiom. Java has no refinement types. A record with a compact constructor checks its predicate once, when the value is made, and the type `Percent` then stands for the refined set. `NonEmptyList` makes the predicate structural: it always has a head, so `first()` cannot fail. The compiler does not prove the predicate; a bad value fails at run time, at the constructor.
