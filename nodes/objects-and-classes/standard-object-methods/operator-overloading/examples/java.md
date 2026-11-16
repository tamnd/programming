```java run id=methods-and-records
import java.math.BigInteger;

public class Main {
    record Money(long cents) {
        Money plus(Money other) {
            return new Money(cents + other.cents);
        }
    }

    public static void main(String[] args) {
        Money a = new Money(150);
        Money b = new Money(250);
        System.out.println(a.plus(b));
        System.out.println(a.plus(b).equals(new Money(400)));
        System.out.println(a.plus(b) == new Money(400));
        System.out.println("total: " + a.plus(b));

        BigInteger big = BigInteger.TWO.pow(70);
        System.out.println(big.add(BigInteger.ONE));
    }
}
```

```output for=methods-and-records
Money[cents=400]
true
false
total: Money[cents=400]
1180591620717411303425
```

- Verdict: absent. Java has no user operators, so a type has methods such as `plus` and `equals`. The standard `BigInteger` and `BigDecimal` work the same way, with `add` and `multiply`.
- `==` on two objects compares references, so two equal records are not `==`. Use `equals`, which the record generates.
- `+` with a `String` operand joins strings and calls `toString` on the other operand.
