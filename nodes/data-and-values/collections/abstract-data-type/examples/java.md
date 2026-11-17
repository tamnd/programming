```java run id=stack-adt
import java.util.ArrayList;
import java.util.StringJoiner;

public class Main {
    interface Stack<T> {
        void push(T v);
        T pop();
        boolean isEmpty();
    }

    static final class ArrayStack<T> implements Stack<T> {
        private final ArrayList<T> items = new ArrayList<>();
        public void push(T v) { items.add(v); }
        public T pop() { return items.remove(items.size() - 1); }
        public boolean isEmpty() { return items.isEmpty(); }
    }

    static final class LinkedStack<T> implements Stack<T> {
        private record Cell<T>(T v, Cell<T> rest) {}
        private Cell<T> top;
        public void push(T v) { top = new Cell<>(v, top); }
        public T pop() { T v = top.v(); top = top.rest(); return v; }
        public boolean isEmpty() { return top == null; }
    }

    static String drain(Stack<Integer> s) {
        for (int v = 1; v <= 3; v++) s.push(v);
        StringJoiner out = new StringJoiner(" ");
        while (!s.isEmpty()) out.add(String.valueOf(s.pop()));
        return out.toString();
    }

    public static void main(String[] args) {
        for (Stack<Integer> s : java.util.List.of(new ArrayStack<Integer>(), new LinkedStack<Integer>())) {
            System.out.println(s.getClass().getSimpleName() + ": " + drain(s));
        }
    }
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. An interface names the operations, and each class keeps its representation in `private` fields.
- `drain` takes the interface type, so it works with any class that implements `Stack`.
