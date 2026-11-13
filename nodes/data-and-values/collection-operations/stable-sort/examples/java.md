```java run id=list-sort-is-stable
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class Main {
    record Staff(String name, String team, int grade) {}

    static List<String> names(List<Staff> s) {
        return s.stream().map(Staff::name).toList();
    }

    public static void main(String[] args) {
        List<Staff> staff = List.of(
                new Staff("ann", "ops", 3), new Staff("bob", "dev", 2), new Staff("cy", "ops", 2),
                new Staff("di", "dev", 3), new Staff("ed", "ops", 2));

        // List.sort is stable: equal teams keep the name order.
        List<Staff> s = new ArrayList<>(staff);
        s.sort(Comparator.comparing(Staff::team));
        System.out.println(names(s));

        // Two stable passes: grade first, then team.
        s = new ArrayList<>(staff);
        s.sort(Comparator.comparingInt(Staff::grade));
        s.sort(Comparator.comparing(Staff::team));
        System.out.println(names(s));

        // A stream sort is stable too, because the stream is ordered.
        System.out.println(names(staff.stream()
                .sorted(Comparator.comparing(Staff::team).thenComparingInt(Staff::grade)).toList()));

        // Arrays.sort on an Object[] is stable; on an int[] stability does not matter,
        // because equal ints cannot be told apart.
        Staff[] arr = staff.toArray(new Staff[0]);
        Arrays.sort(arr, Comparator.comparingInt(Staff::grade).reversed());
        System.out.println(names(Arrays.asList(arr)));
    }
}
```

```output
[bob, di, ann, cy, ed]
[bob, di, cy, ed, ann]
[bob, di, cy, ed, ann]
[ann, di, bob, cy, ed]
```

- Verdict: native. `List.sort` keeps the name order inside each team. Two passes give the same order as `comparing(team).thenComparingInt(grade)` on a stream. `Arrays.sort` on an object array is stable, so the reversed grade comparator puts `ann` and `di` first in their input order, and then `bob`, `cy` and `ed`.
