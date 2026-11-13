```java run id=list-interface
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class Main {
    static int sum(List<Integer> items) {
        int total = 0;
        for (int x : items) {
            total += x;
        }
        return total;
    }

    public static void main(String[] args) {
        // List is an interface. ArrayList and LinkedList are two implementations of it.
        List<Integer> array = new ArrayList<>(List.of(3, 1, 2));
        List<Integer> linked = new LinkedList<>(List.of(3, 1, 2));
        array.add(0, 9);
        linked.add(0, 9);
        System.out.println(array + " " + linked + " " + sum(array) + " " + sum(linked));

        // equals compares the items in order, so the two implementations are equal.
        System.out.println(array.equals(linked) + " " + (array.hashCode() == linked.hashCode()));

        // List.of makes an unmodifiable list. A mutator throws.
        List<String> fixed = List.of("a", "b");
        try {
            fixed.add("c");
        } catch (UnsupportedOperationException e) {
            System.out.println("UnsupportedOperationException");
        }

        // A list allows duplicates and keeps its order. indexOf finds the first match.
        List<String> words = new ArrayList<>(List.of("b", "a", "b"));
        System.out.println(words.indexOf("b") + " " + words.lastIndexOf("b") + " " + words.getFirst());
    }
}
```

```output
[9, 3, 1, 2] [9, 3, 1, 2] 15 15
true true
UnsupportedOperationException
0 2 b
```

- Verdict: stdlib. java.util.List is an interface. One method takes an ArrayList and a LinkedList and gives the same sum, and equals compares the items across the two classes. List.of gives an unmodifiable list, and add throws UnsupportedOperationException. indexOf and lastIndexOf find the first and the last of two equal items.
