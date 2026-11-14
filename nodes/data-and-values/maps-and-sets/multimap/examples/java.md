```java run id=computeifabsent
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {
  public static void main(String[] args) {
    // computeIfAbsent makes the list for a new key and returns it.
    Map<String, List<Integer>> m = new TreeMap<>();
    m.computeIfAbsent("yellow", k -> new ArrayList<>()).add(1);
    m.computeIfAbsent("blue", k -> new ArrayList<>()).add(2);
    m.computeIfAbsent("yellow", k -> new ArrayList<>()).add(3);
    System.out.println(m + " " + m.getOrDefault("red", List.of()));

    // Remove one value, and remove the key when its list is empty.
    m.computeIfPresent("blue", (k, list) -> {
      list.remove(Integer.valueOf(2));
      return list.isEmpty() ? null : list;
    });
    System.out.println(m);

    // Collectors.groupingBy builds a multimap from a stream.
    Map<Character, List<String>> byFirst = Stream.of("ant", "bee", "cat", "ape", "bat")
        .collect(Collectors.groupingBy(w -> w.charAt(0), TreeMap::new, Collectors.toList()));
    System.out.println(byFirst);

    // The number of pairs is the sum of the list sizes.
    int pairs = byFirst.values().stream().mapToInt(List::size).sum();
    System.out.println(byFirst.size() + " " + pairs);
  }
}
```

```output
{blue=[2], yellow=[1, 3]} []
{yellow=[1, 3]}
{a=[ant, ape], b=[bee, bat], c=[cat]}
3 5
```

- Verdict: idiom. A Map<K, List<V>> filled with computeIfAbsent, which the Map documentation shows for a multi-value map. computeIfPresent can remove a key when its list becomes empty. Collectors.groupingBy builds a multimap from a stream. Guava and Apache Commons Collections have Multimap types.
