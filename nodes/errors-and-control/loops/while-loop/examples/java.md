```java run id=iterator-loop-and-digit-count
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // An Iterator loop: hasNext is the condition.
        List<Integer> nums = new ArrayList<>(List.of(5, 8, 13, 21));
        Iterator<Integer> it = nums.iterator();
        while (it.hasNext()) {
            int x = it.next();
            if (x % 2 == 0) {
                it.remove();
            }
        }
        System.out.println(nums);

        // Count the digits: the count is not known in advance.
        long n = 9_876_543_210L;
        int digits = 0;
        while (n > 0) {
            n /= 10;
            digits++;
        }
        System.out.println("digits: " + digits);

        // The condition is tested only between passes.
        int i = 0;
        while (i < 1) {
            i = 5;
            System.out.println("the body runs to its end, i = " + i);
        }
    }
}
```

```output for=iterator-loop-and-digit-count
[5, 13, 21]
digits: 10
the body runs to its end, i = 5
```

- Verdict: native. A while loop over an `Iterator` tests `hasNext`, and `it.remove()` removes the even number with no `ConcurrentModificationException`. The digit loop runs 10 times for a 10-digit number. In the last loop, the body makes the condition false in its first line, and the `println` after it still runs.
