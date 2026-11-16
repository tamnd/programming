```java run id=final-and-lambda
import java.util.List;

public class Main {
    public static void main(String[] args) {
        final int base = 10;
        int offset = 5;
        List<Integer> out = List.of(1, 2, 3).stream().map(x -> x * base + offset).toList();
        System.out.println(out);

        final String size;
        if (args.length == 0) {
            size = "none";
        } else {
            size = "some";
        }
        System.out.println(size);
    }
}
```

```output for=final-and-lambda
[15, 25, 35]
none
```

- Verdict: native. A `final` variable can be assigned once, and the compiler rejects a second assignment. A lambda can use only final or effectively final local variables: `offset` is never assigned again, so it is effectively final. A `final` variable can get its value later, once on each path.
