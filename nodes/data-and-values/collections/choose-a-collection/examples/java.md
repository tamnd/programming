```java run id=five-needs
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class Main {
    public static void main(String[] args) {
        List<String> words = new ArrayList<>(Arrays.asList("the cat saw the dog and the cat ran".split(" ")));

        Set<String> seen = new HashSet<>(words);
        Map<String, Integer> counts = new HashMap<>();
        for (String w : words) counts.merge(w, 1, Integer::sum);
        ArrayDeque<String> line = new ArrayDeque<>(words.subList(0, 3));
        String first = line.removeFirst();
        String second = line.removeFirst();
        TreeMap<String, Integer> sorted = new TreeMap<>(counts);

        System.out.println("list: " + words.size() + " words, third is " + words.get(2));
        System.out.println("set: " + seen.size() + " distinct");
        System.out.println("map: the=" + counts.get("the") + " cat=" + counts.get("cat"));
        System.out.println("queue: " + first + ", then " + second);
        System.out.println("sorted: " + String.join(" ", sorted.keySet()));
    }
}
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- `ArrayList`, `HashSet`, `HashMap`, `ArrayDeque` and `TreeMap` are the general purpose implementations. The `TreeMap` keeps its keys sorted, so `keySet` gives them in order.
