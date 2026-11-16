```java run id=stack
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.NoSuchElementException;

public class Main {
    public static void main(String[] args) {
        Deque<String> stack = new ArrayDeque<>();
        for (String item : new String[] {"a", "b", "c"}) {
            stack.push(item);
        }
        System.out.println(stack + " top " + stack.peek());

        System.out.println(stack.pop() + " " + stack.pop() + " " + stack);
        stack.pop();
        System.out.println("empty: " + (stack.isEmpty() ? "yes" : "no"));
        System.out.println("peek on empty: " + stack.peek());
        try {
            stack.pop();
        } catch (NoSuchElementException e) {
            System.out.println("pop on empty: NoSuchElementException");
        }
    }
}
```

```output for=stack
[c, b, a] top c
c b [a]
empty: yes
peek on empty: null
pop on empty: NoSuchElementException
```

- Verdict: stdlib. `ArrayDeque` through the `Deque` interface is the stack. `push`, `pop` and `peek` work at the front of the deque, so the deque prints the top first.
- `peek` on an empty deque returns `null`, and `pop` throws `NoSuchElementException`.
- An `ArrayDeque` does not accept `null` items.
