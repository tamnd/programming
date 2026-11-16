```java run id=int32
public class Main {
  public static void main(String[] args) {
    int x = Integer.MAX_VALUE;
    System.out.println("x + 1: " + (x + 1));
    try {
      Math.addExact(x, 1);
    } catch (ArithmeticException e) {
      System.out.println("Math.addExact(x, 1): " + e.getClass().getSimpleName());
    }
    long wide = (long) x + 1;
    System.out.println("(long) x + 1: " + wide);
    System.out.println("saturated: " + Math.clamp(wide, Integer.MIN_VALUE, Integer.MAX_VALUE));
    System.out.println("Integer.MIN_VALUE / -1: " + Integer.MIN_VALUE / -1);
  }
}
```

```output for=int32
x + 1: -2147483648
Math.addExact(x, 1): ArithmeticException
(long) x + 1: 2147483648
saturated: 2147483647
Integer.MIN_VALUE / -1: -2147483648
```

- Verdict: stdlib. The Java operators wrap, and `Math.addExact`, `subtractExact`, `multiplyExact` and `negateExact` throw `ArithmeticException` on overflow.
- `Math.clamp` saturates a `long` to the `int` range. A wider type and a clamp is the usual way to saturate in Java.
