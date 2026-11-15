```java run id=method-references
import java.util.function.Function;
import java.util.function.Supplier;

public class Main {
  static final class Rectangle {
    private int width;
    private int height;

    Rectangle(int width, int height) {
      this.width = width;
      this.height = height;
    }

    int area() {
      return this.width * this.height; // this is the receiver
    }

    void scale(int factor) {
      width *= factor; // this. is implied
      height *= factor;
    }

    static int area(int width, int height) { // a static method: no receiver
      return width * height;
    }
  }

  public static void main(String[] args) {
    Rectangle r = new Rectangle(3, 4);
    Supplier<Integer> bound = r::area;                 // the receiver is fixed
    Function<Rectangle, Integer> unbound = Rectangle::area; // the receiver is the argument
    r.scale(2);
    System.out.println(r.area() + " " + bound.get() + " " + unbound.apply(r));
    System.out.println(Rectangle.area(6, 8));
  }
}
```

```output for=method-references
48 48 48
48
```

- Verdict: native. Every method is declared in a class. An instance method has the hidden receiver `this`, and a `static` method has none. The JLS calls a static method a class method, which is invoked relative to the class, not to an object.
- A method reference `r::area` binds the receiver, and `Rectangle::area` takes the receiver as the first argument of the functional interface. The compiler picks the instance method or the static method of the same name by the target type.
