```java run id=points
public class Main {
    record Point(int x, int y) {}

    static String describe(Point p) {
        return switch (p) {
            case Point(int x, int y) when x == 0 && y == 0 -> "origin";
            case Point(int x, int y) when y == 0 -> "on the x axis at " + x;
            case Point(int x, int y) when x == 0 -> "on the y axis at " + y;
            case Point(int x, int y) when x == y -> "on the diagonal at " + x;
            case Point(int x, int y) -> "at " + x + ", " + y;
        };
    }

    public static void main(String[] args) {
        Point[] points = {
            new Point(0, 0), new Point(3, 0), new Point(0, -2), new Point(4, 4), new Point(1, 5)
        };
        for (Point p : points) {
            System.out.println(p + ": " + describe(p));
        }

        Object o = "hello";
        if (o instanceof String s && s.length() > 3) {
            System.out.println("a long string: " + s);
        }
    }
}
```

```output for=points
Point[x=0, y=0]: origin
Point[x=3, y=0]: on the x axis at 3
Point[x=0, y=-2]: on the y axis at -2
Point[x=4, y=4]: on the diagonal at 4
Point[x=1, y=5]: at 1, 5
a long string: hello
```

- Verdict: native. Java 21 has type patterns and record patterns in `switch` and in `instanceof`, and a guard after `when`.
- A record pattern holds only type patterns, record patterns and `_`. It cannot hold a constant, such as `Point(0, 0)`, so this example tests the zeros in guards.
- The last case has no guard. Guarded cases do not count when the compiler checks that the `switch` covers its type, so without that case the `switch` does not compile.
