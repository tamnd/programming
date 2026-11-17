```java run id=typed-containers
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    @SuppressWarnings({"rawtypes", "unchecked"})
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        names.add("ann");
        String first = names.get(0); // no cast
        System.out.println(first.toUpperCase());

        // List<int> is an error, so the list holds Integer objects.
        List<Integer> nums = new ArrayList<>();
        nums.add(8); // the compiler boxes 8
        Map<String, List<Integer>> m = new HashMap<>();
        m.put("a", nums);
        System.out.println(m);

        // A raw List accepts any object.
        List raw = names;
        raw.add(42);
        try {
            String s = names.get(1);
            System.out.println(s);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException");
        }
    }
}
```

```output for=typed-containers
ANN
{a=[8]}
ClassCastException
```

- Verdict: stdlib. `List<E>` and `Map<K, V>` are generic interfaces in `java.util`. A type argument must be a reference type, so a list of numbers is a `List<Integer>`. The raw `List` lets the program add an `Integer` to a list of strings, and the error comes when the program reads it.
