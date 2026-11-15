```java run id=one-script-per-request
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Customer(String name, String tier) {}
    record Product(String name, long priceCents) {}
    record OrderLine(int orderId, int productId, int qty) {}

    // The tables of the database, kept in memory for this example.
    static final Map<Integer, Customer> customer = new LinkedHashMap<>();
    static final Map<Integer, Product> product = new LinkedHashMap<>();
    static final Map<Integer, Integer> orders = new LinkedHashMap<>(); // order id to customer id
    static final List<OrderLine> orderLine = new ArrayList<>();
    static int nextOrderId = 1;

    static class ScriptError extends RuntimeException {
        ScriptError(String message) { super(message); }
    }

    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    // Transaction script 1: place an order. Each item is {productId, qty}.
    static int placeOrder(int customerId, int[][] items) {
        if (!customer.containsKey(customerId)) {
            throw new ScriptError("no customer " + customerId);
        }
        // Check everything first, so that a failed script writes nothing.
        for (int[] item : items) {
            if (item[1] <= 0) throw new ScriptError("bad quantity " + item[1]);
            if (!product.containsKey(item[0])) throw new ScriptError("no product " + item[0]);
        }
        int orderId = nextOrderId++;
        orders.put(orderId, customerId);
        for (int[] item : items) orderLine.add(new OrderLine(orderId, item[0], item[1]));
        return orderId;
    }

    // Transaction script 2: show the total of an order. Returns {subtotal, total}.
    static long[] orderTotal(int orderId) {
        Integer customerId = orders.get(orderId);
        if (customerId == null) throw new ScriptError("no order " + orderId);
        String tier = customer.get(customerId).tier();
        long subtotal = 0;
        for (OrderLine line : orderLine) {
            if (line.orderId() == orderId) subtotal += line.qty() * product.get(line.productId()).priceCents();
        }
        long total = subtotal;
        if (tier.equals("gold") && subtotal >= 10000) total = subtotal - subtotal / 10;
        return new long[] {subtotal, total};
    }

    public static void main(String[] args) {
        customer.put(1, new Customer("Ann", "gold"));
        customer.put(2, new Customer("Bob", "regular"));
        product.put(1, new Product("lamp", 2500));
        product.put(2, new Product("desk", 7500));
        product.put(3, new Product("pen", 500));

        int a = placeOrder(1, new int[][] {{1, 2}, {2, 1}});
        System.out.println("placed order " + a + " for " + customer.get(1).name());
        int b = placeOrder(2, new int[][] {{1, 1}, {3, 3}});
        System.out.println("placed order " + b + " for " + customer.get(2).name());
        for (int orderId : new int[] {1, 2}) {
            long[] t = orderTotal(orderId);
            System.out.println("order " + orderId + ": subtotal " + cents(t[0]) + ", total " + cents(t[1]));
        }
        int[][][] bad = {{{9, 1}}, {{1, 0}}};
        int[] who = {1, 2};
        for (int i = 0; i < bad.length; i++) {
            try {
                placeOrder(who[i], bad[i]);
            } catch (ScriptError e) {
                System.out.println("error: " + e.getMessage());
            }
        }
        System.out.println("orders in the database: " + orders.size());
    }
}
```

```output
placed order 1 for Ann
placed order 2 for Bob
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
error: no product 9
error: bad quantity 0
orders in the database: 2
```

- Verdict: idiom. Each script is a static method. In a Java application, a script of this kind is often a method of a class with one method for each request, and the method runs in one JDBC or JPA transaction.
- `ScriptError` is unchecked, so the scripts do not declare it, and the caller catches it where it can report the error to the user.
