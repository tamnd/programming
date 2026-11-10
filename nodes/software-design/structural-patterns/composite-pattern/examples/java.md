```java run id=tree
import java.util.List;
import java.util.stream.Stream;

public class Main {
    sealed interface Entry permits File, Directory {}

    record File(String name, long size) implements Entry {}

    record Directory(String name, List<Entry> children) implements Entry {
        Directory(String name, Entry... children) {
            this(name, List.of(children));
        }
    }

    static long size(Entry entry) {
        return switch (entry) {
            case File f -> f.size();
            case Directory d -> d.children().stream().mapToLong(Main::size).sum();
        };
    }

    static Stream<String> lines(Entry entry, int depth) {
        String indent = "  ".repeat(depth);
        return switch (entry) {
            case File f -> Stream.of(indent + f.name() + " " + f.size());
            case Directory d -> Stream.concat(
                    Stream.of(indent + d.name() + "/ " + size(d)),
                    d.children().stream().flatMap(child -> lines(child, depth + 1)));
        };
    }

    public static void main(String[] args) {
        Entry project = new Directory("project",
                new File("readme.md", 200),
                new Directory("src", new File("main.go", 1200), new File("util.go", 300)));
        lines(project, 0).forEach(System.out::println);
    }
}
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. A sealed interface lists its two implementations, and records hold the data. A `switch` over a sealed type needs no `default`, because the compiler knows every case.
- The operations live outside the types, as functions with a case for each type. The classic form puts `size` in each class as a method. Both are composites: the client calls `size` on any entry and does not test whether it is a leaf.
- `List.of` makes the children immutable, so a directory cannot gain itself as a child after it is built, and the tree has no cycles.
