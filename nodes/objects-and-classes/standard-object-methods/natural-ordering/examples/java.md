```java run id=comparable-and-comparator
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Main {
    record Version(int major, int minor) implements Comparable<Version> {
        @Override
        public int compareTo(Version o) {
            int c = Integer.compare(major, o.major);
            return c != 0 ? c : Integer.compare(minor, o.minor);
        }
    }

    public static void main(String[] args) {
        List<Version> vs = new ArrayList<>(List.of(new Version(1, 10), new Version(1, 2), new Version(0, 9)));
        vs.sort(null);
        System.out.println(vs);
        vs.sort(Comparator.comparingInt(Version::minor).reversed());
        System.out.println(vs);
    }
}
```

```output for=comparable-and-comparator
[Version[major=0, minor=9], Version[major=1, minor=2], Version[major=1, minor=10]]
[Version[major=1, minor=10], Version[major=0, minor=9], Version[major=1, minor=2]]
```

- Verdict: native. A class that implements `Comparable<T>` has a natural ordering, and `list.sort(null)`, `Collections.sort`, `TreeSet` and `TreeMap` use it.
- `compareTo` compares the fields in order of importance with `Integer.compare`, which avoids the overflow of `a - b`.
- A `Comparator` gives another order. `Comparator.comparingInt(Version::minor).reversed()` sorts by the minor number, largest first.
