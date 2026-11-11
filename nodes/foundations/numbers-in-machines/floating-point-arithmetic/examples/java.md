```java run id=bigdecimal
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Main {
    public static void main(String[] args) {
        double x = 0.1 + 0.2;
        System.out.println("0.1 + 0.2 = " + x);
        System.out.println("exact value of 0.1: " + new BigDecimal(0.1));
        System.out.println("BigDecimal 0.1 + 0.2 = " + new BigDecimal("0.1").add(new BigDecimal("0.2")));
        System.out.println("Math.ulp(1.0) = " + Math.ulp(1.0));
        System.out.println("Math.ulp(1e16) = " + Math.ulp(1e16));

        BigDecimal price = new BigDecimal("19.99");
        BigDecimal tax = price.multiply(new BigDecimal("0.075")).setScale(2, RoundingMode.HALF_EVEN);
        System.out.println("tax on 19.99 at 7.5%: " + tax);

        double nan = Double.NaN;
        System.out.println("NaN == NaN: " + (nan == nan));
        System.out.println("Double.valueOf(NaN).equals(NaN): " + Double.valueOf(nan).equals(nan));
        System.out.println("0.0 == -0.0: " + (0.0 == -0.0));
        System.out.println("Double.compare(0.0, -0.0): " + Double.compare(0.0, -0.0));
    }
}
```

```output
0.1 + 0.2 = 0.30000000000000004
exact value of 0.1: 0.1000000000000000055511151231257827021181583404541015625
BigDecimal 0.1 + 0.2 = 0.3
Math.ulp(1.0) = 2.220446049250313E-16
Math.ulp(1e16) = 2.0
tax on 19.99 at 7.5%: 1.50
NaN == NaN: false
Double.valueOf(NaN).equals(NaN): true
0.0 == -0.0: true
Double.compare(0.0, -0.0): 1
```

- Verdict: native. `double` is IEEE 754 binary64 and `float` is binary32. Since Java 17, all floating-point operations are strict, so they give the same result on every JVM.
- `new BigDecimal(0.1)` shows the exact value of the double. `new BigDecimal("0.1")` holds exactly one tenth. Make a `BigDecimal` from a string, not from a `double`.
- `==` on `double` follows IEEE 754. `Double.equals` and `Double.compare` do not: they treat `NaN` as equal to itself and put `-0.0` below `0.0`, so that collections and sorting work.
- `Math.ulp` gives the gap to the next larger double. The gap grows with the size of the number.

A loop that adds 0.1 ten times does not reach exactly 1.0.

```java wrong id=loop
public class Main {
    public static void main(String[] args) {
        double total = 0.0;
        for (int i = 0; i < 10; i++) {
            total += 0.1;
        }
        System.out.println("total = " + total);
        System.out.println("total == 1.0: " + (total == 1.0));
    }
}
```

```output
total = 0.9999999999999999
total == 1.0: false
```
