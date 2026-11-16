```java run id=ratio
import java.math.BigInteger;

public class Main {
  record Ratio(BigInteger n, BigInteger d) {
    static Ratio of(long n, long d) {
      return make(BigInteger.valueOf(n), BigInteger.valueOf(d));
    }

    static Ratio make(BigInteger n, BigInteger d) {
      if (d.signum() == 0) {
        throw new ArithmeticException("denominator is 0");
      }
      if (d.signum() < 0) {
        n = n.negate();
        d = d.negate();
      }
      BigInteger g = n.gcd(d);
      return new Ratio(n.divide(g), d.divide(g));
    }

    Ratio add(Ratio o) {
      return make(n.multiply(o.d).add(o.n.multiply(d)), d.multiply(o.d));
    }

    @Override
    public String toString() {
      return n + "/" + d;
    }
  }

  public static void main(String[] args) {
    System.out.println("of(6, -8): " + Ratio.of(6, -8));
    Ratio s = Ratio.of(1, 10).add(Ratio.of(2, 10));
    System.out.println("1/10 + 2/10: " + s);
    System.out.println("equal to 3/10: " + (s.equals(Ratio.of(3, 10)) ? "yes" : "no"));
    System.out.println("0.1 + 0.2: " + (0.1 + 0.2));
    Ratio h = Ratio.of(0, 1);
    for (int k = 1; k <= 30; k++) {
      h = h.add(Ratio.of(1, k));
    }
    System.out.println("1/1 + ... + 1/30: " + h);
    try {
      Ratio.of(1, 0);
    } catch (ArithmeticException e) {
      System.out.println("of(1, 0): ArithmeticException");
    }
  }
}
```

```output for=ratio
of(6, -8): -3/4
1/10 + 2/10: 3/10
equal to 3/10: yes
0.1 + 0.2: 0.30000000000000004
1/1 + ... + 1/30: 9304682830147/2329089562800
of(1, 0): ArithmeticException
```

- Verdict: library. The JDK has no rational type, and Apache Commons Math has a `Fraction` class. The block uses a `record` of two `BigInteger` values instead, with `BigInteger.gcd` for the normal form.
- A `record` gets `equals` from its fields, so two values in lowest terms compare correctly.
