```java run id=move
import java.util.List;

public class Main {
    record Position(int x, int y) {}

    interface Movable {
        Position getPosition();
        void setPosition(Position p);
    }

    static final class MoveServant {
        void moveTo(Movable serviced, Position where) {
            serviced.setPosition(where);
        }

        void moveBy(Movable serviced, int dx, int dy) {
            Position p = serviced.getPosition();
            serviced.setPosition(new Position(p.x() + dx, p.y() + dy));
        }
    }

    static class Shape implements Movable {
        private final String name;
        private Position p;

        Shape(String name, Position p) {
            this.name = name;
            this.p = p;
        }

        public Position getPosition() { return p; }
        public void setPosition(Position p) { this.p = p; }

        @Override
        public String toString() { return name + " at (" + p.x() + ", " + p.y() + ")"; }
    }

    static final class Rectangle extends Shape { Rectangle(Position p) { super("rectangle", p); } }
    static final class Ellipse extends Shape { Ellipse(Position p) { super("ellipse", p); } }
    static final class Triangle extends Shape { Triangle(Position p) { super("triangle", p); } }

    public static void main(String[] args) {
        MoveServant servant = new MoveServant();
        List<Shape> shapes = List.of(
            new Rectangle(new Position(0, 0)), new Ellipse(new Position(10, 0)), new Triangle(new Position(0, 10)));
        for (Shape s : shapes) {
            servant.moveBy(s, 3, 4);
            System.out.println(s);
        }
        servant.moveTo(shapes.get(2), new Position(1, 1));
        System.out.println(shapes.get(2));
    }
}
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. The interface `Movable` states what the servant needs, as in the Wikipedia example.
