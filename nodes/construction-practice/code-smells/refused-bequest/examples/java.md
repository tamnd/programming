```java run id=stop-inheriting-the-array
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.List;
import java.util.Stack;

public class Main {
    // After: a stack that holds a deque and offers four methods.
    static final class IntStack {
        private final Deque<Integer> items = new ArrayDeque<>();
        void push(int x) { items.push(x); }
        int pop() { return items.pop(); }
        int peek() { return items.peek(); }
        int size() { return items.size(); }
    }

    public static void main(String[] args) {
        // Before: java.util.Stack extends Vector, so it is also a List.
        Stack<Integer> before = new Stack<>();
        IntStack after = new IntStack();
        for (int x : List.of(1, 2, 3)) {
            before.push(x);
            after.push(x);
        }
        if (after.peek() != before.peek() || after.pop() != before.pop() || after.size() != before.size()) {
            throw new AssertionError("stack operations");
        }

        // The bequest: List methods reach the bottom of the stack.
        before.add(0, 0);
        before.remove(Integer.valueOf(1));
        if (!before.equals(List.of(0, 2))) throw new AssertionError("vector methods");

        // A method that takes a List accepts the stack and can sort it.
        List<Integer> asList = before;
        asList.sort(null);
        System.out.println("stack: tests pass");
    }
}
```

```output
stack: tests pass
```

- `java.util.Stack` is the class that this smell is often explained with. Its documentation recommends `Deque` instead.
- `IntStack` has no `add(int, E)` and no `remove(Object)`, so a client cannot reach below the top. A client that wants a list must ask for one.
- `ArrayDeque.push` adds at the front and `pop` removes from the front, so the front of the deque is the top of the stack.
