```java run id=binary-search-and-overflow
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        int[] xs = {12, 31, 47, 58, 90};
        System.out.println("search 58: " + Arrays.binarySearch(xs, 58));
        int r = Arrays.binarySearch(xs, 50);
        System.out.println("search 50: " + r + ", insertion point " + (-r - 1));

        List<String> names = new java.util.ArrayList<>(List.of("Grace", "Ada", "Alan"));
        Collections.sort(names); // stable for objects
        System.out.println(names + " Alan at " + Collections.binarySearch(names, "Alan"));

        int lo = 1_500_000_000, hi = 2_000_000_000; // indexes of a very large array
        System.out.println("(lo + hi) / 2   = " + (lo + hi) / 2);
        System.out.println("(lo + hi) >>> 1 = " + ((lo + hi) >>> 1));
        System.out.println("lo + (hi - lo) / 2 = " + (lo + (hi - lo) / 2));
    }
}
```

```output for=binary-search-and-overflow
search 58: 3
search 50: -4, insertion point 3
[Ada, Alan, Grace] Alan at 1
(lo + hi) / 2   = -397483648
(lo + hi) >>> 1 = 1750000000
lo + (hi - lo) / 2 = 1750000000
```

- Verdict: stdlib. `Arrays.sort` sorts arrays of primitives with a dual-pivot quicksort and arrays of objects with a stable merge sort. `Arrays.binarySearch` returns the index, or `-(insertion point) - 1` when the key is absent, so a negative result also says where to insert. The last lines show the bug that Joshua Bloch reported in 2006: with large indexes, `lo + hi` overflows `int` and the midpoint is negative. The unsigned shift and `lo + (hi - lo) / 2` give the right midpoint.
