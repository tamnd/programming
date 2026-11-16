```java run id=queue
import java.util.ArrayDeque;
import java.util.NoSuchElementException;
import java.util.Queue;

public class Main {
    public static void main(String[] args) {
        Queue<String> queue = new ArrayDeque<>();
        for (String item : new String[] {"a", "b", "c"}) {
            queue.offer(item);
        }
        System.out.println(queue + " front " + queue.peek());

        String first = queue.poll();
        System.out.println(first + " next " + queue.peek() + " size " + queue.size());
        queue.poll();
        queue.poll();
        System.out.println("empty: " + (queue.isEmpty() ? "yes" : "no"));
        System.out.println("poll on empty: " + queue.poll());
        try {
            queue.remove();
        } catch (NoSuchElementException e) {
            System.out.println("remove on empty: NoSuchElementException");
        }
    }
}
```

```output for=queue
[a, b, c] front a
a next b size 2
empty: yes
poll on empty: null
remove on empty: NoSuchElementException
```

- Verdict: stdlib. `ArrayDeque` through the `Queue` interface is the queue. `offer` adds at the tail, `poll` removes the head, and `peek` reads it.
- `poll` and `peek` return `null` on an empty queue. `remove` and `element` throw `NoSuchElementException`.
