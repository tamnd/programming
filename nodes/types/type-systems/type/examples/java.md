```java run id=primitive-and-reference-types
import java.util.List;

public class Main {
    record Celsius(double degrees) {}

    public static void main(String[] args) {
        // Primitive types hold values directly and have fixed ranges.
        int i = Integer.MAX_VALUE;
        long l = i + 1L;
        char c = 'A';
        System.out.println(i + " " + l + " " + (c + 1) + " " + (char) (c + 1));

        // Each primitive type has a reference type, and boxing converts between them.
        Integer boxed = 127;
        Integer same = 127;
        Integer big = 1000;
        Integer big2 = 1000;
        System.out.println((boxed == same) + " " + (big == big2) + " " + big.equals(big2));

        // The static type of a variable can differ from the class of its object.
        Object o = "hello";
        System.out.println(o.getClass().getSimpleName() + " " + (o instanceof String s ? s.length() : -1));

        // var lets the compiler infer the static type, which then does not change.
        var list = List.of(1, 2, 3);
        var sum = 0L;
        for (int v : list) sum += v;
        Object boxedSum = sum;
        System.out.println(sum + " " + boxedSum.getClass().getSimpleName() + " " + list.get(0).getClass().getSimpleName());

        // A record is a new type with its own name.
        Celsius t = new Celsius(21.5);
        System.out.println(t + " " + t.getClass().getSimpleName() + " " + int.class + " " + int[].class.getSimpleName());
    }
}
```

```output for=primitive-and-reference-types
2147483647 2147483648 66 B
true false true
String 5
6 Long Integer
Celsius[degrees=21.5] Celsius int int[]
```

- Verdict: native. Primitive types have fixed ranges, and `i + 1L` is a `long` that holds `Integer.MAX_VALUE + 1`. `char` is a numeric type, so `c + 1` is `66`. Boxed `Integer` values from -128 to 127 come from a cache, so `==` is true for 127 and false for 1000. A variable of static type `Object` holds a `String`. `var` infers `long` for `sum`, and a record makes a new named type.
