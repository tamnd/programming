```java run id=enhanced-for-and-concurrent-modification
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        int[] scores = {70, 85, 92};
        int total = 0;
        for (int s : scores) {
            total += s;
        }
        System.out.println("total: " + total);

        List<String> names = new ArrayList<>(List.of("ann", "cy", "bob", "dee"));
        try {
            for (String n : names) {
                if (n.length() == 2) {
                    names.remove(n);
                }
            }
        } catch (ConcurrentModificationException e) {
            System.out.println("ConcurrentModificationException");
        }

        List<String> fresh = new ArrayList<>(List.of("ann", "cy", "bob", "dee"));
        fresh.removeIf(n -> n.length() == 2);
        System.out.println(fresh);
    }
}
```

```output for=enhanced-for-and-concurrent-modification
total: 247
ConcurrentModificationException
[ann, bob, dee]
```

- Verdict: native. The enhanced `for` works on an array and on any `Iterable`. A `remove` on the list during the loop makes the hidden iterator fail at its next step with a `ConcurrentModificationException`. `removeIf` removes the elements safely in one call.
