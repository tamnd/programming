```java run id=max
import java.util.List;

public class Main {
    static <T extends Comparable<? super T>> T max(List<T> items) {
        T result = items.get(0);
        for (T item : items) {
            if (item.compareTo(result) > 0) {
                result = item;
            }
        }
        return result;
    }

    public static void main(String[] args) {
        Integer i = max(List.of(3, 9, 4));
        String s = max(List.of("pear", "apple", "plum"));
        System.out.println(i + " " + s);
    }
}
```

```output for=max
9 plum
```

- Verdict: native. Classes, interfaces, methods and constructors can declare type parameters.
- `T extends Comparable<? super T>` is a bound. The body may call `compareTo` because every allowed type has it. The wildcard `? super T` also accepts a type that inherits its `compareTo` from a supertype.
- A type argument must be a reference type or a wildcard. `List<int>` is not valid, so the list holds boxed `Integer` objects.
- The compiler erases type arguments. At run time, a `List<Integer>` and a `List<String>` have the same class (see How it works on the concept page).
