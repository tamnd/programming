```java run id=shapes
import java.util.List;

public class Main {
    sealed interface Shape permits Circle, Square {
        double area();

        default String describe() { // calls area() on the receiver, so each class decides
            return "%s with area %.2f".formatted(getClass().getSimpleName(), area());
        }
    }

    record Circle(double r) implements Shape {
        public double area() { return Math.PI * r * r; }
    }

    record Square(double side) implements Shape {
        public double area() { return side * side; }
    }

    // Overloads: the compiler picks one from the static type of the argument.
    static String kind(Shape s) { return "some shape"; }
    static String kind(Circle c) { return "a circle"; }

    public static void main(String[] args) {
        for (Shape s : List.of(new Circle(1), new Square(2)))
            System.out.println(s.describe());

        Shape s = new Circle(1);
        Circle c = new Circle(1);
        System.out.println(kind(s) + ", " + kind(c)); // same object, different overloads
    }
}
```

```output for=shapes
Circle with area 3.14
Square with area 4.00
some shape, a circle
```

```java wrong id=equals-overload
import java.util.List;

public class Main {
    static final class Point {
        final int x, y;
        Point(int x, int y) { this.x = x; this.y = y; }

        public boolean equals(Point p) { // overloads equals(Object); overrides nothing
            return p != null && x == p.x && y == p.y;
        }

        @Override
        public int hashCode() { return 31 * x + y; }
    }

    public static void main(String[] args) {
        Point a = new Point(1, 2), b = new Point(1, 2);
        System.out.println(a.equals(b));                // static type Point: the overload
        System.out.println(List.of(a).contains(b));     // calls equals(Object): identity
    }
}
```

```output for=equals-overload
true
false
```

- Verdict: native. Every instance method in Java is virtual unless it is `static`, `private` or `final`, and interface methods, including default methods such as `describe`, dispatch on the class of the receiver. Overloads are different: the compiler chooses among `kind(Shape)` and `kind(Circle)` from the static type of the argument, so the same circle gives two answers.
- The wrong block declares `equals(Point)`, which overloads `equals(Object)` instead of overriding it. The direct call uses the overload, but `List.contains` calls `equals(Object)`, which still compares identity. An `@Override` annotation on the method makes the compiler reject it, because it overrides nothing.
