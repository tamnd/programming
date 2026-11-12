```java run id=no-int-conversion
public class Main {
    static int calls = 0;

    static boolean check(boolean v) {
        calls++;
        return v;
    }

    public static void main(String[] args) {
        // boolean is its own type: no conversion from int, so the idiom is x != 0.
        int x = 5;
        boolean nonZero = x != 0;
        System.out.println(nonZero + " " + (x > 3 && x < 10) + " " + !nonZero);

        // && and || skip the right operand; & and | evaluate both.
        calls = 0;
        boolean a = check(false) && check(true);
        int shortCircuit = calls;
        calls = 0;
        boolean b = check(false) & check(true);
        System.out.println(a + " " + shortCircuit + " " + b + " " + calls);

        // ^ is exclusive or on booleans.
        System.out.println((true ^ true) + " " + (true ^ false));

        // Parsing accepts only "true", in any case; everything else is false.
        System.out.println(Boolean.parseBoolean("TRUE") + " " + Boolean.parseBoolean("yes") + " " + Boolean.parseBoolean("1"));

        // A Boolean object can be null, and unboxing null throws.
        Boolean maybe = null;
        try {
            if (maybe) System.out.println("unreachable");
        } catch (NullPointerException e) {
            System.out.println("NullPointerException");
        }

        // String concatenation and conversion.
        System.out.println("flag=" + true + " " + Boolean.toString(false) + " " + Boolean.compare(true, false));
    }
}
```

```output for=no-int-conversion
true true false
false 1 false 2
false true
true false false
NullPointerException
flag=true false 1
```

- Verdict: native. There is no conversion from `int` to `boolean`, so the idiom is `x != 0`. `&&` evaluates one operand of `check(false) && check(true)`, and `&` evaluates both. `^` is exclusive or. `Boolean.parseBoolean` accepts `"true"` in any case and gives `false` for `"yes"` and `"1"`. Unboxing a `null` `Boolean` in an `if` throws `NullPointerException`.
