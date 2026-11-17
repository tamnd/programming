```java run id=extends-bound
import java.util.List;

public class Main {
    // An F-bound: T must be comparable with T.
    static <T extends Comparable<T>> T max(List<T> items) {
        T best = items.get(0);
        for (T x : items) {
            if (x.compareTo(best) > 0) {
                best = x;
            }
        }
        return best;
    }

    // Two bounds: a class first, then an interface.
    static <T extends Number & Comparable<T>> double maxAsDouble(List<T> items) {
        return max(items).doubleValue();
    }

    enum Size { SMALL, MEDIUM, LARGE }

    public static void main(String[] args) {
        System.out.println(max(List.of(3, 9, 4)));
        System.out.println(max(List.of("pear", "apple")));
        System.out.println(max(List.of(Size.MEDIUM, Size.SMALL)));
        System.out.println(maxAsDouble(List.of(2, 7)));
    }
}
```

```output for=extends-bound
9
pear
MEDIUM
7.0
```

- Verdict: native. `<T extends Comparable<T>>` lets `max` call `compareTo`. An enum works too, because `Enum<E extends Enum<E>>` implements `Comparable<E>`. `<T extends Number & Comparable<T>>` has two bounds, so the body can call `doubleValue` and `compareTo`.
