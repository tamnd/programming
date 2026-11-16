```java run id=method-java
public class Main {
    static class Rectangle {
        final int width;
        final int height;

        Rectangle(int width, int height) {
            this.width = width;
            this.height = height;
        }

        int area() {
            return width * height;
        }

        static Rectangle square(int side) {
            return new Rectangle(side, side);
        }
    }

    public static void main(String[] args) {
        Rectangle r = new Rectangle(3, 4);
        System.out.println(r.area());
        System.out.println(Rectangle.square(5).area());
    }
}
```

```output for=method-java
12
25
```

- Verdict: native. All Java functions are methods. `area` is an instance method, and `square` is a `static` method with no receiver.
