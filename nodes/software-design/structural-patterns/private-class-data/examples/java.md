```java run id=order-data
import java.util.ArrayList;
import java.util.List;

record OrderData(int orderId, String customer) {}

class Order {
    private final OrderData data; // fixed after construction
    private final List<String> items; // changes

    Order(int orderId, String customer, List<String> items) {
        this.data = new OrderData(orderId, customer);
        this.items = new ArrayList<>(items);
    }

    int orderId() { return data.orderId(); }
    String customer() { return data.customer(); }
    void add(String item) { items.add(item); }

    String describe() {
        return "order " + orderId() + " for " + customer() + ": " + items.size() + " items";
    }
}

public class Main {
    public static void main(String[] args) {
        Order order = new Order(42, "ann", List.of("pen", "ink"));
        System.out.println(order.describe());
        order.add("paper");
        System.out.println(order.describe());
    }
}
```

```output
order 42 for ann: 2 items
order 42 for ann: 3 items
```

- Verdict: idiom. A record is the data class: its component fields are private and final, and it has accessors but no setters. For one or two fields, `private final` fields in `Order` give the same protection.
