```java run id=shapes
public class Main {
    sealed interface Shape permits Circle, Rect, Triangle {}
    record Circle(double radius) implements Shape {}
    record Rect(double width, double height) implements Shape {}
    record Triangle(double base, double height) implements Shape {}

    static double area(Shape shape) {
        return switch (shape) {
            case Circle(double r) -> Math.PI * r * r;
            case Rect(double w, double h) -> w * h;
            case Triangle(double b, double h) -> b * h / 2;
        };
    }

    public static void main(String[] args) {
        Shape[] shapes = { new Circle(1), new Rect(2, 3), new Triangle(4, 5) };
        for (Shape shape : shapes) {
            System.out.printf("%.2f%n", area(shape));
        }
        System.out.println(shapes[1]);
        System.out.println(Shape.class.isSealed());
        for (Class<?> c : Shape.class.getPermittedSubclasses()) {
            System.out.println(c.getSimpleName());
        }
    }
}
```

```output for=shapes
3.14
6.00
10.00
Rect[width=2.0, height=3.0]
true
Circle
Rect
Triangle
```

- Verdict: native. A `sealed` interface names its permitted subclasses, and each record holds the fields of one case.
- A `switch` over a sealed type needs no `default` when its cases cover every permitted subclass. A missing case is a compile error (see Pitfalls on the concept page).
- Record patterns, such as `Circle(double r)`, take the record apart in the `case` label.
- `isSealed` and `getPermittedSubclasses` show the closed set to reflection.
