```java run id=money
import java.math.BigDecimal;

public class Main {
  public static void main(String[] args) {
    double total = 0.0;
    for (int i = 0; i < 10; i++) {
      total += 0.1;
    }
    System.out.println("loop: " + total);
    System.out.println("new BigDecimal(0.1): " + new BigDecimal(0.1));
    System.out.println("BigDecimal.valueOf(0.1): " + BigDecimal.valueOf(0.1));

    BigDecimal exact = BigDecimal.ZERO;
    BigDecimal tenth = new BigDecimal("0.1");
    for (int i = 0; i < 10; i++) {
      exact = exact.add(tenth);
    }
    System.out.println("BigDecimal sum: " + exact);
    System.out.println("equals 1.0: " + (exact.compareTo(BigDecimal.ONE) == 0 ? "yes" : "no"));
    System.out.println("float 0.1f as double: " + (double) 0.1f);
  }
}
```

```output for=money
loop: 0.9999999999999999
new BigDecimal(0.1): 0.1000000000000000055511151231257827021181583404541015625
BigDecimal.valueOf(0.1): 0.1
BigDecimal sum: 1.0
equals 1.0: yes
float 0.1f as double: 0.10000000149011612
```

- Verdict: native. Java `double` is binary64 and `float` is binary32.
- `new BigDecimal(0.1)` shows the exact stored value. `BigDecimal.valueOf(0.1)` goes through `Double.toString`, so it gives `0.1`.
- A `float` has a larger representation error than a `double`, as the last line shows.
