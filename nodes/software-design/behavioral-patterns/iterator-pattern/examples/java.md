```java run id=tree
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.StringJoiner;
import java.util.stream.StreamSupport;

public class Main {
    static final class Tree implements Iterable<Integer> {
        private static final class Node {
            final int key;
            Node left;
            Node right;

            Node(int key) {
                this.key = key;
            }
        }

        private Node root;

        Tree(int... keys) {
            for (int key : keys) {
                insert(key);
            }
        }

        void insert(int key) {
            if (root == null) {
                root = new Node(key);
                return;
            }
            Node node = root;
            while (true) {
                if (key < node.key) {
                    if (node.left == null) {
                        node.left = new Node(key);
                        return;
                    }
                    node = node.left;
                } else {
                    if (node.right == null) {
                        node.right = new Node(key);
                        return;
                    }
                    node = node.right;
                }
            }
        }

        // The factory method of the aggregate: a fresh iterator for each traversal.
        @Override
        public Iterator<Integer> iterator() {
            return new Iterator<>() {
                private final Deque<Node> stack = new ArrayDeque<>();

                {
                    pushLeft(root);
                }

                private void pushLeft(Node node) {
                    for (; node != null; node = node.left) {
                        stack.push(node);
                    }
                }

                @Override
                public boolean hasNext() {
                    return !stack.isEmpty();
                }

                @Override
                public Integer next() {
                    if (stack.isEmpty()) {
                        throw new NoSuchElementException();
                    }
                    Node node = stack.pop();
                    pushLeft(node.right);
                    return node.key;
                }
            };
        }
    }

    static boolean sameKeys(Iterable<Integer> a, Iterable<Integer> b) {
        Iterator<Integer> left = a.iterator();
        Iterator<Integer> right = b.iterator();
        while (left.hasNext() && right.hasNext()) {
            if (!left.next().equals(right.next())) {
                return false;
            }
        }
        return !left.hasNext() && !right.hasNext();
    }

    public static void main(String[] args) {
        Tree a = new Tree(5, 3, 8, 1, 4, 9);
        Tree b = new Tree(1, 3, 4, 5, 8, 9);
        Tree c = new Tree(1, 3, 4, 5, 8);

        StringJoiner keys = new StringJoiner(" ");
        for (int key : a) {
            keys.add(Integer.toString(key));
        }
        System.out.println("in order: " + keys);

        int sum = StreamSupport.stream(a.spliterator(), false).mapToInt(Integer::intValue).sum();
        System.out.println("sum: " + sum);

        Iterator<Integer> it = a.iterator();
        System.out.println("first two: " + it.next() + " " + it.next());
        System.out.println("same keys as b: " + (sameKeys(a, b) ? "yes" : "no"));
        System.out.println("same keys as c: " + (sameKeys(a, c) ? "yes" : "no"));
    }
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A class that implements `Iterable<T>` can be the target of the enhanced `for` statement. Its `iterator()` method returns an `Iterator<T>` with `hasNext()` and `next()`.
- `iterator()` is a factory method. Each call makes an anonymous class with its own stack, so `sameKeys` runs two traversals at once.
- Java has no generators, so the state of the traversal lives in fields of the iterator. `next()` throws `NoSuchElementException` at the end, as the `Iterator` contract asks.
- `Iterable` has a default `spliterator()`, so `StreamSupport.stream` turns the tree into a stream. `Iterable.forEach` is the internal form: the tree calls the action for each element.
