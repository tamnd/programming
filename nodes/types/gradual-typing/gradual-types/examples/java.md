```java run id=checked-casts
import java.util.List;

public class Main {
    public static void main(String[] args) {
        Object value = "42";             // the static type forgets the class
        if (value instanceof String s) { // a checked test, then a typed name
            System.out.println("a String of length " + s.length());
        }
        try {
            Integer n = (Integer) value; // a cast is checked when it runs
            System.out.println(n + 1);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException at the cast");
        }
        List<Object> mixed = List.of(1, "two", 3.0);
        for (Object o : mixed) {
            String kind = switch (o) {
                case Integer i -> "Integer " + (i + 1);
                case String t -> "String " + t.toUpperCase();
                default -> "other " + o.getClass().getSimpleName();
            };
            System.out.println(kind);
        }
    }
}
```

```output for=checked-casts
a String of length 2
ClassCastException at the cast
Integer 2
String TWO
other Double
```

```java wrong id=raw-type-pollution
import java.util.ArrayList;
import java.util.List;

public class Main {
    @SuppressWarnings({"rawtypes", "unchecked"})
    static void addLegacy(List list) { // a raw type: no type arguments
        list.add("not a number");      // the compiler cannot check this
    }

    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2));
        addLegacy(numbers);
        System.out.println("size " + numbers.size());
        try {
            int sum = 0;
            for (Integer n : numbers) sum += n; // the hidden cast fails here
            System.out.println(sum);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException far from the cause");
        }
    }
}
```

```output for=raw-type-pollution
size 3
ClassCastException far from the cause
```

- Verdict: idiom. Java is statically typed, with `Object` as the type that every class fits and casts, `instanceof` and pattern matching that check the class at run time. The run block shows the checked forms: a failed cast throws at the cast, and a `switch` with type patterns names each case.
- The wrong block passes a `List<Integer>` to a method that takes a raw `List`, the type of code written before generics. The compiler cannot check the raw call, so a string enters a list of integers, and the error appears later, at the hidden cast in the loop. The JLS calls this heap pollution. Fix the legacy method to take `List<Integer>`, and treat unchecked warnings as errors.
