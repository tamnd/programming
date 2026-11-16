```java run id=force-stream
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) {
        List<Integer> prices = new ArrayList<>(List.of(10, 20, 30));

        // Lazy: the stream reads the list when the terminal operation runs.
        Stream<Integer> lazy = prices.stream().map(price -> price * 2);
        // Forced: toList runs the stream now and stores the result.
        List<Integer> forced = prices.stream().map(price -> price * 2).toList();

        prices.add(40);
        System.out.println(lazy.toList());
        System.out.println(forced);
    }
}
```

```output for=force-stream
[20, 40, 60, 80]
[20, 40, 60]
```

- Intermediate operations such as `map` are lazy. The stream reads the source only when a terminal operation such as `toList` or `collect` starts.
- The lazy stream sees the element that the program adds after the stream was made. The forced list does not.
- A stream can run only once. To use the result more than once, force it into a list.
