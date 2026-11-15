```java run id=one-call-for-an-order
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

public class Main {
    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    // Fine-grained domain objects: small objects with small methods.
    static class Address {
        private String street, city;
        Address(String street, String city) { this.street = street; this.city = city; }
        String street() { return street; }
        String city() { return city; }
        void moveTo(String street, String city) { this.street = street; this.city = city; }
    }

    static class Customer {
        private final String name;
        private final Address address;
        Customer(String name, Address address) { this.name = name; this.address = address; }
        String name() { return name; }
        Address address() { return address; }
    }

    record Line(String item, int qty, long price) {
        long amount() { return qty * price; }
    }

    record Order(int number, Customer customer, List<Line> lines) {
        long total() { return lines.stream().mapToLong(Line::amount).sum(); }
    }

    // The data that crosses the network: immutable values, with no behavior.
    record LineView(String item, int qty, long price) {}
    record OrderView(int number, String customer, String shipTo, List<LineView> lines, long total) {}

    // Remote facade: coarse-grained methods over the fine-grained objects, with no domain logic.
    static class OrderFacade {
        private final Map<Integer, Order> orders;
        OrderFacade(Map<Integer, Order> orders) { this.orders = orders; }

        OrderView getOrder(int number) {
            Order order = orders.get(number);
            Address address = order.customer().address();
            List<LineView> lines = order.lines().stream()
                    .map(l -> new LineView(l.item(), l.qty(), l.price())).toList();
            return new OrderView(order.number(), order.customer().name(),
                    address.street() + ", " + address.city(), lines, order.total());
        }

        void changeAddress(int number, String street, String city) {
            orders.get(number).customer().address().moveTo(street, city);
        }
    }

    // The network: each call is counted. A real remote call would also serialize the values.
    static class Network {
        int calls = 0;
        <T> T call(Supplier<T> request) {
            calls++;
            return request.get();
        }
        void send(Runnable request) {
            calls++;
            request.run();
        }
    }

    public static void main(String[] args) {
        Customer ann = new Customer("Ann", new Address("1 Elm Street", "Leeds"));
        Map<Integer, Order> orders = Map.of(7, new Order(7, ann,
                List.of(new Line("lamp", 2, 2500), new Line("desk", 1, 7500), new Line("pen", 4, 500))));
        OrderFacade facade = new OrderFacade(orders);
        Network net = new Network();

        // The client: one remote call gets all the data that the screen needs.
        OrderView view = net.call(() -> facade.getOrder(7));
        System.out.println("order " + view.number() + " for " + view.customer() + ": " + view.lines().size()
                + " lines, total " + cents(view.total()));
        for (LineView line : view.lines()) {
            System.out.println("  " + line.item() + " x" + line.qty() + " at " + cents(line.price()));
        }
        System.out.println("ship to: " + view.shipTo());

        net.send(() -> facade.changeAddress(7, "2 Oak Road", "York"));
        System.out.println("ship to: " + net.call(() -> facade.getOrder(7)).shipTo());
        System.out.println("remote calls: " + net.calls);
    }
}
```

```output
order 7 for Ann: 3 lines, total 145.00
  lamp x2 at 25.00
  desk x1 at 75.00
  pen x4 at 5.00
ship to: 1 Elm Street, Leeds
ship to: 2 Oak Road, York
remote calls: 3
```

- Verdict: idiom. `OrderFacade` returns an `OrderView` record, which is immutable and has no reference to the domain objects. In the Core J2EE patterns, a session bean with this role is a Session Facade.
- `Network` only counts the calls. A real remote call, for example over HTTP with a JSON library or over gRPC, would also serialize the records.
