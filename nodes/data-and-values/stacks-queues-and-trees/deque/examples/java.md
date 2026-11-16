```java run id=deque
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
    public static void main(String[] args) {
        Deque<String> d = new ArrayDeque<>();
        d.addLast("b");
        d.addLast("c");
        d.addFirst("a");
        d.addLast("d");
        System.out.println(d + " front " + d.peekFirst() + " back " + d.peekLast());

        System.out.println(d.pollFirst() + " " + d.pollLast() + " " + d);

        // The same deque as a stack and as a queue.
        d.push("top");
        d.offer("tail");
        System.out.println(d + " pop " + d.pop() + " poll " + d.poll());
        System.out.println("descending: " + String.join(",", (Iterable<String>) d::descendingIterator));
    }
}
```

```output for=deque
[a, b, c, d] front a back d
a d [b, c]
[top, b, c, tail] pop top poll b
descending: tail,c
```

- Verdict: stdlib. `ArrayDeque` implements `Deque`, with `addFirst`, `addLast`, `pollFirst`, `pollLast`, `peekFirst` and `peekLast`.
- `push` and `pop` work at the front, and `offer` and `poll` use the deque as a queue.
- `descendingIterator` walks from the back to the front.
