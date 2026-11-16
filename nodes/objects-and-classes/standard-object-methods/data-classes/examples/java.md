```java run id=record-and-compact-constructor
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Main {
    record Version(int major, int minor) {
        // A compact constructor checks the components.
        Version {
            if (major < 0 || minor < 0) {
                throw new IllegalArgumentException("negative part");
            }
        }
    }

    record Release(String name, List<String> files) {}

    public static void main(String[] args) {
        Version a = new Version(1, 4);
        System.out.println(a);
        System.out.println(a.equals(new Version(1, 4)) + " " + a.minor());
        Set<Version> seen = new HashSet<>(List.of(a));
        System.out.println(seen.contains(new Version(1, 4)));

        var files = new java.util.ArrayList<>(List.of("a.txt"));
        Release r = new Release("r1", files);
        files.add("b.txt");
        System.out.println(r.files());
    }
}
```

```output for=record-and-compact-constructor
Version[major=1, minor=4]
true 4
true
[a.txt, b.txt]
```

- Verdict: native. A `record` declares its components in the header, and the compiler writes the constructor, the accessors such as `minor()`, `equals`, `hashCode` and `toString`.
- Equal records have equal hash codes, so a `HashSet` finds an equal copy.
- A record is shallowly immutable: `Release` keeps the list that it was given, so a change to that list shows in the record. A compact constructor can store `List.copyOf(files)` instead.
