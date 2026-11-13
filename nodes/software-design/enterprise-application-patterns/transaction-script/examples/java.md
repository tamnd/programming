```java run id=transaction-script
import java.util.HashMap;
import java.util.Map;

public class Main {
    static final class Customer {
        final String name;
        int credit;

        Customer(String name, int credit) {
            this.name = name;
            this.credit = credit;
        }
    }

    static final class Order {
        final int customer;
        final int cents;
        String state = "open";

        Order(int customer, int cents) {
            this.customer = customer;
            this.cents = cents;
        }
    }

    // A thin wrapper over the stored tables. The scripts call it directly.
    static final Map<Integer, Customer> customers = new HashMap<>(Map.of(1, new Customer("ada", 5000)));
    static final Map<Integer, Order> orders = new HashMap<>();

    static Customer findCustomer(int customerId) {
        return customers.get(customerId);
    }

    static int insertOrder(int customerId, int cents) {
        int orderId = orders.size() + 1;
        orders.put(orderId, new Order(customerId, cents));
        return orderId;
    }

    static String money(int cents) {
        return String.format("%d.%02d", cents / 100, cents % 100);
    }

    // One transaction script for each request from the presentation.
    static String placeOrder(int customerId, int cents) {
        Customer customer = findCustomer(customerId);
        if (customer.credit < cents) return "refused, credit left " + money(customer.credit);
        customer.credit -= cents;
        return "order " + insertOrder(customerId, cents);
    }

    static String cancelOrder(int orderId) {
        Order order = orders.get(orderId);
        if (!order.state.equals("open")) return "already cancelled";
        order.state = "cancelled";
        findCustomer(order.customer).credit += order.cents;
        return "refunded " + money(order.cents);
    }

    public static void main(String[] args) {
        System.out.println("place_order(ada, 30.00): " + placeOrder(1, 3000));
        System.out.println("place_order(ada, 30.00): " + placeOrder(1, 3000));
        System.out.println("cancel_order(1): " + cancelOrder(1));
        System.out.println("cancel_order(1): " + cancelOrder(1));
        System.out.println("place_order(ada, 30.00): " + placeOrder(1, 3000));
        System.out.println("credit of ada: " + money(findCustomer(1).credit));
    }
}
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a static method, and the tables are maps of small row classes.
