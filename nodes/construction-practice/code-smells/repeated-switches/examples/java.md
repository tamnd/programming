```java run id=switch-to-classes
import java.util.List;

public class Main {
    record Raw(String kind, double a, double b) {}

    // Before: each method switches on the kind.
    static double areaBefore(Raw s) {
        switch (s.kind()) {
            case "circle": return Math.PI * s.a() * s.a();
            case "square": return s.a() * s.a();
            case "triangle": return s.a() * s.b() / 2;
            default: return 0;
        }
    }

    static double perimeterBefore(Raw s) {
        switch (s.kind()) {
            case "circle": return 2 * Math.PI * s.a();
            case "square": return 4 * s.a();
            default: return 0; // the triangle case was never added
        }
    }

    // After: a sealed interface, and a record for each kind.
    sealed interface Shape permits Circle, Square, Triangle {
        double area();
        double perimeter();
    }

    record Circle(double r) implements Shape {
        public double area() { return Math.PI * r * r; }
        public double perimeter() { return 2 * Math.PI * r; }
    }

    record Square(double side) implements Shape {
        public double area() { return side * side; }
        public double perimeter() { return 4 * side; }
    }

    record Triangle(double a, double b, double c) implements Shape {
        public double area() {
            double s = (a + b + c) / 2;
            return Math.sqrt(s * (s - a) * (s - b) * (s - c));
        }
        public double perimeter() { return a + b + c; }
    }

    // A switch over the sealed type must cover each case; it needs no default.
    static String name(Shape s) {
        return switch (s) {
            case Circle c -> "circle";
            case Square q -> "square";
            case Triangle t -> "triangle";
        };
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        Raw t = new Raw("triangle", 3, 4);
        check(areaBefore(t) == 6, "before area");
        check(perimeterBefore(t) == 0, "before perimeter");
        System.out.println("before: the perimeter of a triangle is 0");

        List<Shape> shapes = List.of(new Circle(1), new Square(2), new Triangle(3, 4, 5));
        check(shapes.get(0).perimeter() == 2 * Math.PI, "circle");
        check(shapes.get(1).perimeter() == 8 && shapes.get(1).area() == 4, "square");
        check(shapes.get(2).perimeter() == 12 && shapes.get(2).area() == 6, "triangle");
        check(name(shapes.get(2)).equals("triangle"), "name");
        System.out.println("after: tests pass");
    }
}
```

```output
before: the perimeter of a triangle is 0
after: tests pass
```

- A record that implements `Shape` and leaves out `perimeter` does not compile, so a kind that is added cannot be half done.
- The `switch` in `name` has no `default`. If a fourth record is added to the `permits` list, this switch stops compiling until it has a case for it.
