```java run id=orders-that-know-their-rules
import java.util.ArrayList;
import java.util.List;

public class Main {
    static class DomainError extends RuntimeException {
        DomainError(String message) { super(message); }
    }

    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    // Each kind of customer knows its own discount rule.
    static class Customer {
        final String name;
        Customer(String name) { this.name = name; }
        String kind() { return "regular"; }
        long discount(long subtotal) { return 0; }
    }

    static class GoldCustomer extends Customer {
        GoldCustomer(String name) { super(name); }
        @Override String kind() { return "gold"; }
        @Override long discount(long subtotal) { return subtotal >= 10000 ? subtotal / 10 : 0; }
    }

    static class StaffCustomer extends Customer {
        StaffCustomer(String name) { super(name); }
        @Override String kind() { return "staff"; }
        @Override long discount(long subtotal) { return subtotal / 5; }
    }

    record Product(String name, long priceCents) {}

    record OrderLine(Product product, int qty) {
        OrderLine {
            if (qty <= 0) throw new DomainError("bad quantity " + qty);
        }
        long amount() { return qty * product.priceCents(); }
    }

    static class Order {
        final int number;
        final Customer customer;
        private final List<OrderLine> lines = new ArrayList<>();
        private boolean placed;

        Order(int number, Customer customer) {
            this.number = number;
            this.customer = customer;
        }

        void add(Product product, int qty) {
            if (placed) throw new DomainError("order " + number + " is placed, so it takes no new lines");
            lines.add(new OrderLine(product, qty));
        }

        void place() {
            if (lines.isEmpty()) throw new DomainError("order " + number + " has no lines");
            placed = true;
        }

        long subtotal() {
            return lines.stream().mapToLong(OrderLine::amount).sum();
        }

        long total() {
            long subtotal = subtotal();
            return subtotal - customer.discount(subtotal);
        }
    }

    public static void main(String[] args) {
        Product lamp = new Product("lamp", 2500), desk = new Product("desk", 7500), pen = new Product("pen", 500);
        List<Order> orders = List.of(
                new Order(1, new GoldCustomer("Ann")),
                new Order(2, new Customer("Bob")),
                new Order(3, new StaffCustomer("Cy")));
        orders.get(0).add(lamp, 2);
        orders.get(0).add(desk, 1);
        for (Order order : orders.subList(1, 3)) {
            order.add(lamp, 1);
            order.add(pen, 3);
        }
        for (Order order : orders) {
            order.place();
            System.out.println("order " + order.number + " for " + order.customer.name + " (" + order.customer.kind()
                    + "): subtotal " + cents(order.subtotal()) + ", total " + cents(order.total()));
        }
        List<Runnable> actions = List.of(
                () -> orders.get(0).add(pen, 1),
                () -> new Order(4, new Customer("Di")).place(),
                () -> new Order(5, new Customer("Ed")).add(pen, 0));
        for (Runnable action : actions) {
            try {
                action.run();
            } catch (DomainError e) {
                System.out.println("error: " + e.getMessage());
            }
        }
    }
}
```

```output
order 1 for Ann (gold): subtotal 125.00, total 112.50
order 2 for Bob (regular): subtotal 40.00, total 40.00
order 3 for Cy (staff): subtotal 40.00, total 32.00
error: order 1 is placed, so it takes no new lines
error: order 4 has no lines
error: bad quantity 0
```

- Verdict: idiom. `OrderLine` is a record whose compact constructor checks the quantity. `Order` keeps its list private, so no other class can add a line without the check in `add`. The customer subclasses override `discount`.
- In a Spring or Jakarta EE application, JPA would map these classes to tables, and the rules would stay in the classes.
