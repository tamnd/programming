```java run id=arrays-records
import java.util.Arrays;
import java.util.List;

public class Main {
  record Point(int x, int y) {}

  public static void main(String[] args) {
    int[] a = {1, 2, 3};
    int[] b = {1, 2, 3};
    System.out.println((a == b) + " " + Arrays.equals(a, b));

    Point p = new Point(1, 2);
    System.out.println(p + " " + p.equals(new Point(1, 2)) + " " + (p == new Point(1, 2)));

    int[][] grid = new int[2][3];
    grid[0][1] = 7;
    System.out.println(Arrays.deepToString(grid));

    List<Point> points = List.of(p, new Point(3, 4));
    System.out.println(points.get(1).y());

    int[] c = a;
    c[0] = 99;
    System.out.println(a[0]);
  }
}
```

```output for=arrays-records
false true
Point[x=1, y=2] true false
[[0, 7, 0], [0, 0, 0]]
4
99
```

- Verdict: native. Arrays, classes and records are reference types, so `==` compares references and `Arrays.equals` compares the elements. A record gets `equals`, `hashCode` and `toString` from its components, so two points with the same coordinates are equal, but they are two objects. `int[][]` is an array of arrays. `c = a` copies the reference, so a change through `c` shows in `a`.
