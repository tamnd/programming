```java run id=traps
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public class Main {
  public static void main(String[] args) {
    double nan = Double.NaN;
    System.out.println("nan == nan: " + (nan == nan));
    System.out.println("Double.isNaN: " + Double.isNaN(0.0 / 0.0));
    System.out.println("1.0 / 0: " + (1.0 / 0) + ", 1.0 / -0.0: " + (1.0 / -0.0));
    System.out.println("Double.equals(NaN, NaN): " + Double.valueOf(nan).equals(nan));
    System.out.println("0.0 == -0.0: " + (0.0 == -0.0) + ", equals: " + Double.valueOf(0.0).equals(-0.0));

    Double[] boxed = {3.0, nan, 1.0, -0.0, 0.0};
    Arrays.sort(boxed);
    System.out.println("sorted Double[]: " + Arrays.toString(boxed));

    Set<Double> set = new HashSet<>();
    set.add(nan);
    set.add(0.0 / 0.0);
    System.out.println("set size: " + set.size() + ", contains NaN: " + set.contains(Double.NaN));
    System.out.println("Math.max(1.0, NaN): " + Math.max(1.0, nan));
    try {
      int i = 1 / 0;
      System.out.println(i);
    } catch (ArithmeticException e) {
      System.out.println("int 1 / 0: " + e.getMessage());
    }
  }
}
```

```output for=traps
nan == nan: false
Double.isNaN: true
1.0 / 0: Infinity, 1.0 / -0.0: -Infinity
Double.equals(NaN, NaN): true
0.0 == -0.0: true, equals: false
sorted Double[]: [-0.0, 0.0, 1.0, 3.0, NaN]
set size: 1, contains NaN: true
Math.max(1.0, NaN): NaN
int 1 / 0: / by zero
```

- Verdict: native. Java `double` follows IEEE 754 for `==` and `<`.
- `Double.equals` and `Double.compare` treat NaN as equal to itself and put `-0.0` before `0.0`, so a sort of `Double[]` puts NaN last and a `HashSet` finds it.
- Integer division by zero throws `ArithmeticException`, because `int` has no infinity.
