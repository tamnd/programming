```java run id=keep
interface HasXY<P extends HasXY<P>> {
    int x();

    int y();

    P withX(int x);
}

record Point(int x, int y) implements HasXY<Point> {
    public Point withX(int x) {
        return new Point(x, y);
    }
}

record Labelled(int x, int y, String label) implements HasXY<Labelled> {
    public Labelled withX(int x) {
        return new Labelled(x, y, label);
    }
}

public class Main {
    static <P extends HasXY<P>> P shift(P p) {
        return p.withX(p.x() + 1);
    }

    public static void main(String[] args) {
        Point a = shift(new Point(1, 2));
        Labelled b = shift(new Labelled(1, 2, "home"));
        System.out.println("point: x=" + a.x() + " y=" + a.y());
        System.out.println("labelled: x=" + b.x() + " y=" + b.y() + " label=" + b.label());
    }
}
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
```

- Verdict: idiom. Java types are nominal, so each record must name the interface `HasXY`. The type parameter `P` lets `shift` return the same record type that it gets, so `b` is a `Labelled`.
- Each record writes its own `withX`, because only the record knows how to copy its other fields.
