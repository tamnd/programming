```java run id=substring-and-index-out-of-bounds
import java.util.List;
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        int[] scores = {70, 85, 90};
        int total = 0;
        try {
            for (int i = 0; i <= scores.length; i++) {
                total += scores[i];
            }
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("caught: " + e.getMessage());
        }
        System.out.println("total before the error: " + total);

        // substring(begin, end) excludes end, so its length is end - begin.
        String s = "hamburger";
        String part = s.substring(4, 8);
        System.out.println(part + " " + part.length());

        // IntStream.range excludes the end, rangeClosed includes it.
        System.out.println(IntStream.range(1, 5).boxed().toList());
        System.out.println(IntStream.rangeClosed(1, 5).boxed().toList());

        // subList is half-open too.
        List<String> days = List.of("mon", "tue", "wed", "thu", "fri");
        System.out.println(days.subList(1, 3));
    }
}
```

```output for=substring-and-index-out-of-bounds
caught: Index 3 out of bounds for length 3
total before the error: 245
urge 4
[1, 2, 3, 4]
[1, 2, 3, 4, 5]
[tue, wed]
```

- Verdict: native. The JVM checks each array index, so the fourth pass of the loop with `<=` throws an `ArrayIndexOutOfBoundsException` before it reads anything. `substring(4, 8)` has length 8 − 4. `IntStream.range` and `subList` exclude the end, and `rangeClosed` includes it, so the two names tell the reader which form is in use.
