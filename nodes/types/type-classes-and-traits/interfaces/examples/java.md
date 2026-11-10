```java run id=interfaces
import java.util.List;

public class Main {
    interface Shape {
        double area();

        default String describe() {
            return String.format("%s %.2f", getClass().getSimpleName(), area());
        }

        static Shape unit() {
            return new Square(1);
        }
    }

    record Square(double side) implements Shape {
        public double area() {
            return side * side;
        }
    }

    record Circle(double radius) implements Shape {
        public double area() {
            return Math.PI * radius * radius;
        }

        @Override
        public String describe() {
            return "circle of radius " + radius;
        }
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Square(2), new Circle(1), Shape.unit());
        for (Shape shape : shapes) {
            System.out.println(shape.describe());
        }
        System.out.println(Shape.class.isInterface());
        System.out.println(List.of(Square.class.getInterfaces()));
    }
}
```

```output
Square 4.00
circle of radius 1.0
Square 1.00
true
[interface Main$Shape]
```

- Verdict: native. A class or record names its interfaces in an `implements` clause. The relation is nominal: a class with the right methods and no `implements` clause is not a `Shape`.
- `describe` is a default method. `Square` inherits it, and `Circle` overrides it. Default methods came with Java SE 8.
- `unit` is a static method of the interface. A caller writes `Shape.unit()`.
- The call `shape.describe()` compiles to the JVM instruction `invokeinterface`. The JVM selects the method from the class of the object at run time.
