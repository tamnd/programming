```java run id=arraylist
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // ArrayList keeps its items in an array that grows when it is full.
        ArrayList<Integer> a = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            a.add(i * 10);
        }
        System.out.println(a + " " + a.size() + " " + a.get(4));

        // ensureCapacity grows the array once before many adds.
        ArrayList<Integer> big = new ArrayList<>(10);
        big.ensureCapacity(1_000_000);
        for (int i = 0; i < 1_000_000; i++) {
            big.add(i);
        }
        big.trimToSize();
        System.out.println(big.size() + " " + big.getLast());

        // add at index 0 moves every other item one place to the right.
        a.add(0, -10);
        a.removeLast();
        System.out.println(a);

        // A change to the list during a for-each loop makes the iterator fail.
        List<Integer> c = new ArrayList<>(List.of(1, 2, 3));
        try {
            for (int x : c) {
                if (x == 1) {
                    c.add(4);
                }
            }
        } catch (ConcurrentModificationException e) {
            System.out.println("ConcurrentModificationException");
        }
    }
}
```

```output
[0, 10, 20, 30, 40] 5 40
1000000 999999
[-10, 0, 10, 20, 30]
ConcurrentModificationException
```

- Verdict: stdlib. ArrayList grows its array as add runs, and the capacity is not visible. ensureCapacity grows it once before a million adds, and trimToSize gives the spare slots back. add(0, x) moves the other items. A change to the list inside a for-each loop throws ConcurrentModificationException.
