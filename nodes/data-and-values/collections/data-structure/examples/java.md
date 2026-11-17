```java run id=two-layouts
import java.util.StringJoiner;

public class Main {
    record Node(int value, Node next) {}

    public static void main(String[] args) {
        int[] array = {10, 20, 30, 40, 50};
        Node head = null;
        for (int i = array.length - 1; i >= 0; i--) {
            head = new Node(array[i], head);
        }

        System.out.println("array[3] = " + array[3] + " (0 steps)");
        Node n = head;
        int steps = 0;
        for (int i = 3; i > 0; i--, steps++) {
            n = n.next();
        }
        System.out.println("linked[3] = " + n.value() + " (" + steps + " steps)");

        head = new Node(5, head);
        StringJoiner out = new StringJoiner(" ");
        for (Node m = head; m != null; m = m.next()) {
            out.add(String.valueOf(m.value()));
        }
        System.out.println("after push_front: " + out);
    }
}
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: stdlib. Java has arrays in the language, and `ArrayList`, `LinkedList`, `ArrayDeque`, `HashMap` and `TreeMap` in `java.util`.
- The example writes its own node as a record, to show the pointer steps. In a program, use `java.util.LinkedList` or, more often, `ArrayList`.
