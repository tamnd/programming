```java run id=initialization-order
import java.util.Objects;

class Base {
    static { System.out.println("Base: static initializer, once per class"); }

    protected final String id;

    Base(String id) {
        System.out.println("Base constructor");
        this.id = id;
    }
}

class Range extends Base {
    private final int lo, hi;
    private int uses = log("Range: field initializer");  // runs after super(...)

    Range(int lo, int hi) {
        if (lo > hi) throw new IllegalArgumentException("empty range " + lo + ".." + hi);
        super("r" + lo);  // a statement before super: check the arguments first
        this.lo = lo;
        this.hi = hi;
        System.out.println("Range constructor body");
    }

    Range(int hi) { this(0, hi); }  // delegates to another constructor

    static Range parse(String text) {  // a static factory
        String[] parts = Objects.requireNonNull(text).split("\\.\\.");
        return new Range(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
    }

    private static int log(String s) {
        System.out.println(s);
        return 0;
    }

    int size() { return hi - lo + 1; }
}

record Point(int x, int y) {
    Point {  // a compact constructor: checks run before the fields are set
        if (x < 0 || y < 0) throw new IllegalArgumentException("negative coordinate");
    }
}

public class Main {
    public static void main(String[] args) {
        System.out.println(new Range(3, 7).size());
        System.out.println(Range.parse("10..12").size() + " " + new Range(4).id);
        try {
            new Range(9, 1);
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
        System.out.println(new Point(1, 2));
    }
}
```

```output for=initialization-order
Base: static initializer, once per class
Base constructor
Range: field initializer
Range constructor body
5
Base constructor
Range: field initializer
Range constructor body
Base constructor
Range: field initializer
Range constructor body
3 r0
empty range 9..1
Point[x=1, y=2]
```

```java wrong id=overridable-call
class Widget {
    Widget() {
        render();  // an overridable call from a constructor
    }

    void render() { System.out.println("widget"); }
}

class Label extends Widget {
    private final String text;

    Label(String text) {
        super();
        this.text = text;  // too late for the call in Widget()
    }

    @Override
    void render() { System.out.println("label: " + text); }
}

public class Main {
    public static void main(String[] args) {
        new Label("hello");
    }
}
```

```output for=overridable-call
label: null
```

- Verdict: native. A constructor has the name of its class and no return type. The static initializer of a class runs once, when the class is first used. For each object, Java sets every field to its default value, runs the superclass constructor, then the field initializers of the class, then the constructor body. A constructor can delegate to another with `this(...)`. Statements may come before the call to `super(...)` when they do not use `this`, which lets `Range` check its arguments first. A record's compact constructor checks the components before the fields are set.
- The wrong block calls an overridable method from a constructor. Java dispatches the call to `Label.render`, which runs before `Label`'s constructor has set `text`, so it prints the default value `null`, even though `text` is `final`. Make methods called from constructors `private`, `static` or `final`.
