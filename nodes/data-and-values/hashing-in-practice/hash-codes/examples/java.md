```java run id=records-and-string-hashcode
import java.util.HashMap;
import java.util.HashSet;
import java.util.Objects;

public class Main {
    record Point(int x, int y) {}  // equals and hashCode come from the fields

    static final class Account {  // written by hand: both methods use the same field
        final String id;
        final int visits;
        Account(String id, int visits) { this.id = id; this.visits = visits; }
        @Override public boolean equals(Object o) {
            return o instanceof Account a && a.id.equals(id);
        }
        @Override public int hashCode() { return Objects.hash(id); }
    }

    public static void main(String[] args) {
        var points = new HashMap<Point, String>();
        points.put(new Point(1, 2), "a");
        System.out.println("record key: " + points.get(new Point(1, 2)));

        var accounts = new HashSet<Account>();
        accounts.add(new Account("ada", 1));
        System.out.println("same id: " + accounts.contains(new Account("ada", 7)));

        // String.hashCode is s[0]*31^(n-1) + ... + s[n-1], the same in every run
        System.out.println("\"Aa\" " + "Aa".hashCode() + ", \"BB\" " + "BB".hashCode());
        System.out.println("\"AaAa\" " + "AaAa".hashCode() + ", \"BBBB\" " + "BBBB".hashCode()
            + ", \"AaBB\" " + "AaBB".hashCode());
    }
}
```

```output for=records-and-string-hashcode
record key: a
same id: true
"Aa" 2112, "BB" 2112
"AaAa" 2031744, "BBBB" 2031744, "AaBB" 2031744
```

```java wrong id=mutable-key
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> path = new ArrayList<>(List.of("home", "ada"));
        var visited = new HashSet<List<String>>();
        visited.add(path);
        path.add("docs");  // the key changes while it is in the set
        System.out.println("contains path: " + visited.contains(path));
        System.out.println("contains a copy: " + visited.contains(List.of("home", "ada", "docs")));
        System.out.println("size " + visited.size() + ", iterator finds it: "
            + visited.iterator().next().equals(path));
    }
}
```

```output for=mutable-key
contains path: false
contains a copy: false
size 1, iterator finds it: true
```

- Verdict: native. Every object has `equals` and `hashCode`, and records generate both from their components. A class written by hand must use the same fields in both, as `Account` does with `Objects.hash`. The specification of `String.hashCode` fixes the formula, so every run gives the same value, and strings that collide are easy to build: `"Aa"` and `"BB"` have one hash, so every string made of these two blocks does too. Since Java 8, `HashMap` turns a long chain into a balanced tree, which limits the damage.
- The wrong block changes a list after it is added to a `HashSet`. The set keeps the entry in the bucket of the old hash, so `contains` fails for the list itself and for an equal copy, while iteration still finds it.
