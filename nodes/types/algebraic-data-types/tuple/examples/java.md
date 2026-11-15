```java run id=pair
import java.util.Collections;
import java.util.List;
import java.util.Map;

public class Main {
    record Pair<A, B>(A first, B second) {}

    static Pair<Integer, Integer> minMax(List<Integer> values) {
        return new Pair<>(Collections.min(values), Collections.max(values));
    }

    static String yn(boolean b) {
        return b ? "yes" : "no";
    }

    public static void main(String[] args) {
        Pair<Integer, Integer> pair = minMax(List.of(7, 2, 9, 4));
        System.out.println("pair: " + pair);
        if (pair instanceof Pair<Integer, Integer>(Integer lo, Integer hi)) {
            System.out.println("lo: " + lo + " hi: " + hi);
        }
        System.out.println("second part: " + pair.second());
        System.out.println("equal pairs: " + yn(pair.equals(new Pair<>(2, 9))));

        Map.Entry<String, Integer> entry = Map.entry("Ada", 36);
        System.out.println("entry: " + entry.getKey() + " " + entry.getValue());
    }
}
```

```output for=pair
pair: Pair[first=2, second=9]
lo: 2 hi: 9
second part: 9
equal pairs: yes
entry: Ada 36
```

- Verdict: idiom. Java has no tuple type, so the program declares a small generic `record`.
- A record pattern such as `Pair<Integer, Integer>(Integer lo, Integer hi)` unpacks the parts.
- `Map.entry` makes an immutable pair of a key and a value.
