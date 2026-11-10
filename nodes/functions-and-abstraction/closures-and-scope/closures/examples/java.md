```java run id=counter
import java.util.function.IntSupplier;

public class Main {
    static IntSupplier makeCounter() {
        int[] count = {0};
        return () -> ++count[0];
    }

    public static void main(String[] args) {
        IntSupplier first = makeCounter();
        IntSupplier second = makeCounter();
        System.out.println(first.getAsInt() + " " + first.getAsInt() + " " + first.getAsInt());
        System.out.println(second.getAsInt());
    }
}
```

```output for=counter
1 2 3
1
```

- Verdict: native. A lambda expression or an anonymous class can use the local variables of the enclosing method.
- The variable must be final or effectively final, so the lambda copies its value. Here the variable `count` holds a reference to an array that never changes, and the lambda changes the element of the array.
- A lambda has a functional interface type, such as `IntSupplier` or `Runnable`. There is no separate function type.
- An `AtomicInteger` or a field of a class are the other usual holders of changing state.
