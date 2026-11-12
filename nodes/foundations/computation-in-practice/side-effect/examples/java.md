```java run id=views-and-lazy-streams
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Stream;

public class Main {
    static void addAll(List<String> target, String... items) { // changes its argument
        Collections.addAll(target, items);
    }

    public static void main(String[] args) {
        List<String> names = new ArrayList<>(List.of("Ada"));
        List<String> view = Collections.unmodifiableList(names); // a read-only view
        List<String> copy = List.copyOf(names); // an immutable copy
        addAll(names, "Alan", "Grace");
        System.out.println("view " + view + ", copy " + copy);
        try {
            view.add("Edsger");
        } catch (UnsupportedOperationException e) {
            System.out.println("the view rejects add");
        }

        // Streams are lazy: the peek runs only when a terminal operation pulls items.
        Stream<Integer> s = Stream.of(1, 2, 3).peek(x -> System.out.println("peek " + x));
        System.out.println("no terminal operation yet");
        System.out.println("sum " + s.mapToInt(Integer::intValue).sum());

        long n = List.of(1, 2, 3).stream().peek(x -> System.out.println("count peek " + x)).count();
        System.out.println("count " + n + " (the count came from the list size)");
    }
}
```

```output for=views-and-lazy-streams
view [Ada, Alan, Grace], copy [Ada]
the view rejects add
no terminal operation yet
peek 1
peek 2
peek 3
sum 6
count 3 (the count came from the list size)
```

- Verdict: native. A method can change any object that it can reach. `Collections.unmodifiableList` gives a read-only view: the view rejects `add`, but it shows the changes that other code makes to the list behind it. `List.copyOf` makes an immutable copy that does not change. Streams are lazy, so a `peek` runs only when a terminal operation pulls items, and `count` can return the size of the source and skip the pipeline, so the `peek` before it does not run.
