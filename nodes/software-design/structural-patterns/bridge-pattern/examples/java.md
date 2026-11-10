```java run id=shapes
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    record Point(int x, int y) {}

    // The implementor: the primitive drawing operations of one output format.
    interface Renderer {
        String name();
        String circle(int x, int y, int radius);
        String polygon(List<Point> points);
    }

    static final class TextRenderer implements Renderer {
        public String name() { return "text"; }

        public String circle(int x, int y, int radius) {
            return "circle at (" + x + ", " + y + ") radius " + radius;
        }

        public String polygon(List<Point> points) {
            return "polygon " + points.stream()
                .map(p -> "(" + p.x() + ", " + p.y() + ")")
                .collect(Collectors.joining(" "));
        }
    }

    static final class SvgRenderer implements Renderer {
        public String name() { return "svg"; }

        public String circle(int x, int y, int radius) {
            return "<circle cx=\"" + x + "\" cy=\"" + y + "\" r=\"" + radius + "\"/>";
        }

        public String polygon(List<Point> points) {
            return "<polygon points=\"" + points.stream()
                .map(p -> p.x() + "," + p.y())
                .collect(Collectors.joining(" ")) + "\"/>";
        }
    }

    // The abstraction: it holds a renderer and draws itself with the primitives.
    abstract static class Shape {
        protected Renderer renderer;

        Shape(Renderer renderer) { this.renderer = renderer; }

        void setRenderer(Renderer renderer) { this.renderer = renderer; }

        abstract String draw();
        abstract void scale(int factor);
    }

    static final class Circle extends Shape {
        private final int x, y;
        private int radius;

        Circle(Renderer renderer, int x, int y, int radius) {
            super(renderer);
            this.x = x;
            this.y = y;
            this.radius = radius;
        }

        String draw() { return renderer.circle(x, y, radius); }

        void scale(int factor) { radius *= factor; }
    }

    static final class Square extends Shape {
        private final int x, y;
        private int side;

        Square(Renderer renderer, int x, int y, int side) {
            super(renderer);
            this.x = x;
            this.y = y;
            this.side = side;
        }

        String draw() {
            return renderer.polygon(List.of(
                new Point(x, y), new Point(x + side, y),
                new Point(x + side, y + side), new Point(x, y + side)));
        }

        void scale(int factor) { side *= factor; }
    }

    public static void main(String[] args) {
        for (Renderer renderer : List.of(new TextRenderer(), new SvgRenderer())) {
            System.out.println(renderer.name() + ":");
            for (Shape shape : List.of(new Circle(renderer, 1, 1, 2), new Square(renderer, 0, 0, 3))) {
                System.out.println("  " + shape.draw());
                shape.scale(2);
                System.out.println("  " + shape.draw());
            }
        }

        Shape circle = new Circle(new TextRenderer(), 1, 1, 2);
        circle.setRenderer(new SvgRenderer()); // the implementation changes at run time
        System.out.println("switched: " + circle.draw());
    }
}
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. An `interface` for the implementor, an `abstract class` for the abstraction, and a `protected` field that joins them.
- The abstraction is a class hierarchy of its own, so it is an abstract class and not an interface: it holds state, the renderer, that all shapes share.
- JDBC has a related split. `java.sql.Driver` is the interface that each database driver implements, and application code calls the `java.sql` interfaces, not the driver classes.
