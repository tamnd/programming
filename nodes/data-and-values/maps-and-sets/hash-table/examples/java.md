```java run id=hashcode
import java.util.HashMap;
import java.util.Map;

public class Main {
  // A class that overrides equals but not hashCode breaks the hash table contract.
  static final class BadPoint {
    final int x, y;
    BadPoint(int x, int y) { this.x = x; this.y = y; }
    @Override public boolean equals(Object o) {
      return o instanceof BadPoint p && p.x == x && p.y == y;
    }
  }

  // A record gets equals and hashCode from its fields.
  record Point(int x, int y) {}

  public static void main(String[] args) {
    // String.hashCode is s[0]*31^(n-1) + ... + s[n-1], so "Aa" and "BB" collide.
    System.out.println("Aa".hashCode() + " " + "BB".hashCode());
    Map<String, Integer> m = new HashMap<>();
    m.put("Aa", 1);
    m.put("BB", 2);
    System.out.println(m.get("Aa") + " " + m.get("BB") + " " + m.size());

    // Equal BadPoints usually have different hash codes, so the lookup misses.
    Map<BadPoint, String> bad = new HashMap<>();
    bad.put(new BadPoint(1, 2), "found");
    System.out.println(new BadPoint(1, 2).equals(new BadPoint(1, 2)) + " " + bad.get(new BadPoint(1, 2)));

    Map<Point, String> good = new HashMap<>();
    good.put(new Point(1, 2), "found");
    System.out.println(good.get(new Point(1, 2)) + " " + (new Point(1, 2).hashCode() == new Point(1, 2).hashCode()));

    // The default capacity is 16 and the load factor is 0.75, so the 13th entry makes the table grow.
    // A capacity given at the start avoids the copies: 1000 entries need 1000 / 0.75 buckets.
    Map<Integer, Integer> sized = HashMap.newHashMap(1000);
    for (int i = 0; i < 1000; i++) sized.put(i, i * i);
    System.out.println(sized.get(999));
  }
}
```

```output
2112 2112
1 2 2
true null
found true
998001
```

- Verdict: stdlib. HashMap uses hashCode to find a bucket and equals to find the key. "Aa" and "BB" have the same hash code, 2112, and the map keeps both. A class that overrides equals and not hashCode is lost in the map. A record defines both. HashMap.newHashMap(n) sizes the table for n entries at the default load factor of 0.75.
