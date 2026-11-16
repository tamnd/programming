```java run id=record-patterns
public class Main {
    record Point(int x, int y) {}
    record Line(Point from, Point to) {}

    sealed interface Shape permits Circle, Square {}
    record Circle(double r) implements Shape {}
    record Square(double side) implements Shape {}

    static double area(Shape s) {
        return switch (s) {
            case Circle(double r) -> Math.PI * r * r;
            case Square(double side) -> side * side;
        };
    }

    public static void main(String[] args) {
        Object o = new Point(3, 4);
        if (o instanceof Point(int x, int y)) {
            System.out.println(x + y);
        }

        Line line = new Line(new Point(0, 0), new Point(2, 5));
        if (line instanceof Line(Point(var x1, var y1), Point(var x2, var y2))) {
            System.out.println((x2 - x1) + " " + (y2 - y1));
        }

        System.out.println(area(new Square(3)));
    }
}
```

```output for=record-patterns
7
2 5
9.0
```

- Verdict: native. A record pattern takes a record apart into its components, in `instanceof` and in `switch`. Patterns can nest, and `var` takes the type of the component. Java has no destructuring of arrays or of other classes.
