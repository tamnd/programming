```java run id=hide-the-path
public class Main {
    record Address(String country) {}

    record Customer(Address address) {}

    static final class Order {
        private final Customer customer;
        private final Address delivery;

        Order(Customer customer, Address delivery) {
            this.customer = customer;
            this.delivery = delivery;
        }

        Customer getCustomer() { return customer; }

        // After: the order answers the question, and knows the path.
        String shipsTo() {
            return (delivery != null ? delivery : customer.address()).country();
        }
    }

    // Before: the client walks the chain.
    static String carrierBefore(Order order) {
        return order.getCustomer().address().country().equals("US") ? "ups" : "dhl";
    }

    // After: the client asks the order.
    static String carrier(Order order) {
        return order.shipsTo().equals("US") ? "ups" : "dhl";
    }

    public static void main(String[] args) {
        Order home = new Order(new Customer(new Address("DE")), null);
        if (!carrierBefore(home).equals(carrier(home))) throw new AssertionError("home");

        Order gift = new Order(new Customer(new Address("DE")), new Address("US"));
        if (!carrierBefore(gift).equals("dhl")) throw new AssertionError("before"); // reads the customer's address
        if (!carrier(gift).equals("ups")) throw new AssertionError("after");
        System.out.println("shipsTo: tests pass");
    }
}
```

```output
shipsTo: tests pass
```

- `order.getCustomer().address().country()` has three dots after `order`, and each one is a type the client must know. `order.shipsTo()` has one.
- Once no client calls `getCustomer` to reach the address, the getter may not be needed at all, and the customer becomes a detail of the order.
