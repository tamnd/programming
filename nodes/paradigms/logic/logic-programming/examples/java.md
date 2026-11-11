```java run id=fixpoint
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class Main {
    record Pair(String x, String y) implements Comparable<Pair> {
        public int compareTo(Pair o) {
            int c = x.compareTo(o.x);
            return c != 0 ? c : y.compareTo(o.y);
        }
    }

    public static void main(String[] args) {
        List<Pair> parent = List.of(new Pair("zeus", "ares"), new Pair("ares", "harmonia"),
                new Pair("harmonia", "semele"), new Pair("semele", "dionysus"));

        Set<Pair> ancestor = new TreeSet<>(parent);
        boolean changed = true;
        while (changed) {
            changed = false;
            for (Pair p : parent) {
                for (Pair a : List.copyOf(ancestor)) {
                    if (p.y().equals(a.x())) {
                        changed |= ancestor.add(new Pair(p.x(), a.y()));
                    }
                }
            }
        }
        System.out.println(ancestor.size());
        ancestor.stream().filter(a -> a.x().equals("ares")).forEach(a -> System.out.println(a.y()));
    }
}
```

```output for=fixpoint
10
dionysus
harmonia
semele
```

- Verdict: absent. Java has no logic engine in the language or the JDK. Rule engines such as Drools, and Prolog systems that run on the JVM, are libraries.
