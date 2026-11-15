```java run id=one-service-two-clients
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    static class DomainError extends RuntimeException {
        DomainError(String message) { super(message); }
    }

    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    record Customer(String name, String email, String tier) {}
    record Line(String item, int qty, long priceCents) {}
    record Item(String name, int qty) {}
    record Placed(int number, String customer, long total) {}

    // Domain layer: the objects hold the business rules.
    static class Order {
        final int number;
        final Customer customer;
        final List<Line> lines = new ArrayList<>();

        Order(int number, Customer customer) {
            this.number = number;
            this.customer = customer;
        }

        void add(String item, int qty, long priceCents) {
            if (qty <= 0) throw new DomainError("bad quantity " + qty);
            lines.add(new Line(item, qty, priceCents));
        }

        long total() {
            long subtotal = lines.stream().mapToLong(l -> l.qty() * l.priceCents()).sum();
            if (customer.tier().equals("gold") && subtotal >= 10000) return subtotal - subtotal / 10;
            return subtotal;
        }
    }

    // Data source and other resources, which the service layer coordinates.
    static class Store {
        final Map<Integer, Customer> customers = Map.of(
                1, new Customer("Ann", "ann@example.com", "gold"),
                2, new Customer("Bob", "bob@example.com", "regular"));
        final Map<String, Long> prices = Map.of("lamp", 2500L, "desk", 7500L, "pen", 500L);
        Map<Integer, Order> orders = new LinkedHashMap<>();
        private Map<Integer, Order> saved;

        void begin() { saved = new LinkedHashMap<>(orders); } // a copy of the map of orders
        void commit() { saved = null; }
        void rollback() { orders = saved; }
    }

    static class Mailer {
        final List<String> sent = new ArrayList<>();
        void send(String to, String text) { sent.add(to + ": " + text); }
    }

    // Service layer: the operations of the application, with the transaction and the
    // coordination of the responses in one place.
    static class OrderService {
        private final Store store;
        private final Mailer mailer;

        OrderService(Store store, Mailer mailer) {
            this.store = store;
            this.mailer = mailer;
        }

        Placed placeOrder(int customerId, List<Item> items) {
            store.begin();
            Customer customer;
            Order order;
            try {
                customer = store.customers.get(customerId);
                if (customer == null) throw new DomainError("no customer " + customerId);
                order = new Order(store.orders.size() + 1, customer);
                store.orders.put(order.number, order);
                for (Item item : items) {
                    Long price = store.prices.get(item.name());
                    if (price == null) throw new DomainError("no product " + item.name());
                    order.add(item.name(), item.qty(), price);
                }
                store.commit();
            } catch (DomainError e) {
                store.rollback();
                throw e;
            }
            // Only a committed order gets an email.
            mailer.send(customer.email(), "order " + order.number + ", total " + cents(order.total()));
            return new Placed(order.number, customer.name(), order.total());
        }
    }

    public static void main(String[] args) {
        Store store = new Store();
        Mailer mailer = new Mailer();
        OrderService service = new OrderService(store, mailer);

        // Client 1: a web handler.
        Placed p = service.placeOrder(1, List.of(new Item("lamp", 2), new Item("desk", 1)));
        System.out.println("web: placed order " + p.number() + " for " + p.customer() + ", total " + cents(p.total()));

        // Client 2: a loader that imports orders from a file.
        List<List<Item>> rows = List.of(
                List.of(new Item("lamp", 1), new Item("pen", 3)),
                List.of(new Item("pen", 1), new Item("chair", 1)));
        for (int i = 0; i < rows.size(); i++) {
            try {
                Placed q = service.placeOrder(2, rows.get(i));
                System.out.println("import line " + (i + 1) + ": placed order " + q.number() + " for " + q.customer()
                        + ", total " + cents(q.total()));
            } catch (DomainError e) {
                System.out.println("import line " + (i + 1) + ": refused: " + e.getMessage());
            }
        }

        System.out.println("orders stored: " + store.orders.size());
        for (String message : mailer.sent) System.out.println("email " + message);
    }
}
```

```output
web: placed order 1 for Ann, total 112.50
import line 1: placed order 2 for Bob, total 40.00
import line 2: refused: no product chair
orders stored: 2
email ann@example.com: order 1, total 112.50
email bob@example.com: order 2, total 40.00
```

- Verdict: idiom. `OrderService` is a class with one method for each operation. In a Spring application, the class would have the `@Service` annotation, and `@Transactional` on the method would replace the calls to `begin`, `commit` and `rollback`.
- `DomainError` is unchecked, so `placeOrder` does not declare it. The service catches it only to roll back, and then throws it again to the client.
