```java run id=sequenced-collection
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.SequencedCollection;
import java.util.stream.Stream;

public class Main {
    // One method for every collection with a defined order: lists, deques and ordered sets.
    static String ends(SequencedCollection<String> c) {
        return c.getFirst() + ".." + c.getLast() + " " + c.reversed();
    }

    public static void main(String[] args) {
        List<String> list = new ArrayList<>(List.of("a", "b", "c"));
        Deque<String> deque = new ArrayDeque<>(List.of("x", "y"));
        LinkedHashSet<String> set = new LinkedHashSet<>(List.of("k", "j", "k", "i"));
        System.out.println(ends(list) + " | " + ends(deque) + " | " + ends(set));

        // reversed() is a view: a change through it changes the list.
        List<String> back = list.reversed();
        back.addFirst("z");
        System.out.println(list + " " + back);

        // HashSet has no defined order, so it is not a SequencedCollection.
        System.out.println((Object) new HashSet<String>() instanceof SequencedCollection);

        // A Stream is a lazy sequence. iterate gives an infinite stream, and limit makes it finite.
        System.out.println(Stream.iterate(1, n -> n * 2).limit(6).toList());
    }
}
```

```output
a..c [c, b, a] | x..y [y, x] | k..i [i, j, k]
[a, b, c, z] [z, c, b, a]
false
[1, 2, 4, 8, 16, 32]
```

- Verdict: stdlib. Since Java 21, one method takes any SequencedCollection: a list, a deque or a LinkedHashSet. reversed() is a view, so addFirst through it adds to the end of the list. A HashSet has no defined order and is not a SequencedCollection. Stream.iterate gives an infinite lazy stream, and limit makes it finite.
