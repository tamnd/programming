```java run id=to-the-interface
import java.util.ArrayDeque;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class Main {
    static int total(Iterable<Integer> xs) {
        int sum = 0;
        for (int x : xs) sum += x;
        return sum;
    }

    static String describe(Collection<Integer> c) {
        return "size " + c.size() + ", total " + total(c);
    }

    public static void main(String[] args) {
        Map<String, Integer> prices = new TreeMap<>(Map.of("tea", 3, "cake", 4));
        System.out.println("list: " + describe(List.of(1, 2, 3)));
        System.out.println("set: " + describe(new TreeSet<>(Set.of(4, 5))));
        System.out.println("map values: " + describe(prices.values()));

        ArrayDeque<Integer> copy = new ArrayDeque<>(prices.values()); // conversion constructor
        System.out.println(copy + " " + (prices instanceof Collection<?>));
    }
}
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
[4, 3] false
```

- Verdict: stdlib. `Iterable` is in `java.lang`, and `Collection`, `List`, `Set`, `Queue`, `Deque` and `Map` are in `java.util`.
- `describe` takes a `Collection<Integer>`, so a list, a set and the `values()` view of a map all pass.
- A `Map` is not a `Collection`. The `TreeMap` orders its keys, so the values come as `cake` then `tea`.
