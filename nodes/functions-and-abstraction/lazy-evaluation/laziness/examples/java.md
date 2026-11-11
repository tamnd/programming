```java run id=streams
import java.util.function.Supplier;
import java.util.stream.Stream;

public class Main {
    static <T> Supplier<T> memoize(Supplier<T> source) {
        return new Supplier<>() {
            private T value;
            private boolean ready;

            public T get() {
                if (!ready) {
                    value = source.get();
                    ready = true;
                }
                return value;
            }
        };
    }

    public static void main(String[] args) {
        Stream<Integer> evens = Stream.iterate(1, n -> n + 1)
                .peek(n -> System.out.println("visit " + n))
                .filter(n -> n % 2 == 0);
        System.out.println("nothing visited yet");
        System.out.println(evens.limit(2).toList());

        Supplier<String> config = memoize(() -> {
            System.out.println("reading config");
            return "debug";
        });
        System.out.println(config.get() + " " + config.get());
    }
}
```

```output for=streams
nothing visited yet
visit 1
visit 2
visit 3
visit 4
[2, 4]
reading config
debug debug
```

- Verdict: stdlib. Java evaluates arguments eagerly. The intermediate operations of a `Stream`, such as `map`, `filter` and `peek`, are lazy, and nothing runs until a terminal operation, such as `toList`.
- A `Supplier` is a thunk. The standard library has no memoizing supplier, so programs write one or use a library. The memoizer above is not safe for use by many threads.

```java wrong id=reuse
import java.util.stream.Stream;

public class Main {
    public static void main(String[] args) {
        Stream<String> names = Stream.of("a", "b");
        System.out.println(names.count());
        try {
            System.out.println(names.count());
        } catch (IllegalStateException e) {
            System.out.println(e.getMessage());
        }
    }
}
```

```output for=reuse
2
stream has already been operated upon or closed
```

A stream can be consumed one time. Keep the source collection and make a new stream for each pass.
