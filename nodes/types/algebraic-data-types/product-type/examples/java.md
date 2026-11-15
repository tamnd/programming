```java run id=parts
public class Main {
    record Point(int x, int y) {}

    record Unit() {}

    static String yn(boolean b) {
        return b ? "yes" : "no";
    }

    public static void main(String[] args) {
        Point p = new Point(3, 4);
        System.out.println("record: " + p + " x = " + p.x() + " y = " + p.y());
        System.out.println("same parts equal: " + yn(p.equals(new Point(3, 4))));
        System.out.println("swapped parts equal: " + yn(p.equals(new Point(4, 3))));
        System.out.println("same object (==): " + yn(p == new Point(3, 4)));
        System.out.println("unit: " + new Unit() + " equal to unit: " + yn(new Unit().equals(new Unit())));
    }
}
```

```output for=parts
record: Point[x=3, y=4] x = 3 y = 4
same parts equal: yes
swapped parts equal: no
same object (==): no
unit: Unit[] equal to unit: yes
```

- Verdict: native. A `record` is a product by name. Java has no tuple type.
- `equals` compares the components, but `==` compares references.
- A record with no components has one value up to `equals`, so it can act as a unit.
