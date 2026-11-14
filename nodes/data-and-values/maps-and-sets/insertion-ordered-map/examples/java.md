```java run id=linkedhashmap
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
  // An access-order LinkedHashMap with removeEldestEntry is a small LRU cache.
  static class Lru<K, V> extends LinkedHashMap<K, V> {
    private final int size;

    Lru(int size) {
      super(16, 0.75f, true);
      this.size = size;
    }

    @Override
    protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
      return size() > size;
    }
  }

  public static void main(String[] args) {
    // LinkedHashMap keeps insertion order. HashMap does not promise any order.
    Map<String, Integer> m = new LinkedHashMap<>();
    m.put("pear", 4);
    m.put("apple", 7);
    m.put("fig", 2);
    System.out.println(m);

    // Putting a key again does not move it. Remove and put moves it to the end.
    m.put("pear", 9);
    System.out.println(m);
    m.remove("pear");
    m.put("pear", 1);
    System.out.println(m.keySet());

    // Java 21 added SequencedMap: first and last entries, putFirst, reversed.
    LinkedHashMap<String, Integer> s = new LinkedHashMap<>(m);
    s.putFirst("kiwi", 5);
    System.out.println(s.firstEntry() + " " + s.lastEntry() + " " + s.reversed().keySet());

    // A copy into a LinkedHashMap keeps the order of the source.
    Map<String, Integer> copy = new LinkedHashMap<>(s);
    System.out.println(copy.keySet().equals(s.keySet()) + " " + copy.equals(new HashMap<>(s)));

    Lru<String, Integer> lru = new Lru<>(2);
    lru.put("a", 1);
    lru.put("b", 2);
    lru.get("a");
    lru.put("c", 3);
    System.out.println(lru.keySet());
  }
}
```

```output
{pear=4, apple=7, fig=2}
{pear=9, apple=7, fig=2}
[apple, fig, pear]
kiwi=5 pear=1 [pear, fig, apple, kiwi]
true true
[a, c]
```

- Verdict: stdlib. LinkedHashMap keeps a doubly linked list through its entries. Java 21 added SequencedMap with firstEntry, lastEntry, putFirst and reversed. An access-order LinkedHashMap with removeEldestEntry is an LRU cache. HashMap promises no order.
