```java run id=erasure
import java.util.ArrayList;
import java.util.List;
import java.util.function.Supplier;

public class Main {
    // new T() is not allowed, so the caller passes a factory.
    static <T> List<T> fill(int n, Supplier<T> make) {
        List<T> out = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            out.add(make.get());
        }
        return out;
    }

    @SuppressWarnings("unchecked")
    public static void main(String[] args) {
        System.out.println(fill(2, StringBuilder::new).size());

        // The cast to List<String> is not checked when the program runs.
        Object o = List.of(1, 2);
        List<String> strings = (List<String>) o;
        System.out.println(strings.size());
        try {
            String s = strings.get(0); // the cast that erasure inserts fails here
            System.out.println(s);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException");
        }
    }
}
```

```output for=erasure
2
2
ClassCastException
```

- Verdict: native. The compiler checks `List<String>` and then erases it to `List`. The cast `(List<String>) o` cannot check the item type, so the error comes from the cast to `String` that the compiler inserts at `get`.
