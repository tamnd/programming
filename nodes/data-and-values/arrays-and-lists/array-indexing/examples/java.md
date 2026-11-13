```java run id=checked-access
import java.util.List;
import java.util.Objects;

public class Main {
    public static void main(String[] args) {
        int[] a = {10, 20, 30, 40, 50};
        // Indices go from 0 to length - 1.
        System.out.println(a[0] + " " + a[a.length - 1] + " " + a.length);

        // Every array access is checked. A bad index throws ArrayIndexOutOfBoundsException.
        for (int i : new int[] {5, -1}) {
            try {
                System.out.println(a[i]);
            } catch (ArrayIndexOutOfBoundsException e) {
                System.out.println(e.getMessage());
            }
        }

        // A List throws IndexOutOfBoundsException. Java has no negative indices.
        List<String> list = List.of("x", "y", "z");
        System.out.println(list.get(list.size() - 1) + " " + list.getLast());
        try {
            list.get(3);
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getClass().getSimpleName() + ": " + e.getMessage());
        }

        // Objects.checkIndex does the same check for code that keeps its own storage.
        System.out.println(Objects.checkIndex(2, 3));
        try {
            Objects.checkIndex(3, 3);
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }

        // A char index is promoted to int.
        char c = 2;
        System.out.println(a[c]);
    }
}
```

```output
10 50 5
Index 5 out of bounds for length 5
Index -1 out of bounds for length 5
z z
ArrayIndexOutOfBoundsException: Index 3 out of bounds for length 3
2
Index 3 out of bounds for length 3
30
```

- Verdict: native. Every array access is checked, and a bad index on either side throws ArrayIndexOutOfBoundsException with the index and the length. The list from List.of throws a subclass of IndexOutOfBoundsException for index 3. Java has no negative indices, but getLast gives the last item. Objects.checkIndex does the same check for other code. A char index is promoted to int.
