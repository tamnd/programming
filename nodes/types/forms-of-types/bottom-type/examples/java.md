```java run id=generic-fail
public class Main {
  // Java has no bottom type, so a method that always throws uses a type variable as its result.
  static <T> T fail(String msg) {
    throw new IllegalArgumentException(msg);
  }

  static String day(int d) {
    return switch (d) {
      case 1 -> "Mon";
      case 2 -> "Tue";
      default -> throw new IllegalArgumentException("bad day " + d);
    };
  }

  public static void main(String[] args) {
    try {
      String host = args.length > 0 ? args[0] : fail("no host");
      System.out.println(host);
    } catch (IllegalArgumentException e) {
      System.out.println(e.getMessage());
    }

    System.out.println(day(1));
    try {
      day(9);
    } catch (IllegalArgumentException e) {
      System.out.println(e.getMessage());
    }

    // null fits every reference type, but not int.
    String s = null;
    Integer i = null;
    System.out.println(s + " " + i);
  }
}
```

```output for=generic-fail
no host
Mon
bad day 9
null null
```

- Verdict: absent. Java has no bottom type. The generic method `fail` returns `T`, so the compiler infers `String` for the conditional expression, but the method never returns. A `switch` expression allows `throw` in an arm, and the other arms give the value. `null` can be assigned to any reference type, but the null type is not a subtype of `int`.
