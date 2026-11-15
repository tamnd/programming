```java run id=cme-iterator-remove-and-removeif
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> nums = new ArrayList<>(List.of(1, 2, 3, 4, 5));
        try {
            for (Integer n : nums) {
                if (n % 2 == 0) nums.remove(n);
            }
        } catch (ConcurrentModificationException e) {
            System.out.println("caught " + e.getClass().getSimpleName());
        }
        System.out.println("after the error: " + nums);

        // The iterator's own remove keeps it valid.
        nums = new ArrayList<>(List.of(1, 2, 3, 4, 5));
        for (Iterator<Integer> it = nums.iterator(); it.hasNext(); ) {
            if (it.next() % 2 == 0) it.remove();
        }
        System.out.println("Iterator.remove: " + nums);

        // removeIf does the loop inside the list.
        nums = new ArrayList<>(List.of(1, 2, 3, 4, 5));
        nums.removeIf(n -> n % 2 == 0);
        System.out.println("removeIf: " + nums);

        // set is not a structural change, so it does not throw.
        for (int i = 0; i < nums.size(); i++) nums.set(i, nums.get(i) * 10);
        for (Integer n : nums) nums.set(0, n);
        System.out.println("after set: " + nums);
    }
}
```

```output for=cme-iterator-remove-and-removeif
caught ConcurrentModificationException
after the error: [1, 3, 4, 5]
Iterator.remove: [1, 3, 5]
removeIf: [1, 3, 5]
after set: [50, 30, 50]
```

- Verdict: native. The for-each loop removes 2, and the next call to `next` finds that the list changed and throws, so 4 stays in the list. `Iterator.remove` removes the element that `next` returned and keeps the iterator valid. `removeIf` needs no loop in user code. `set` changes a value and not the structure, so the last loop does not throw: it writes each element into index 0, and the last one is 50.
