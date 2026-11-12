```java run id=promotion-and-big-classes
import java.math.BigDecimal;
import java.math.BigInteger;

public class Main {
    public static void main(String[] args) {
        // Integer division truncates, and % takes the sign of the dividend.
        System.out.println((7 / 2) + " " + (-7 / 2) + " " + (-7 % 2) + " " + Math.floorDiv(-7, 2) + " " + Math.floorMod(-7, 2));

        // Arithmetic on byte, short and char promotes the operands to int.
        byte a = 100, b = 100;
        int sum = a + b;
        byte wrapped = (byte) (a + b);
        System.out.println(sum + " " + wrapped + " " + ('A' + 1) + " " + (char) ('A' + 1));

        // int times int is an int, even when the result is stored in a long.
        int big = 1_000_000;
        long wrong = big * big;
        long right = (long) big * big;
        System.out.println(wrong + " " + right);

        // Integer division by zero throws, but floating-point division gives Infinity.
        try {
            System.out.println(1 / (big - big));
        } catch (ArithmeticException e) {
            System.out.println("ArithmeticException " + (1.0 / 0) + " " + (0.0 / 0));
        }

        // BigInteger and BigDecimal are classes for exact arithmetic.
        BigInteger f = BigInteger.ONE;
        for (int i = 2; i <= 30; i++) f = f.multiply(BigInteger.valueOf(i));
        System.out.println(f + " " + (0.1 + 0.2) + " " + new BigDecimal("0.1").add(new BigDecimal("0.2")));

        // The ranges come from the wrapper classes.
        System.out.println(Integer.MAX_VALUE + " " + Long.MAX_VALUE + " " + Double.MAX_VALUE);
    }
}
```

```output for=promotion-and-big-classes
3 -3 -1 -4 1
200 -56 66 B
-727379968 1000000000000
ArithmeticException Infinity NaN
265252859812191058636308480000000 0.30000000000000004 0.3
2147483647 9223372036854775807 1.7976931348623157E308
```

- Verdict: native. `/` truncates and `%` takes the sign of the dividend, and `Math.floorDiv` and `Math.floorMod` round down. Arithmetic on `byte` and `char` gives an `int`, so `a + b` is 200 and the cast to `byte` gives -56. `big * big` wraps as an `int` before it is stored in a `long`. Integer division by zero throws `ArithmeticException`, and `1.0 / 0` is `Infinity`. `BigInteger` and `BigDecimal` give exact results.
