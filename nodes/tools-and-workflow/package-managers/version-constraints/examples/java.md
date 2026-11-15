```java run id=caret-and-tilde-ranges
// Turn caret, tilde and exact constraints into a range [low, high), then test versions against it.
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    record Version(int major, int minor, int patch) implements Comparable<Version> {
        static Version parse(String s) {
            int[] p = Arrays.stream(s.split("\\.")).mapToInt(Integer::parseInt).toArray();
            return new Version(p[0], p[1], p[2]);
        }

        public int compareTo(Version o) {
            if (major != o.major) {
                return Integer.compare(major, o.major);
            }
            if (minor != o.minor) {
                return Integer.compare(minor, o.minor);
            }
            return Integer.compare(patch, o.patch);
        }

        @Override
        public String toString() {
            return major + "." + minor + "." + patch;
        }
    }

    record Range(Version low, Version high) {
        boolean contains(Version v) {
            return low.compareTo(v) <= 0 && v.compareTo(high) < 0;
        }
    }

    static Range toRange(String constraint) {
        char first = constraint.charAt(0);
        String op = first == '^' || first == '~' ? String.valueOf(first) : "";
        Version low = Version.parse(constraint.substring(op.length()));
        Version high = switch (op) {
            case "^" -> low.major() > 0 ? new Version(low.major() + 1, 0, 0) // keep the left-most part that is not zero
                    : low.minor() > 0 ? new Version(0, low.minor() + 1, 0)
                    : new Version(0, 0, low.patch() + 1);
            case "~" -> new Version(low.major(), low.minor() + 1, 0); // allow patch changes only
            default -> new Version(low.major(), low.minor(), low.patch() + 1); // an exact version
        };
        return new Range(low, high);
    }

    public static void main(String[] args) {
        List<String> candidates = List.of("0.0.4", "0.0.5", "0.2.3", "0.2.4", "0.2.9", "0.3.0",
                "1.2.2", "1.2.3", "1.2.9", "1.4.0", "2.0.0");
        System.out.println("versions: " + String.join(" ", candidates));
        for (String constraint : List.of("^1.2.3", "^0.2.4", "^0.0.4", "~1.2.3", "~0.2.4", "1.2.3")) {
            Range r = toRange(constraint);
            String matches = candidates.stream()
                    .filter(c -> r.contains(Version.parse(c)))
                    .collect(Collectors.joining(" "));
            System.out.println(constraint + " means >=" + r.low() + " <" + r.high() + ", matches " + matches);
        }
    }
}
```

```output
versions: 0.0.4 0.0.5 0.2.3 0.2.4 0.2.9 0.3.0 1.2.2 1.2.3 1.2.9 1.4.0 2.0.0
^1.2.3 means >=1.2.3 <2.0.0, matches 1.2.3 1.2.9 1.4.0
^0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
^0.0.4 means >=0.0.4 <0.0.5, matches 0.0.4
~1.2.3 means >=1.2.3 <1.3.0, matches 1.2.3 1.2.9
~0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
1.2.3 means >=1.2.3 <1.2.4, matches 1.2.3
```

- Verdict: idiom. A record implements `Comparable`, so versions compare by major, then minor, then patch.
- A switch expression picks the upper bound, and a nested conditional handles the three cases of the caret.
- The record `Range` holds the bounds and the test `contains`.
