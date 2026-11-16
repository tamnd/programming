```java run id=scaled
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Main {
  static final int SHIFT = 16;

  static int mul(int a, int b) {
    long p = ((long) a * b + (1L << (SHIFT - 1))) >> SHIFT;
    return Math.toIntExact(p);
  }

  public static void main(String[] args) {
    int a = 3 << (SHIFT - 1);
    int b = 9 << (SHIFT - 2);
    System.out.println("1.5 * 2.25: " + mul(a, b) / 65536.0);
    int big = 300 << SHIFT;
    try {
      System.out.println(mul(big, big));
    } catch (ArithmeticException e) {
      System.out.println("300 * 300: ArithmeticException");
    }
    long cents = 1999L * 3;
    System.out.printf("3 x 19.99: %d.%02d%n", cents / 100, cents % 100);
    BigDecimal tax = new BigDecimal("59.97").multiply(new BigDecimal("0.0825"));
    System.out.println("tax: " + tax + ", scale " + tax.scale());
    System.out.println("tax at scale 2: " + tax.setScale(2, RoundingMode.HALF_UP));
  }
}
```

```output for=scaled
1.5 * 2.25: 3.375
300 * 300: ArithmeticException
3 x 19.99: 59.97
tax: 4.947525, scale 6
tax at scale 2: 4.95
```

- Verdict: idiom. Java has no fixed-point type. The block multiplies two Q16.16 `int` values in a `long`, then `Math.toIntExact` throws when the result does not fit back in an `int`.
- `BigDecimal` has an explicit scale. The product of a scale 2 value and a scale 4 value has scale 6, and `setScale` rounds it back to scale 2.
