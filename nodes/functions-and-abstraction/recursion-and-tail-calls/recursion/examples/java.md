```java run id=tree
public class Main {
    sealed interface Tree permits Leaf, Node {}

    record Leaf(int value) implements Tree {}

    record Node(Tree left, Tree right) implements Tree {}

    static int total(Tree tree) {
        return switch (tree) {
            case Leaf leaf -> leaf.value();
            case Node node -> total(node.left()) + total(node.right());
        };
    }

    static int depth(int n) {
        return n == 0 ? 0 : 1 + depth(n - 1);
    }

    public static void main(String[] args) {
        Tree tree = new Node(new Leaf(1), new Node(new Leaf(2), new Leaf(3)));
        System.out.println(total(tree));
        System.out.println(depth(1000));
        try {
            depth(100_000_000);
        } catch (StackOverflowError error) {
            System.out.println("caught " + error.getClass().getName());
        }
    }
}
```

```output for=tree
6
1000
caught java.lang.StackOverflowError
```

- Verdict: native. A method can call itself. Each call creates a fresh frame on the stack of its thread.
- When a thread needs more stack than the JVM permits, the JVM throws `StackOverflowError`. It is an `Error`, not an `Exception`, but a `catch` clause can still catch it.
- The `-Xss` option of the `java` launcher sets the stack size of a thread. The default depends on the platform.
- The JVM creates a frame each time a method is invoked, so a call in tail position also takes a frame.
- A sealed interface with records gives a recursive data type, and a `switch` over it covers each case.
