```java run id=generic-and-raw
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Main {
    // A generic class: Box is a type constructor, Box<String> is a type.
    static final class Box<T> {
        private final T item;

        Box(T item) {
            this.item = item;
        }

        T get() {
            return item;
        }
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public static void main(String[] args) {
        Box<String> b = new Box<>("hi");
        Map<String, List<Integer>> m = Map.of("a", List.of(1, 2));
        System.out.println(b.get().length() + " " + m.get("a"));

        // A raw type: the compiler no longer checks the items.
        List raw = new ArrayList<String>();
        raw.add(42);
        List<String> strings = raw;
        try {
            String s = strings.get(0);
            System.out.println(s);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException");
        }

        // The type argument is erased: both lists have one class.
        System.out.println(new ArrayList<String>().getClass() == new ArrayList<Integer>().getClass());
    }
}
```

```output for=generic-and-raw
2 [1, 2]
ClassCastException
true
```

- Verdict: native. `Box<T>` is a generic class, and `Box<String>` is a parameterized type. `List raw` is a raw type: the compiler lets the program add an `Integer` to a list of strings, and the error comes later, when the program reads the item as a `String`. The `@SuppressWarnings` hides the warnings that the compiler gives for this code. The last line prints `true`, because Java erases the type arguments.
