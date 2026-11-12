```java run id=merge-the-twins
import java.util.Map;

public class Main {
    // Before: a shape hierarchy and a renderer hierarchy that mirrors it.
    interface ShapeBefore {}
    record CircleBefore(int r) implements ShapeBefore {}
    record SquareBefore(int side) implements ShapeBefore {}
    record TriangleBefore(int base, int height) implements ShapeBefore {}

    interface Renderer { String svg(ShapeBefore s); }
    static final class CircleRenderer implements Renderer {
        public String svg(ShapeBefore s) { return "<circle r=\"" + ((CircleBefore) s).r() + "\"/>"; }
    }
    static final class SquareRenderer implements Renderer {
        public String svg(ShapeBefore s) {
            int side = ((SquareBefore) s).side();
            return "<rect width=\"" + side + "\" height=\"" + side + "\"/>";
        }
    }

    static final Map<Class<?>, Renderer> RENDERERS =
            Map.of(CircleBefore.class, new CircleRenderer(), SquareBefore.class, new SquareRenderer());

    static String svgBefore(ShapeBefore s) {
        Renderer r = RENDERERS.get(s.getClass());
        if (r == null) throw new IllegalStateException("no renderer for " + s.getClass().getSimpleName());
        return r.svg(s);
    }

    // After: each shape draws itself, and the sealed interface lists the shapes.
    sealed interface Shape permits Circle, Square, Triangle {
        String svg();
    }
    record Circle(int r) implements Shape {
        public String svg() { return "<circle r=\"" + r + "\"/>"; }
    }
    record Square(int side) implements Shape {
        public String svg() { return "<rect width=\"" + side + "\" height=\"" + side + "\"/>"; }
    }
    record Triangle(int base, int height) implements Shape {
        public String svg() {
            return "<polygon points=\"0," + height + " " + base + "," + height + " " + base / 2 + ",0\"/>";
        }
    }

    public static void main(String[] args) {
        if (!new Circle(5).svg().equals(svgBefore(new CircleBefore(5)))) throw new AssertionError("circle");
        if (!new Square(4).svg().equals(svgBefore(new SquareBefore(4)))) throw new AssertionError("square");
        try {
            svgBefore(new TriangleBefore(6, 3));
            throw new AssertionError("the missing twin must fail");
        } catch (IllegalStateException expected) {
        }
        if (!new Triangle(6, 3).svg().equals("<polygon points=\"0,3 6,3 3,0\"/>")) throw new AssertionError("triangle");
        System.out.println("svg: tests pass");
    }
}
```

```output
svg: tests pass
```

- Each renderer in the first version must cast its argument to the shape it draws. The casts are a second sign that the two hierarchies belong together.
- `Shape` is `sealed` and declares `svg`, so every permitted shape must implement it, and the compiler reports a missing one.
- `base / 2` is integer division in Java, which is correct here because the base is even.
