```java run id=tree
import java.util.List;

public class Main {
    record Node(String name, int size, List<Node> children) {
        static Node file(String name, int size) {
            return new Node(name, size, List.of());
        }

        static Node dir(String name, Node... children) {
            return new Node(name, 0, List.of(children));
        }
    }

    static void show(Node node, int depth) {
        String indent = "  ".repeat(depth);
        if (node.children().isEmpty()) {
            System.out.println(indent + node.name() + " " + node.size());
            return;
        }
        System.out.println(indent + node.name() + "/");
        for (Node child : node.children()) {
            show(child, depth + 1);
        }
    }

    static int countFiles(Node node) {
        if (node.children().isEmpty()) {
            return 1;
        }
        return node.children().stream().mapToInt(Main::countFiles).sum();
    }

    static int totalSize(Node node) {
        return node.size() + node.children().stream().mapToInt(Main::totalSize).sum();
    }

    static int height(Node node) {
        return node.children().stream().mapToInt(child -> 1 + height(child)).max().orElse(0);
    }

    public static void main(String[] args) {
        Node root = Node.dir("project",
                Node.file("README.md", 120),
                Node.dir("src", Node.file("main.py", 300), Node.file("util.py", 80)),
                Node.dir("tests", Node.file("test_main.py", 150)));
        show(root, 0);
        System.out.println("files " + countFiles(root) + ", total size " + totalSize(root)
                + ", height " + height(root));
    }
}
```

```output for=tree
project/
  README.md 120
  src/
    main.py 300
    util.py 80
  tests/
    test_main.py 150
files 4, total size 650, height 2
```

- Verdict: idiom. A record with a `List` of children is a node. `List.of` makes the lists unmodifiable.
- The functions use streams to sum and to find the maximum over the children.
