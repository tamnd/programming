```java run id=pipeline
import java.util.List;
import java.util.stream.Stream;

public class Main {
    static int square(int n) {
        System.out.println("square " + n);
        return n * n;
    }

    public static void main(String[] args) {
        List<Integer> oddSquares = Stream.iterate(1, n -> n + 1)
                .map(Main::square)
                .filter(s -> s % 2 == 1)
                .limit(2)
                .toList();
        System.out.println(oddSquares);

        Stream<Integer> once = Stream.of(1, 2, 3);
        System.out.println(once.count());
        try {
            System.out.println(once.count());
        } catch (IllegalStateException error) {
            System.out.println("second use: " + error.getClass().getSimpleName());
        }
    }
}
```

```output for=pipeline
square 1
square 2
square 3
[1, 9]
3
second use: IllegalStateException
```

- Verdict: stdlib. `Stream.iterate` makes an infinite stream. `map` and `filter` are intermediate and lazy, `limit` is a short-circuiting intermediate operation, and `toList` is the terminal operation that starts the work.
- A stream can be used only one time. The second terminal operation throws `IllegalStateException`. A `Supplier<Stream<T>>` makes a fresh stream for each use.
