```java run id=nominal-types-and-run-time-checks
import java.util.ArrayList;
import java.util.List;

public class Main {
    interface Shape { double area(); }
    record Square(double side) implements Shape { public double area() { return side * side; } }
    record Box(double side) { double area() { return side * side; } }

    @SuppressWarnings("unchecked")
    static <T> T unsafeCast(Object o) { return (T) o; }

    public static void main(String[] args) {
        // Nominal typing: Box has an area() method but is not a Shape.
        Shape s = new Square(2);
        Object b = new Box(2);
        System.out.println(s.area() + " " + (b instanceof Shape));

        // A cast is checked at run time.
        Object o = "text";
        try {
            Integer i = (Integer) o;
            System.out.println(i);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException");
        }

        // Arrays are covariant, so the check moves to run time.
        Object[] objects = new String[1];
        try {
            objects[0] = 42;
        } catch (ArrayStoreException e) {
            System.out.println("ArrayStoreException: " + e.getMessage());
        }

        // Generic types are erased, so an unchecked cast fails later, at the read.
        List<String> strings = new ArrayList<>();
        List<Integer> ints = unsafeCast(strings);
        ints.add(7);
        try {
            String first = strings.get(0);
            System.out.println(first);
        } catch (ClassCastException e) {
            System.out.println("ClassCastException at the read, size " + strings.size());
        }
    }
}
```

```output for=nominal-types-and-run-time-checks
4.0 false
ClassCastException
ArrayStoreException: java.lang.Integer
ClassCastException at the read, size 1
```

- Verdict: native. Java is static and nominal, so `Box` is not a `Shape` although it has an `area()` method. A cast is checked at run time and throws `ClassCastException`. Arrays are covariant, so storing an `Integer` in a `String[]` through an `Object[]` throws `ArrayStoreException`. Generics are erased, so an unchecked cast lets an `Integer` into a `List<String>`, and the error appears only when the program reads it.
