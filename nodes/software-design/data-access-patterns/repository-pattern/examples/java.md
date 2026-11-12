```java run id=orders
import java.util.List;
import java.util.Optional;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    enum Status { OPEN, SHIPPED }

    // An immutable domain object: ship() returns the changed order.
    record Order(int id, String customer, int total, Status status) {
        Order(int id, String customer, int total) { this(id, customer, total, Status.OPEN); }

        Order ship() {
            if (status != Status.OPEN) throw new IllegalStateException("order " + id + " is " + status);
            return new Order(id, customer, total, Status.SHIPPED);
        }
    }

    interface OrderRepository {
        void add(Order order);
        Optional<Order> get(int id);
        List<Order> byCustomer(String customer);
        void update(Order order);
    }

    static final class InMemoryOrderRepository implements OrderRepository {
        private final TreeMap<Integer, Order> rows = new TreeMap<>();

        public void add(Order order) {
            if (rows.putIfAbsent(order.id(), order) != null)
                throw new IllegalArgumentException("order " + order.id() + " exists");
        }
        public Optional<Order> get(int id) { return Optional.ofNullable(rows.get(id)); }
        public List<Order> byCustomer(String customer) {
            return rows.values().stream().filter(o -> o.customer().equals(customer)).toList();
        }
        public void update(Order order) {
            if (rows.replace(order.id(), order) == null)
                throw new IllegalArgumentException("order " + order.id() + " does not exist");
        }
    }

    static String shipOrder(OrderRepository orders, int id) {
        return orders.get(id)
                .map(order -> {
                    Order shipped = order.ship();
                    orders.update(shipped);
                    return "order " + id + " is " + shipped.status().name().toLowerCase();
                })
                .orElse("order " + id + ": not found");
    }

    public static void main(String[] args) {
        OrderRepository repo = new InMemoryOrderRepository();
        repo.add(new Order(1, "ana", 30));
        repo.add(new Order(2, "bo", 12));
        repo.add(new Order(3, "ana", 5));
        System.out.println("ana: " + repo.byCustomer("ana").stream()
                .map(o -> "order " + o.id() + " (" + o.total() + ")")
                .collect(Collectors.joining(", ")));
        System.out.println(shipOrder(repo, 1));
        System.out.println(shipOrder(repo, 9));
        Order stored = repo.get(1).orElseThrow();
        System.out.println("stored: order " + stored.id() + " is " + stored.status().name().toLowerCase());
    }
}
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. An interface gives the collection-like contract, and `Optional` makes "not found" part of the return type of `get`.
- `Order` is a record, so it is immutable. The repository can store the object itself, because no caller can change it. A change makes another record, and the caller must pass it to `update`.
- `TreeMap` keeps the orders sorted by id, so `byCustomer` returns them in a stable order.
- Spring Data generates an implementation of an interface like this one from the names of its methods. That library is not part of the JDK, so this program writes the implementation by hand.
