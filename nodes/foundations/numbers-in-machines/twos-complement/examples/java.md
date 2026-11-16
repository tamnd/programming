```java run id=negation
public class Main {
  public static void main(String[] args) {
    int x = 6;
    System.out.println("-x == ~x + 1: " + (-x == ~x + 1 ? "yes" : "no"));
    System.out.println("-1 in hex: " + Integer.toHexString(-1));
    System.out.println("-Integer.MIN_VALUE: " + -Integer.MIN_VALUE);
    System.out.println("Math.abs(Integer.MIN_VALUE): " + Math.abs(Integer.MIN_VALUE));
    try {
      Math.negateExact(Integer.MIN_VALUE);
    } catch (ArithmeticException e) {
      System.out.println("Math.negateExact(Integer.MIN_VALUE): ArithmeticException");
    }

    byte b = (byte) 0xFA;
    System.out.println("(int) b: " + (int) b);
    System.out.println("b & 0xFF: " + (b & 0xFF));
  }
}
```

```output for=negation
-x == ~x + 1: yes
-1 in hex: ffffffff
-Integer.MIN_VALUE: -2147483648
Math.abs(Integer.MIN_VALUE): -2147483648
Math.negateExact(Integer.MIN_VALUE): ArithmeticException
(int) b: -6
b & 0xFF: 250
```

- Verdict: native. The Java Language Specification defines `byte`, `short`, `int` and `long` as two's complement integers.
- The negation of `Integer.MIN_VALUE` is the same value, with no exception. `Math.negateExact` and `Math.absExact` throw an `ArithmeticException` instead.
- A cast from `byte` to `int` copies the sign bit. The mask `& 0xFF` keeps only the low 8 bits.
