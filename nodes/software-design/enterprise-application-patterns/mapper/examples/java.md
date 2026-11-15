```java run id=order-warehouse-mapper
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

public class Main {
    // Subsystem 1: the order system. It knows nothing about the warehouse.
    record OrderLine(String item, int qty) {}

    static class Order {
        final int number;
        final String customer;
        final List<OrderLine> lines;
        String status = "new";
        Order(int number, String customer, List<OrderLine> lines) {
            this.number = number; this.customer = customer; this.lines = lines;
        }
    }

    // Subsystem 2: the warehouse. It knows nothing about orders. It works with
    // product codes and reference strings, and it cannot be changed.
    record PickLine(String code, int qty) {}
    record PickRequest(String ref, List<PickLine> lines) {}
    record PickReply(String ref, String state) {}

    static class Warehouse {
        static final Set<String> CODES = Set.of("PEN-01", "INK-02");
        PickReply pick(PickRequest request) {
            long known = request.lines().stream().filter(l -> CODES.contains(l.code())).count();
            return new PickReply(request.ref(), known == request.lines().size() ? "PICKED" : "SHORT");
        }
    }

    // The mapper knows both subsystems and connects them. Neither of them knows the mapper.
    static class OrderWarehouseMapper {
        static final Map<String, String> CODE_OF = Map.of("pen", "PEN-01", "ink", "INK-02");
        static final Map<String, String> STATUS_OF = Map.of("PICKED", "picked", "SHORT", "waiting for stock");
        private final Warehouse warehouse;
        OrderWarehouseMapper(Warehouse warehouse) { this.warehouse = warehouse; }

        void send(Order order) {
            List<PickLine> lines = new ArrayList<>();
            for (OrderLine l : order.lines) lines.add(new PickLine(CODE_OF.get(l.item()), l.qty()));
            PickRequest request = new PickRequest("ORD-" + order.number, lines);
            System.out.println("warehouse gets: ref " + request.ref() + ", lines "
                + lines.stream().map(l -> l.code() + " x" + l.qty()).collect(Collectors.joining(", ")));
            PickReply reply = warehouse.pick(request);
            System.out.println("warehouse replies: ref " + reply.ref() + ", state " + reply.state());
            order.status = STATUS_OF.get(reply.state());
        }
    }

    public static void main(String[] args) {
        Order order = new Order(7, "ann", List.of(new OrderLine("pen", 2), new OrderLine("ink", 1)));
        System.out.println("order " + order.number + " for " + order.customer + ": "
            + order.lines.stream().map(l -> l.qty() + " x " + l.item()).collect(Collectors.joining(", "))
            + ", status " + order.status);
        new OrderWarehouseMapper(new Warehouse()).send(order);
        System.out.println("order " + order.number + " status: " + order.status);
    }
}
```

```output
order 7 for ann: 2 x pen, 1 x ink, status new
warehouse gets: ref ORD-7, lines PEN-01 x2, INK-02 x1
warehouse replies: ref ORD-7, state PICKED
order 7 status: picked
```

- Verdict: idiom. The records `PickLine`, `PickRequest` and `PickReply` (Java 16 or later) are the types of the warehouse, and `OrderLine` is the type of the order system. Only `OrderWarehouseMapper` uses both.
- In a real program, a library such as MapStruct can generate the copy of the fields, and the mapper keeps the rules that are not a plain copy.
