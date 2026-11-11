```java run id=dispatch
import java.util.List;

public class Main {
    sealed interface Shape permits Circle, Square {
        double area();

        default String describe() {
            return getClass().getSimpleName() + " " + String.format("%.2f", area());
        }
    }

    record Circle(double r) implements Shape {
        public double area() {
            return Math.PI * r * r;
        }
    }

    record Square(double side) implements Shape {
        public double area() {
            return side * side;
        }

        @Override
        public String describe() {
            return "a square of side " + side;
        }
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Circle(1), new Square(2));
        for (Shape s : shapes) {
            System.out.println(s.describe());
        }
        System.out.println(new Square(2).equals(new Square(2)));
    }
}
```

```output for=dispatch
Circle 3.14
a square of side 2.0
true
```

- Verdict: native. All code in Java is in classes, and instance methods are virtual by default. The method that runs is chosen at run time from the class of the object.
- A class extends one class and implements any number of interfaces. Interfaces can have `default` methods.
- A `sealed` interface lists all its implementations, and a `record` is a class for immutable data with `equals`, `hashCode` and `toString` already written.
