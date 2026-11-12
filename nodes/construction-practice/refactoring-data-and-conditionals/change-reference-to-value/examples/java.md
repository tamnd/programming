```java run id=replace-not-change
import java.util.HashSet;
import java.util.Set;

public class Main {
    // A record has final fields, and equals and hashCode from its components.
    record DeliveryWindow(int day, int startHour, int endHour) {
        DeliveryWindow movedBy(int days) {
            return new DeliveryWindow(day + days, startHour, endHour);
        }
    }

    static final class Order {
        DeliveryWindow window;
        Order(DeliveryWindow w) { window = w; }
    }

    public static void main(String[] args) {
        DeliveryWindow morning = new DeliveryWindow(3, 9, 12);
        Order c = new Order(morning);
        Order d = new Order(morning);
        c.window = c.window.movedBy(1);
        if (d.window.day() != 3) throw new AssertionError("d moved");
        if (!c.window.equals(new DeliveryWindow(4, 9, 12))) throw new AssertionError("equals");
        Set<DeliveryWindow> set = new HashSet<>(Set.of(c.window));
        if (!set.contains(new DeliveryWindow(4, 9, 12))) throw new AssertionError("hashCode");
        System.out.println("change reference to value: tests pass");
    }
}
```

```output
change reference to value: tests pass
```
