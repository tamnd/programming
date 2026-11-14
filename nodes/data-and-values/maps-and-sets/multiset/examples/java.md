```java run id=merge-counts
import java.util.Map;
import java.util.TreeMap;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
  public static void main(String[] args) {
    String[] words = "the cat and the hat and the bat".split(" ");

    // merge with Integer::sum adds 1 to the count, or puts 1 for a new word.
    Map<String, Integer> counts = new TreeMap<>();
    for (String w : words) counts.merge(w, 1, Integer::sum);
    System.out.println(counts + " " + counts.getOrDefault("dog", 0));

    // groupingBy with counting builds the same multiset from a stream.
    Map<String, Long> byStream = Stream.of(words)
        .collect(Collectors.groupingBy(Function.identity(), TreeMap::new, Collectors.counting()));
    System.out.println(byStream);

    // Remove one copy: merge with -1, and return null at zero to remove the key.
    counts.merge("and", -1, (a, b) -> a + b == 0 ? null : a + b);
    counts.merge("cat", -1, (a, b) -> a + b == 0 ? null : a + b);
    System.out.println(counts.get("and") + " " + counts.containsKey("cat"));

    // The size of the multiset is the sum of the counts.
    int total = counts.values().stream().mapToInt(Integer::intValue).sum();
    System.out.println(counts.size() + " " + total);
  }
}
```

```output
{and=2, bat=1, cat=1, hat=1, the=3} 0
{and=2, bat=1, cat=1, hat=1, the=3}
1 false
4 6
```

- Verdict: idiom. A Map<E, Integer> updated with merge(e, 1, Integer::sum), or Collectors.groupingBy with Collectors.counting. A merge function that returns null removes the key at zero. Guava has Multiset, HashMultiset and TreeMultiset.
