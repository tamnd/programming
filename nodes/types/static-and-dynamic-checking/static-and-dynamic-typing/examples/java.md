```java run id=types
import java.util.List;

public class Main {
    static String describe(Object value) {
        if (value instanceof Integer n) {
            return "Integer " + (n + 1);
        }
        if (value instanceof String s) {
            return "String of length " + s.length();
        }
        return value.getClass().getSimpleName();
    }

    public static void main(String[] args) {
        var count = 41;
        Object boxed = count;
        System.out.println(boxed.getClass().getName());
        for (Object value : List.of(count, "abc", 2.5)) {
            System.out.println(describe(value));
        }
    }
}
```

```output
java.lang.Integer
Integer 42
String of length 3
Double
```

- Verdict: native. Java is statically typed. Every variable and expression has a type at compile time, and every object also has a class at run time.
- `var` asks the compiler to infer the type of a local variable. Here `count` is an `int`, and its type is still fixed.
- The variable `boxed` has the static type `Object`. Its value is an `Integer`, which `getClass()` shows.
- `instanceof` with a pattern tests the class and binds a variable of the narrower type. A plain cast throws `ClassCastException` when the class does not fit.
