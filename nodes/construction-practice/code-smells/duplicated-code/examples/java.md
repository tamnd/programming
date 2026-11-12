```java run id=extract-shared-weight
import java.util.List;

public class Main {
    record Item(double kg, int qty) {}

    // Before: the weight rule is written twice.
    static long domesticBefore(List<Item> items) {
        double weight = 0;
        for (Item item : items) weight += item.kg() * item.qty();
        return 5 + (long) Math.ceil(weight) * 2;
    }

    static long internationalBefore(List<Item> items) {
        double weight = 0;
        for (Item item : items) weight += item.kg() * item.qty();
        return 20 + (long) Math.ceil(weight) * 6;
    }

    // After: the weight rule has one home.
    private static long billableKg(List<Item> items) {
        double weight = 0;
        for (Item item : items) weight += item.kg() * item.qty();
        return (long) Math.ceil(weight);
    }

    static long domestic(List<Item> items) { return 5 + billableKg(items) * 2; }
    static long international(List<Item> items) { return 20 + billableKg(items) * 6; }

    interface Price { long of(List<Item> items); }

    static void check(String name, Price dom, Price intl) {
        List<Item> items = List.of(new Item(0.4, 3), new Item(1.1, 1));
        if (dom.of(items) != 11 || intl.of(items) != 38 || dom.of(List.of()) != 5) {
            throw new AssertionError(name);
        }
        System.out.println(name + ": tests pass");
    }

    public static void main(String[] args) {
        check("before", Main::domesticBefore, Main::internationalBefore);
        check("after", Main::domestic, Main::international);
    }
}
```

```output
before: tests pass
after: tests pass
```

- `Math.ceil` returns a `double`, so each version casts the result to `long`. In the "before" version the cast is written twice, and a fix to it must be made twice.
- The extracted method is `private`. The callers keep their public names, so no code outside the class changes.
- The test throws `AssertionError` itself, because the `assert` statement is off unless the JVM runs with `-ea`.
