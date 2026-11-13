```java run id=three-layers
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

public class Main {
    record Order(String customer, String item, int quantity, int cents) {}

    static final class Tables {
        final Map<String, Integer> stock = new LinkedHashMap<>();
        final List<Order> orders = new ArrayList<>();

        Tables copy() {
            Tables t = new Tables();
            t.stock.putAll(stock);
            t.orders.addAll(orders); // the orders are immutable records
            return t;
        }
    }

    // Data source layer: the stored data, and a transaction that keeps all or none of a change.
    static final class Store {
        Tables tables = new Tables();

        Store() {
            tables.stock.put("widget", 10);
        }

        boolean transaction(Consumer<Tables> change) {
            Tables saved = tables.copy();
            try {
                change.accept(tables);
                return true;
            } catch (IllegalStateException e) {
                tables = saved; // roll back
                return false;
            }
        }
    }

    // Domain layer: the business rules, with a special deal for one customer.
    static final int PRICE_CENTS = 1000;

    static int priceCents(String customer, int quantity) {
        int total = PRICE_CENTS * quantity;
        if (customer.equals("acme") && quantity >= 3) {
            total = total * 90 / 100; // a deal that sales made with one customer
        }
        return total;
    }

    static boolean placeOrder(Store store, String customer, String item, int quantity) {
        return store.transaction(tables -> {
            tables.orders.add(new Order(customer, item, quantity, priceCents(customer, quantity)));
            int count = tables.stock.get(item);
            if (count < quantity) throw new IllegalStateException("not enough stock");
            tables.stock.put(item, count - quantity);
        });
    }

    static String money(int cents) {
        return String.format("%d.%02d", cents / 100, cents % 100);
    }

    // Presentation layer: two screens show the same data in different ways.
    static void orderScreen(Store store) {
        for (Order o : store.tables.orders) {
            System.out.println("order screen: " + o.customer() + " bought " + o.quantity() + " x " + o.item()
                    + " for " + money(o.cents()));
        }
    }

    static void stockScreen(Store store) {
        for (Map.Entry<String, Integer> e : store.tables.stock.entrySet()) {
            System.out.println("stock screen: " + e.getKey() + ": " + e.getValue() + " in stock, orders: "
                    + store.tables.orders.size());
        }
    }

    public static void main(String[] args) {
        Store store = new Store();
        System.out.println("acme orders 3: " + (placeOrder(store, "acme", "widget", 3) ? "kept" : "rolled back"));
        System.out.println("bolt orders 9: " + (placeOrder(store, "bolt", "widget", 9) ? "kept" : "rolled back"));
        orderScreen(store);
        stockScreen(store);
    }
}
```

```output
acme orders 3: kept
bolt orders 9: rolled back
order screen: acme bought 3 x widget for 27.00
stock screen: widget: 7 in stock, orders: 1
```

- Verdict: library. Jakarta EE and Spring give transactions, persistence and web screens; JDBC in the standard library connects to the database. The code examples of PoEAA are in Java, C# or both. The example uses records for the orders, so a copy of the list is a copy of the table.
