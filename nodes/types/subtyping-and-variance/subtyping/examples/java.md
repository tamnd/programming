```java run id=hierarchy
import java.util.List;

public class Main {
    interface Shape {
        double area();

        Shape scaled(double factor);
    }

    record Square(double side) implements Shape {
        public double area() {
            return side * side;
        }

        public Square scaled(double factor) {
            return new Square(side * factor);
        }
    }

    record Circle(double radius) implements Shape {
        public double area() {
            return Math.PI * radius * radius;
        }

        public Circle scaled(double factor) {
            return new Circle(radius * factor);
        }
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Square(2), new Circle(1));
        for (Shape shape : shapes) {
            System.out.printf("%s %.2f%n", shape.getClass().getSimpleName(), shape.area());
        }
        Square big = new Square(1).scaled(3);
        System.out.println(big.side());
        Object any = big;
        System.out.println(any instanceof Shape);
        if (any instanceof Square square) {
            System.out.println(square.area());
        }
    }
}
```

```output for=hierarchy
Square 4.00
Circle 3.14
3.0
true
9.0
```

- Verdict: native. Subtyping is nominal. A class is a subtype of its superclass and of the interfaces in its `implements` clause.
- `Square.scaled` overrides `Shape.scaled` with the return type `Square`. This is a covariant return type, so `new Square(1).scaled(3)` needs no cast.
- `instanceof` with a pattern tests the dynamic type and binds a variable of the subtype.
- Arrays are covariant and generic types are invariant. The concept page shows the `ArrayStoreException` that covariant arrays allow.
