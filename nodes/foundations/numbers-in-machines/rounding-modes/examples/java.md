```java run id=ties
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Main {
  public static void main(String[] args) {
    String[] inputs = {"2.5", "-2.5", "2.6", "-2.4"};
    for (RoundingMode mode : RoundingMode.values()) {
      StringBuilder row = new StringBuilder(String.format("%-12s", mode));
      for (String s : inputs) {
        try {
          row.append(String.format("%4s", new BigDecimal(s).setScale(0, mode)));
        } catch (ArithmeticException e) {
          row.append("   !");
        }
      }
      System.out.println(row);
    }

    System.out.println("Math.round(2.5): " + Math.round(2.5) + ", Math.round(-2.5): " + Math.round(-2.5));
    System.out.println("Math.rint(2.5): " + Math.rint(2.5) + ", Math.rint(3.5): " + Math.rint(3.5));
    System.out.println("(int) -2.7: " + (int) -2.7);
    System.out.println("new BigDecimal(\"2.675\") HALF_UP: " + new BigDecimal("2.675").setScale(2, RoundingMode.HALF_UP));
    System.out.println("new BigDecimal(2.675) HALF_UP:   " + new BigDecimal(2.675).setScale(2, RoundingMode.HALF_UP));
  }
}
```

```output for=ties
UP             3  -3   3  -3
DOWN           2  -2   2  -2
CEILING        3  -2   3  -2
FLOOR          2  -3   2  -3
HALF_UP        3  -3   3  -2
HALF_DOWN      2  -2   3  -2
HALF_EVEN      2  -2   3  -2
UNNECESSARY    !   !   !   !
Math.round(2.5): 3, Math.round(-2.5): -2
Math.rint(2.5): 2.0, Math.rint(3.5): 4.0
(int) -2.7: -2
new BigDecimal("2.675") HALF_UP: 2.68
new BigDecimal(2.675) HALF_UP:   2.67
```

- Verdict: native. `java.math.RoundingMode` has eight modes for `BigDecimal`.
- `UNNECESSARY` throws `ArithmeticException` when the result is not exact, here shown as `!`.
- `Math.round` sends ties toward plus infinity, and `Math.rint` sends them to the even neighbour. A cast to `int` truncates toward zero.
- `new BigDecimal(2.675)` holds the exact value of the double, which is a little below 2.675, so it rounds down.
