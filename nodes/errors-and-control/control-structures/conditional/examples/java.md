```java run id=instanceof-patterns-and-ternary-types
public class Main {
    static String describe(Object o) {
        // instanceof with a pattern tests the type and binds a variable.
        if (o instanceof String s && !s.isEmpty()) {
            return "string of " + s.length();
        } else if (o instanceof Integer i) {
            return i > 0 ? "positive int" : "other int";
        }
        return "something else";
    }

    public static void main(String[] args) {
        System.out.println(describe("abc") + ", " + describe(5) + ", " + describe(""));

        // The two operands of ?: are converted to one type, so 1 prints as 1.0.
        boolean flag = true;
        System.out.println(flag ? 1 : 2.0);

        // With Integer and int operands, ?: unboxes, so a null Integer throws.
        Integer missing = null;
        try {
            int v = flag ? missing : 0;
            System.out.println(v);
        } catch (NullPointerException e) {
            System.out.println("NullPointerException from unboxing");
        }

        // The else belongs to the inner if, whatever the indentation says,
        // so r stays "none" when a is negative.
        int a = -5, b = 1;
        String r = "none";
        if (a > 0)
            if (b > 0)
                r = "both positive";
        else
            r = "a not positive";
        System.out.println(r);
    }
}
```

```output for=instanceof-patterns-and-ternary-types
string of 3, positive int, something else
1.0
NullPointerException from unboxing
none
```

- Verdict: native. `instanceof` with a pattern tests the type and binds `s` or `i`, and `&&` adds a second condition. `flag ? 1 : 2.0` prints `1.0`, because both operands become `double`. With a `null` `Integer` and an `int` operand, `?:` unboxes and throws. The `else` belongs to the inner `if`, so `r` stays `"none"`, although the indentation suggests otherwise.
