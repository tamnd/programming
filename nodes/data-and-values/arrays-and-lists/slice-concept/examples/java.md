```java run id=copyofrange-and-sublist
import java.util.ArrayList;
import java.util.Arrays;
import java.util.ConcurrentModificationException;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // Arrays.copyOfRange copies a part of an array. The end index is not included.
        int[] a = {10, 20, 30, 40, 50};
        int[] part = Arrays.copyOfRange(a, 1, 3);
        part[0] = 99;
        System.out.println(Arrays.toString(part) + " " + Arrays.toString(a));

        // subList is a view. A change through the view changes the list behind it.
        List<Integer> xs = new ArrayList<>(List.of(10, 20, 30, 40, 50));
        List<Integer> view = xs.subList(1, 3);
        view.set(0, 99);
        System.out.println(view + " " + xs);

        // Clearing a view removes that range from the list.
        xs.subList(0, 2).clear();
        System.out.println(xs);

        // A structural change to the list makes an older view unusable.
        List<Integer> old = xs.subList(0, 1);
        xs.add(60);
        try {
            System.out.println(old.get(0));
        } catch (ConcurrentModificationException e) {
            System.out.println("ConcurrentModificationException");
        }

        // Bounds are checked: an end past the size throws.
        try {
            xs.subList(1, 10);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("IndexOutOfBoundsException");
        }
    }
}
```

```output
[99, 30] [10, 20, 30, 40, 50]
[99, 30] [10, 99, 30, 40, 50]
[30, 40, 50]
ConcurrentModificationException
IndexOutOfBoundsException
```

- Verdict: stdlib. Arrays.copyOfRange copies a range of an array. subList is a view, so set through it changes the list, and clear on a view removes that range. After the list changes its size, the old view throws ConcurrentModificationException. An end past the size throws IndexOutOfBoundsException.
