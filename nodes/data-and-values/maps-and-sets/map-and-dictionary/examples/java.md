```java run id=hashmap
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class Main {
  public static void main(String[] args) {
    // A HashMap maps each key to one value. put returns the old value, or null.
    Map<String, Integer> m = new HashMap<>();
    System.out.println(m.put("x", 1) + " " + m.put("x", 10) + " " + m.put("y", 2));
    System.out.println(m.get("x") + " " + m.size() + " " + m.containsKey("y"));

    // A missing key gives null, and getOrDefault gives a default.
    System.out.println(m.get("w") + " " + m.getOrDefault("w", 0));

    // null is ambiguous: a key can also map to null. containsKey tells the two apart.
    m.put("n", null);
    System.out.println(m.get("n") + " " + m.containsKey("n") + " " + m.containsKey("w"));

    m.remove("n");
    // A HashMap has no defined order, so copy it to a TreeMap to print it sorted.
    System.out.println(new TreeMap<>(m));

    // Map.of makes a map that cannot change.
    Map<String, Integer> fixed = Map.of("a", 1, "b", 2);
    try {
      fixed.put("c", 3);
    } catch (UnsupportedOperationException e) {
      System.out.println("UnsupportedOperationException");
    }

    // Unboxing a missing value throws NullPointerException.
    try {
      int v = m.get("w");
      System.out.println(v);
    } catch (NullPointerException e) {
      System.out.println("NullPointerException");
    }
  }
}
```

```output
null 1 null
10 2 true
null 0
null true false
{x=10, y=2}
UnsupportedOperationException
NullPointerException
```

- Verdict: stdlib. put returns the old value or null, and get returns null for a missing key. A null result is ambiguous, because a key can map to null, so containsKey tells the two cases apart. Map.of makes a map that cannot change, and unboxing a missing Integer throws NullPointerException.
