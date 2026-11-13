```java run id=intstream-range
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        // range excludes the end, and rangeClosed includes it.
        System.out.println(IntStream.range(0, 5).boxed().toList() + " " + IntStream.rangeClosed(1, 5).boxed().toList());

        // An empty range gives no items. The sum of 1 to 100 needs rangeClosed.
        System.out.println(IntStream.range(5, 5).count() + " " + IntStream.range(1, 100).sum() + " " + IntStream.rangeClosed(1, 100).sum());

        // IntStream has no step argument. iterate with a condition gives a range with a step.
        System.out.println(IntStream.iterate(10, i -> i > 0, i -> i - 3).boxed().toList());

        // The classic for loop is the most common range in Java.
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 5; i++) sb.append(i);
        System.out.println(sb);
    }
}
```

```output
[0, 1, 2, 3, 4] [1, 2, 3, 4, 5]
0 4950 5050
[10, 7, 4, 1]
01234
```

- Verdict: stdlib. IntStream.range excludes the end and rangeClosed includes it, so the sum of range(1, 100) is 4950 and the sum of rangeClosed(1, 100) is 5050. An empty range has a count of 0. IntStream has no step argument, so iterate with a condition gives a count down by 3. The for loop with < is the usual range.
