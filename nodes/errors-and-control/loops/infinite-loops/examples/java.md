```java run id=while-true-without-return-and-a-byte-counter
import java.util.List;

public class Main {
    // No return after the loop: while (true) without break cannot complete normally.
    static int firstNegative(List<Integer> xs) {
        int i = 0;
        while (true) {
            if (xs.get(i) < 0) {
                return i;
            }
            i++;
        }
    }

    public static void main(String[] args) {
        System.out.println("index: " + firstNegative(List.of(4, 7, -2, 9)));

        // A byte is always under 200, so only the guard ends this loop.
        byte b = 0;
        int passes = 0;
        while (b < 200 && passes < 1000) {
            b++;
            passes++;
        }
        System.out.println("byte b = " + b + " after " + passes + " passes");
    }
}
```

```output for=while-true-without-return-and-a-byte-counter
index: 2
byte b = -24 after 1000 passes
```

- Verdict: idiom. `firstNegative` compiles with no `return` after the loop, because the compiler knows that `while (true)` with no `break` cannot fall through. For a list with no negative number, `get` throws an exception, which is the only other exit. A `byte` goes from 127 to -128, so `b < 200` is always true and the loop without the guard would never end.
