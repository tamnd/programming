```java run id=record
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Point(int x, int y) {
        Point withY(int newY) {
            return new Point(x, newY);
        }
    }

    public static void main(String[] args) {
        Point p = new Point(1, 2);
        System.out.println(p + " " + p.withY(5));

        List<String> source = new ArrayList<>(List.of("a", "b"));
        List<String> copy = List.copyOf(source);
        source.add("c");
        System.out.println(source + " " + copy);

        String word = "abc";
        System.out.println(word.toUpperCase() + " " + word);
    }
}
```

```output for=record
Point[x=1, y=2] Point[x=1, y=5]
[a, b, c] [a, b]
ABC abc
```

- Verdict: native. A `final` variable or field gets one value. The fields of a record are `private final`. `String` objects cannot change.
- `List.of` and `List.copyOf` return unmodifiable lists. `List.copyOf` copies, so later changes to the source do not show in the copy.
- An unmodifiable list has the type `List`, so a call to `add` compiles. It throws `UnsupportedOperationException` at run time.
- `Collections.unmodifiableList` returns a view. Changes to the backing list show through the view (see the pitfalls in the node).
- A `final` field that holds a mutable object only fixes the reference.
