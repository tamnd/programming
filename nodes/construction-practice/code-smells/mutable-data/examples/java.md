```java run id=sort-a-copy
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    // Before: Collections.sort changes the caller's list.
    static int medianBefore(List<Integer> values) {
        Collections.sort(values);
        return values.get(values.size() / 2);
    }

    // After: the stream sorts a copy.
    static int median(List<Integer> values) {
        List<Integer> sorted = values.stream().sorted().toList();
        return sorted.get(sorted.size() / 2);
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        List<Integer> readings = new ArrayList<>(List.of(30, 10, 20));
        check(medianBefore(readings) == 20, "before");
        check(readings.equals(List.of(10, 20, 30)), "the first reading is lost");
        System.out.println("medianBefore: the caller's list was changed");

        readings = new ArrayList<>(List.of(30, 10, 20));
        check(median(readings) == 20, "after");
        check(readings.equals(List.of(30, 10, 20)), "unchanged");
        System.out.println("median: tests pass");

        try {
            medianBefore(List.of(30, 10, 20));
            throw new AssertionError("sorted an unmodifiable list");
        } catch (UnsupportedOperationException expected) {
            System.out.println("medianBefore: List.of rejects the change");
        }
    }
}
```

```output
medianBefore: the caller's list was changed
median: tests pass
medianBefore: List.of rejects the change
```

- A list from `List.of` is unmodifiable, so the "before" version fails at once with `UnsupportedOperationException`. An unmodifiable list turns a silent change into an error at the line that makes it.
- `Stream.toList()` also returns an unmodifiable list, so the copy in `median` cannot be changed by a later caller either.
- The method signature `int median(List<Integer> values)` says nothing about a change to `values`. Java has no way to mark a parameter as read only, so the contract must be in the name, the documentation or the type of the list.
