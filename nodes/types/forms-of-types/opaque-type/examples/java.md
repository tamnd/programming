```java run id=hidden-class
import java.util.ArrayList;
import java.util.List;

public class Main {
    // The public face of the type: callers see only these methods.
    interface Stack {
        void push(int x);
        int pop();
        boolean isEmpty();
    }

    // The representation is private to Main, so callers cannot name it.
    private static final class ListStack implements Stack {
        private final List<Integer> items = new ArrayList<>();

        public void push(int x) { items.add(x); }
        public int pop() { return items.remove(items.size() - 1); }
        public boolean isEmpty() { return items.isEmpty(); }
    }

    static Stack newStack() {
        return new ListStack();
    }

    public static void main(String[] args) {
        Stack s = newStack();
        s.push(1);
        s.push(2);
        System.out.println(s.pop() + " " + s.pop() + " " + s.isEmpty());
        System.out.println(s instanceof Stack);
    }
}
```

```output for=hidden-class
2 1 true
true
```

- Verdict: idiom. Java has no opaque type declaration. A factory returns the interface `Stack`, and the class `ListStack` and its list stay private. Code outside can change only through the interface, so the class could switch to an array without any change to the callers.
