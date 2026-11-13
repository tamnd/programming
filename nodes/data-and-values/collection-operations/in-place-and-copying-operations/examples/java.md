```java run id=sort-in-place-and-copies
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // List.sort and Collections.sort work in place and return void.
        List<Integer> nums = new ArrayList<>(List.of(3, 1, 2));
        nums.sort(null);
        System.out.println(nums);

        // A stream makes a new list and leaves the source alone.
        List<Integer> src = new ArrayList<>(List.of(3, 1, 2));
        List<Integer> sorted = src.stream().sorted().toList();
        System.out.println(src + " " + sorted);

        // The list from toList() and List.of() is unmodifiable.
        try {
            sorted.sort(null);
        } catch (UnsupportedOperationException e) {
            System.out.println("UnsupportedOperationException");
        }

        // Arrays.asList is a fixed-size view of the array, so sorting it sorts the array.
        Integer[] arr = {3, 1, 2};
        Collections.sort(Arrays.asList(arr));
        System.out.println(Arrays.toString(arr));

        // reversed() is a view: a later change to the list shows in it.
        List<String> letters = new ArrayList<>(List.of("a", "b", "c"));
        List<String> back = letters.reversed();
        letters.add("d");
        System.out.println(back);

        // To get a sorted copy of a mutable list, copy it first.
        List<Integer> copy = new ArrayList<>(src);
        Collections.sort(copy);
        System.out.println(src + " " + copy);
    }
}
```

```output
[1, 2, 3]
[3, 1, 2] [1, 2, 3]
UnsupportedOperationException
[1, 2, 3]
[d, c, b, a]
[3, 1, 2] [1, 2, 3]
```

- Verdict: native. `List.sort` and `Collections.sort` change the list and return `void`. `stream().sorted().toList()` makes a new list, and that list is unmodifiable, so `sort` on it throws `UnsupportedOperationException`. `Arrays.asList` is a view of its array, so sorting the view sorts the array. `reversed()` is a view too, so a later `add` shows in it.
