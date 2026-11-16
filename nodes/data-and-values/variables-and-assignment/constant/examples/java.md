```java run id=final
import java.util.ArrayList;
import java.util.List;

public class Main {
    static final int MAX_SIZE = 10;

    public static void main(String[] args) {
        final List<String> names = new ArrayList<>();
        names.add("Ada");
        final int length;
        length = names.get(0).length();
        System.out.println(MAX_SIZE + " " + names + " " + length);
        List<String> fixed = List.of("Ada");
        try {
            fixed.add("Bob");
        } catch (UnsupportedOperationException e) {
            System.out.println("UnsupportedOperationException");
        }
    }
}
```

```output for=final
10 [Ada] 3
UnsupportedOperationException
```

- Verdict: native. `static final` with a constant expression gives a compile-time constant. A `final` local can get its value later, once: `length` is a blank final. `final` keeps the reference, but the list behind it can change. `List.of` makes a list that cannot change, and `add` throws an exception.
