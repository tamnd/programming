```java run id=stream-collectors
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) {
        List<String> words = List.of("apple", "fig", "banana", "kiwi", "cherry");

        // A stream does the work of a comprehension: a source, a filter and a map.
        List<Integer> lengths = words.stream().filter(w -> w.length() > 3).map(String::length).toList();
        System.out.println(lengths);

        // A collector builds a set or a map instead of a list.
        Set<Character> firsts = words.stream()
                .map(w -> w.charAt(0))
                .collect(Collectors.toCollection(TreeSet::new));
        System.out.println(firsts);
        Map<String, Integer> withI = words.stream()
                .filter(w -> w.contains("i"))
                .collect(Collectors.toMap(w -> w, String::length, (a, b) -> a, TreeMap::new));
        System.out.println(withI);

        // flatMap does the work of two for clauses.
        List<String> pairs = IntStream.rangeClosed(1, 2).boxed()
                .flatMap(x -> Stream.of("a", "b").map(y -> x + y))
                .toList();
        System.out.println(pairs);

        // A stream is lazy: map runs only for the items that limit needs.
        int[] calls = {0};
        List<String> firstTwo = words.stream()
                .map(w -> {
                    calls[0]++;
                    return w.toUpperCase();
                })
                .limit(2)
                .toList();
        System.out.println(firstTwo + " " + calls[0]);
    }
}
```

```output
[5, 6, 4, 6]
[a, b, c, f, k]
{fig=3, kiwi=4}
[1a, 1b, 2a, 2b]
[APPLE, FIG] 2
```

- Verdict: stdlib. Java has no comprehension syntax, but a stream has the same parts: a source, filter and map. toList, Collectors.toCollection and Collectors.toMap choose the type of the result. flatMap does the work of two for clauses. The stream is lazy, so map runs only twice because limit(2) needs only two items.
