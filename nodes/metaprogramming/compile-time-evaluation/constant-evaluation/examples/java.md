```java run id=constants
public class Main {
    static final int SMALL = 1;
    static final int LARGE = SMALL * 100;
    static final String PREFIX = "user-";

    static String size(int n) {
        switch (n) {
            case SMALL:
                return "small";
            case LARGE:
                return "large";
            default:
                return "other";
        }
    }

    public static void main(String[] args) {
        System.out.println(size(1) + " " + size(100) + " " + size(7));
        String id = "user-42";
        String constant = PREFIX + "42";
        String suffix = "42";
        String computed = PREFIX + suffix;
        System.out.println("constant == id: " + (constant == id));
        System.out.println("computed == id: " + (computed == id));
        System.out.println("computed.equals(id): " + computed.equals(id));
    }
}
```

```output for=constants
small large other
constant == id: true
computed == id: false
computed.equals(id): true
```

`SMALL` and `LARGE` are constant variables: `final`, of a primitive type or `String`, and initialized with a constant expression. So they can be `case` labels. `PREFIX + "42"` is a constant expression, and the compiler interns it, so it is the same object as the literal `"user-42"`. `suffix` is not `final`, so `PREFIX + suffix` is computed at run time and gives a different object. Compare strings with `equals`, not `==`.

```java fails expect=compile id=method-call
public class Main {
    static final int LIMIT = Math.max(10, 20);

    public static void main(String[] args) {
        switch (args.length) {
            case LIMIT:
                System.out.println("limit");
                break;
            default:
                System.out.println("other");
        }
    }
}
```

`LIMIT` is `final`, but its initializer calls a method. A method call is never part of a constant expression, so `LIMIT` is not a constant variable and cannot be a `case` label.

```java fails expect=exception:ArithmeticException id=abrupt
public class Main {
    public static void main(String[] args) {
        final int zero = 0;
        final int ratio = 1 / zero;
        System.out.println(ratio);
    }
}
```

`1 / zero` uses only constants, but it would complete abruptly. A constant expression must not complete abruptly, so this one is not constant. The compiler accepts the program, and the division throws at run time.

- Verdict: native. A constant expression has a primitive or `String` value and uses literals, operators, casts and the names of constant variables. It has no method calls and no user code.
- The compiler copies the value of a constant variable into each class that uses it. If a library changes the value, a class that was compiled against the old value keeps the old value until it is compiled again.
