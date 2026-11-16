```java wrong id=lazy-fold
import java.util.function.Supplier;

public class Main {
    public static void main(String[] args) {
        Supplier<Long> total = () -> 0L;
        for (long i = 0; i < 1_000_000; i++) {
            Supplier<Long> before = total;
            long item = i;
            total = () -> before.get() + item;
        }
        try {
            System.out.println(total.get());
        } catch (StackOverflowError error) {
            System.out.println("error: " + error.getClass().getSimpleName());
        }
    }
}
```

```output for=lazy-fold
error: StackOverflowError
```

```java run id=strict-fold
import java.util.stream.LongStream;

public class Main {
    public static void main(String[] args) {
        long total = 0;
        for (long i = 0; i < 1_000_000; i++) {
            total += i;
        }
        System.out.println(total);
        System.out.println(LongStream.range(0, 1_000_000).reduce(0, Long::sum));
    }
}
```

```output for=strict-fold
499999500000
499999500000
```

- Each `Supplier` keeps the one before it, so the heap holds a million lambda objects before the force. The force then makes a million nested `get` calls, and they overflow the thread stack.
- `LongStream.reduce` is a strict fold: it combines each element into the result as the stream runs.
