```java run id=countdown
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;

public class Main {
    record Countdown(int start) implements Iterable<Integer> {
        public Iterator<Integer> iterator() {
            return new Iterator<>() {
                private int current = start;

                public boolean hasNext() {
                    return current > 0;
                }

                public Integer next() {
                    if (current <= 0) {
                        throw new NoSuchElementException();
                    }
                    return current--;
                }
            };
        }
    }

    public static void main(String[] args) {
        for (int n : new Countdown(3)) {
            System.out.println(n);
        }
        Iterator<String> it = List.of("a", "b").iterator();
        while (it.hasNext()) {
            System.out.println(it.next());
        }
    }
}
```

```output for=countdown
3
2
1
a
b
```

- Verdict: native. A class implements `Iterable<T>`, and its `iterator()` method returns an `Iterator<T>` with `hasNext()` and `next()`.
- The enhanced `for` statement is a short form of the `while` loop above: it calls `iterator()` once, then `hasNext()` and `next()` for each element.
- The iterators of `ArrayList` are fail-fast. A change to the collection that does not go through the iterator makes the next call throw `ConcurrentModificationException` (see Pitfalls on the concept page).
- `Iterator<Integer>` holds boxed `Integer` values. The loop unboxes each one into `int n`.
