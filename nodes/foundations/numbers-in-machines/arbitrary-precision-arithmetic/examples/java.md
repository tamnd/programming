```java run id=biginteger
import java.math.BigInteger;

public class Main {
  public static void main(String[] args) {
    BigInteger two = BigInteger.TWO;
    System.out.println("2^64: " + two.pow(64));
    System.out.println("2^100: " + two.pow(100));
    BigInteger f = BigInteger.ONE;
    for (int i = 1; i <= 30; i++) {
      f = f.multiply(BigInteger.valueOf(i));
    }
    System.out.println("30!: " + f);
    long a = Long.MAX_VALUE;
    System.out.println("valueOf(a * 2): " + BigInteger.valueOf(a * 2));
    System.out.println("valueOf(a).multiply(2): " + BigInteger.valueOf(a).multiply(two));
    BigInteger big = two.pow(64);
    try {
      System.out.println(big.longValueExact());
    } catch (ArithmeticException e) {
      System.out.println("longValueExact(): ArithmeticException");
    }
    System.out.println("longValue(): " + big.longValue());
    BigInteger m7 = BigInteger.valueOf(-7);
    BigInteger three = BigInteger.valueOf(3);
    System.out.println("-7 remainder 3: " + m7.remainder(three));
    System.out.println("-7 mod 3: " + m7.mod(three));
  }
}
```

```output for=biginteger
2^64: 18446744073709551616
2^100: 1267650600228229401496703205376
30!: 265252859812191058636308480000000
valueOf(a * 2): -2
valueOf(a).multiply(2): 18446744073709551614
longValueExact(): ArithmeticException
longValue(): 0
-7 remainder 3: -1
-7 mod 3: 2
```

- Verdict: stdlib. `java.math.BigInteger` is a class, so the program calls methods such as `multiply` and `pow` in place of operators.
- `BigInteger.valueOf(a * 2)` overflows in `long` before the conversion. Convert first, then multiply.
- `longValueExact` throws when the value does not fit, and `longValue` keeps only the low 64 bits.
- `remainder` follows `%`, and `mod` gives a result that is never negative.
