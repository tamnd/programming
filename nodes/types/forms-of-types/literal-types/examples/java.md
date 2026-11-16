```java run id=enum-switch
public class Main {
    enum Align { LEFT, RIGHT, CENTER }

    static String pad(String text, int width, Align align) {
        int space = width - text.length();
        // A switch expression on an enum must cover every constant.
        return switch (align) {
            case LEFT -> text + ".".repeat(space);
            case RIGHT -> ".".repeat(space) + text;
            case CENTER -> ".".repeat(space / 2) + text + ".".repeat(space - space / 2);
        };
    }

    public static void main(String[] args) {
        for (Align a : Align.values()) {
            System.out.print(pad("hi", 6, a) + " ");
        }
        System.out.println();
        System.out.println(Align.valueOf("CENTER").ordinal());
        try {
            Align.valueOf("CENTRE");
        } catch (IllegalArgumentException e) {
            System.out.println("no constant CENTRE");
        }
    }
}
```

```output for=enum-switch
hi.... ....hi ..hi.. 
2
no constant CENTRE
```

- Verdict: idiom. Java has no literal types. An `enum` class has a fixed set of instances, so a parameter of type `Align` takes only these three, and a `switch` expression that leaves one out does not compile. A string from outside must go through `valueOf`, which throws for a name that is not a constant.
