```java run id=three-kinds
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class Main {
    static String describe(Collection<String> c) {
        return c.getClass().getSimpleName() + " " + c.size() + " " + c.contains("red");
    }

    public static void main(String[] args) {
        List<String> words = new ArrayList<>(List.of("red", "blue", "red", "green"));
        TreeSet<String> unique = new TreeSet<>(words);
        Map<String, Integer> counts = new TreeMap<>();
        for (String w : words) {
            counts.merge(w, 1, Integer::sum);
        }

        System.out.println(describe(words));
        System.out.println(describe(unique));
        System.out.println(describe(counts.keySet()) + " " + counts);
        System.out.println(words.get(1) + " " + unique);
    }
}
```

```output for=three-kinds
ArrayList 4 true
TreeSet 3 true
KeySet 3 true {blue=1, green=1, red=2}
blue [blue, green, red]
```

- Verdict: stdlib. The collections framework in `java.util` has the interfaces `List`, `Set` and `Map` and classes that implement them.
- `describe` takes any `Collection<String>`, so it works for the list and the set.
- A `Map` is not a `Collection`. The code passes `counts.keySet()`, a view of the keys that is a collection.
- `TreeSet` and `TreeMap` keep their elements sorted, so the output order is fixed.
