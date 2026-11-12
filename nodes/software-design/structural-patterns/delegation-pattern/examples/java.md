```java run id=window-shape
interface Titled {
    String title();
}

interface Shape {
    int area();
    String describe(Titled owner);
}

record Rectangle(int width, int height) implements Shape {
    public int area() { return width * height; }
    public String describe(Titled owner) {
        return owner.title() + ": a rectangle of " + width + " by " + height;
    }
}

record Triangle(int base, int height) implements Shape {
    public int area() { return base * height / 2; }
    public String describe(Titled owner) {
        return owner.title() + ": a triangle of " + base + " by " + height;
    }
}

class AppWindow implements Titled {
    private final String title;
    private Shape shape; // the delegate

    AppWindow(String title, Shape shape) {
        this.title = title;
        this.shape = shape;
    }

    public String title() { return title; }
    void setShape(Shape shape) { this.shape = shape; }
    int area() { return shape.area(); } // forward
    String describe() { return shape.describe(this); } // delegate, and pass the receiver
}

public class Main {
    public static void main(String[] args) {
        AppWindow w = new AppWindow("main", new Rectangle(3, 4));
        System.out.println("area: " + w.area());
        System.out.println(w.describe());
        w.setShape(new Triangle(3, 4)); // change the behaviour at run time
        System.out.println("area: " + w.area());
        System.out.println(w.describe());
    }
}
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: idiom. Java has no forwarding keyword, so each delegated method is written by hand.
