```java run id=truncated
public class Main {
  public static void main(String[] args) {
    System.out.println("-7 / 3: " + -7 / 3);
    System.out.println("-7 % 3: " + -7 % 3);
    System.out.println("Math.floorDiv(-7, 3): " + Math.floorDiv(-7, 3));
    System.out.println("Math.floorMod(-7, 3): " + Math.floorMod(-7, 3));
    System.out.println("-7 % 2 == 1: " + (-7 % 2 == 1 ? "yes" : "no"));
    System.out.println("Math.floorMod(-7, 2) == 1: " + (Math.floorMod(-7, 2) == 1 ? "yes" : "no"));
    try {
      int zero = 0;
      System.out.println(1 / zero);
    } catch (ArithmeticException e) {
      System.out.println("1 / 0: " + e.getClass().getSimpleName());
    }
  }
}
```

```output for=truncated
-7 / 3: -2
-7 % 3: -1
Math.floorDiv(-7, 3): -3
Math.floorMod(-7, 3): 2
-7 % 2 == 1: no
Math.floorMod(-7, 2) == 1: yes
1 / 0: ArithmeticException
```

- Verdict: native. `/` and `%` truncate toward zero, and `Math.floorDiv` and `Math.floorMod` give the floored results.
- Integer division by zero throws `ArithmeticException`.
