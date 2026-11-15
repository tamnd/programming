```java run id=four-loop-forms-labels-and-streams
import java.util.List;
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        int sum = 0;
        for (int i = 1; i <= 5; i++) {
            sum += i;
        }
        System.out.println("for: " + sum);

        for (String s : List.of("x", "y")) {
            System.out.println("enhanced for: " + s);
        }

        int n = 100;
        int steps = 0;
        while (n > 1) {
            n /= 2;
            steps++;
        }
        System.out.println("while: " + steps);

        // The body of a do statement runs at least once.
        int runs = 0;
        do {
            runs++;
        } while (runs < 0);
        System.out.println("do: " + runs);

        // A labeled break leaves the outer loop.
        outer:
        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++) {
                if (a + b == 3) {
                    System.out.println("break outer at " + a + " " + b);
                    break outer;
                }
            }
        }

        // A stream replaces the counted loop.
        System.out.println("stream: " + IntStream.rangeClosed(1, 5).sum());
    }
}
```

```output for=four-loop-forms-labels-and-streams
for: 15
enhanced for: x
enhanced for: y
while: 6
do: 1
break outer at 1 2
stream: 15
```

- Verdict: native. Java has the basic `for`, the enhanced `for` over arrays and `Iterable` objects, `while` and `do`. The `do` body runs once although `runs < 0` is false. `break outer` leaves both loops. `IntStream.rangeClosed(1, 5).sum()` gets the same sum as the first loop with no loop statement.
