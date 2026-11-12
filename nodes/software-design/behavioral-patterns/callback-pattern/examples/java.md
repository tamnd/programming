```java run id=callbacks
import java.util.ArrayDeque;
import java.util.List;
import java.util.Queue;
import java.util.function.Consumer;

public class Main {
    static <T> void forEach(List<T> items, Consumer<T> callback) {
        for (T item : items) callback.accept(item); // a blocking callback
    }

    public static void main(String[] args) {
        forEach(List.of("hello", "world"), word -> System.out.println(word.toUpperCase()));

        Queue<Runnable> pending = new ArrayDeque<>();
        pending.add(() -> System.out.println("saved")); // a deferred callback
        System.out.println("request sent");
        while (!pending.isEmpty()) pending.poll().run();
    }
}
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: stdlib. Functional interfaces such as `Consumer` and `Runnable` are the callback types, and lambdas implement them.
