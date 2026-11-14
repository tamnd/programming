```java run id=treemap
import java.util.Map;
import java.util.NavigableMap;
import java.util.TreeMap;

public class Main {
  public static void main(String[] args) {
    // A TreeMap keeps its keys sorted, whatever the order of the puts.
    NavigableMap<Integer, String> m = new TreeMap<>();
    m.put(30, "c");
    m.put(10, "a");
    m.put(50, "e");
    m.put(20, "b");
    m.put(40, "d");
    System.out.println(m);

    // The first and last keys, and the closest keys to a value that is not there.
    System.out.println(m.firstKey() + " " + m.lastKey());
    System.out.println(m.floorKey(35) + " " + m.ceilingKey(35) + " " + m.lowerKey(30) + " " + m.higherKey(30));
    System.out.println(m.floorKey(5));

    // Views of a range. A view is not a copy: a change shows in both.
    System.out.println(m.subMap(20, true, 45, false) + " " + m.headMap(30) + " " + m.tailMap(40));
    m.headMap(20).clear();
    System.out.println(m + " " + m.descendingMap());

    // pollFirstEntry removes and returns the smallest entry.
    Map.Entry<Integer, String> e = m.pollFirstEntry();
    System.out.println(e.getKey() + "=" + e.getValue() + " " + m.size());

    // A comparator that ignores case makes "a" and "A" one key.
    TreeMap<String, Integer> ci = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    ci.put("apple", 1);
    ci.put("APPLE", 2);
    System.out.println(ci + " " + ci.size());

    // A null key throws, because the map must compare it.
    try {
      m.put(null, "x");
    } catch (NullPointerException ex) {
      System.out.println("NullPointerException");
    }
  }
}
```

```output
{10=a, 20=b, 30=c, 40=d, 50=e}
10 50
30 40 20 40
null
{20=b, 30=c, 40=d} {10=a, 20=b} {40=d, 50=e}
{20=b, 30=c, 40=d, 50=e} {50=e, 40=d, 30=c, 20=b}
20=b 3
{apple=2} 1
NullPointerException
```

- Verdict: stdlib. TreeMap is a red-black tree that implements NavigableMap: firstKey, floorKey, ceilingKey, lowerKey and higherKey, and the views subMap, headMap, tailMap and descendingMap. A change to a view changes the map. A case-insensitive comparator makes apple and APPLE one key. A null key throws.
