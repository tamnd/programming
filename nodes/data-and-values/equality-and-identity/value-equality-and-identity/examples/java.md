```java run id=points
import java.util.HashSet;
import java.util.List;
import java.util.Objects;

public class Main {
    record Point(int x, int y) {}

    static final class Box {
        final int x;
        final int y;

        Box(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    public static void main(String[] args) {
        Point a = new Point(1, 2);
        Point b = new Point(1, 2);
        Point c = a;
        System.out.println("a.equals(b): " + a.equals(b));
        System.out.println("a == b: " + (a == b));
        System.out.println("a == c: " + (a == c));
        System.out.println("distinct points: " + new HashSet<>(List.of(a, b, c)).size());

        Box p = new Box(1, 2);
        Box q = new Box(1, 2);
        System.out.println("Box p.equals(q): " + p.equals(q));

        String s = "hello";
        String t = new StringBuilder("hel").append("lo").toString();
        System.out.println("s.equals(t): " + s.equals(t));
        System.out.println("Objects.equals(null, null): " + Objects.equals(null, null));

        double nan = Double.NaN;
        System.out.println("nan == nan: " + (nan == nan));
        System.out.println("Double.valueOf(nan).equals(nan): " + Double.valueOf(nan).equals(nan));
        System.out.println("0.0 == -0.0: " + (0.0 == -0.0));
        System.out.println("Double.valueOf(0.0).equals(-0.0): " + Double.valueOf(0.0).equals(-0.0));
    }
}
```

```output for=points
a.equals(b): true
a == b: false
a == c: true
distinct points: 1
Box p.equals(q): false
s.equals(t): true
Objects.equals(null, null): true
nan == nan: false
Double.valueOf(nan).equals(nan): true
0.0 == -0.0: true
Double.valueOf(0.0).equals(-0.0): false
```

- Verdict: native. On references, `==` compares identity. `equals` compares values when the class overrides it. A record overrides `equals` and `hashCode` from its components.
- `Object.equals` is identity, so `Box` compares by identity until it overrides `equals`.
- `Double.equals` differs from `==`: it treats `NaN` as equal to `NaN` and `0.0` as different from `-0.0`, so a `Double` works as a `HashSet` element.
- `Objects.equals` handles `null` on either side.

```java wrong id=string-identity
public class Main {
    public static void main(String[] args) {
        String s = "hello";
        String t = new StringBuilder("hel").append("lo").toString();
        System.out.println(s == t);
        System.out.println(s.equals(t));
    }
}
```

```output for=string-identity
false
true
```

`t` is a new `String` object with the same characters. `==` compares the references, so it is false.

```java wrong id=boxed
public class Main {
    public static void main(String[] args) {
        Integer small1 = Integer.valueOf(127);
        Integer small2 = Integer.valueOf(127);
        Integer large1 = Integer.valueOf(128);
        Integer large2 = Integer.valueOf(128);
        System.out.println(small1 == small2);
        System.out.println(large1 == large2);
        System.out.println(large1.equals(large2));
    }
}
```

```output for=boxed
true
false
true
```

`Integer.valueOf` always caches the values from -128 to 127, so the two `127` objects are the same object. The `128` objects are two objects on this JVM. The result for values outside the cache is not fixed, and a JVM flag can make the cache larger. Compare boxed numbers with `equals`, or unbox them first.

```java wrong id=no-hashcode
import java.util.HashSet;
import java.util.Set;

public class Main {
    static final class Point {
        final int x;
        final int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Point p && p.x == x && p.y == y;
        }
    }

    public static void main(String[] args) {
        Set<Point> seen = new HashSet<>();
        seen.add(new Point(1, 2));
        seen.add(new Point(1, 2));
        System.out.println(new Point(1, 2).equals(new Point(1, 2)));
        System.out.println(seen.size());
        System.out.println(seen.contains(new Point(1, 2)));
    }
}
```

```output for=no-hashcode
true
2
false
```

`equals` says the points are equal, but `hashCode` is still the one from `Object`. As far as is reasonably practical, that method returns distinct integers for distinct objects. `HashSet` looks in the bucket of the hash code first, so it never calls `equals` on the other point. The fix is a `hashCode` that uses the same fields, such as `Objects.hash(x, y)`, or a record.

```java wrong id=mutated-key
import java.util.HashSet;
import java.util.Set;

public class Main {
    static final class Point {
        int x;
        int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Point p && p.x == x && p.y == y;
        }

        @Override
        public int hashCode() {
            return 31 * x + y;
        }
    }

    public static void main(String[] args) {
        Set<Point> seen = new HashSet<>();
        Point p = new Point(1, 2);
        seen.add(p);
        p.x = 5;
        System.out.println(seen.contains(p));
        System.out.println(seen.size());
        for (Point q : seen) {
            System.out.println(q == p);
        }
    }
}
```

```output for=mutated-key
false
1
true
```

The set stored `p` under the hash code of `(1, 2)`. After the change, `contains` looks under the hash code of `(5, 2)` and does not find it, although the set holds the same object. Do not change the fields that `equals` and `hashCode` use while the object is in a hash set or is a map key. Immutable keys, such as records with final fields, avoid the problem.
