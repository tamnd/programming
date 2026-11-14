```java run id=hashset-treeset
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

public class Main {
  public static void main(String[] args) {
    // add returns false when the set has the element already.
    Set<String> s = new HashSet<>();
    System.out.println(s.add("a") + " " + s.add("a") + " " + s.size());

    // A TreeSet keeps its elements sorted, and a HashSet has no defined order.
    Set<Integer> sorted = new TreeSet<>(List.of(3, 1, 2));
    System.out.println(sorted);

    // Set algebra changes the set in place, so copy it first.
    Set<Integer> a = new TreeSet<>(List.of(1, 2, 3));
    Set<Integer> b = Set.of(2, 3, 4);
    Set<Integer> union = new TreeSet<>(a);
    union.addAll(b);
    Set<Integer> inter = new TreeSet<>(a);
    inter.retainAll(b);
    Set<Integer> diff = new TreeSet<>(a);
    diff.removeAll(b);
    System.out.println(union + " " + inter + " " + diff + " " + a.containsAll(Set.of(2, 3)));

    // Set.of rejects duplicate elements.
    try {
      Set.of("x", "x");
    } catch (IllegalArgumentException e) {
      System.out.println("IllegalArgumentException");
    }

    // A mutable element that changes is lost: its hash code no longer matches its bucket.
    List<Integer> key = new ArrayList<>(List.of(1, 2));
    Set<List<Integer>> lists = new HashSet<>();
    lists.add(key);
    key.add(3);
    System.out.println(lists.contains(key) + " " + lists.contains(List.of(1, 2)) + " " + lists.size());
  }
}
```

```output
true false 1
[1, 2, 3]
[1, 2, 3, 4] [2, 3] [1] true
IllegalArgumentException
false false 1
```

- Verdict: stdlib. add returns false for an element that is there already. TreeSet keeps the elements sorted. addAll, retainAll and removeAll change the set in place, so the example copies it first. Set.of rejects duplicates. A list that changes after it goes into a HashSet is not found again.
