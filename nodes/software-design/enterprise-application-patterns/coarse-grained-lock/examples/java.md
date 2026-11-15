```java run id=customer-and-addresses
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static class ConflictException extends Exception {
        ConflictException(String message) {
            super(message);
        }
    }

    static class Customer {
        String name;
        final String lock;

        Customer(String name, String lock) {
            this.name = name;
            this.lock = lock;
        }

        Customer copy() {
            return new Customer(name, lock);
        }
    }

    static class Address {
        final int customer;
        final String street;
        boolean primary;

        Address(int customer, String street, boolean primary) {
            this.customer = customer;
            this.street = street;
            this.primary = primary;
        }

        Address copy() {
            return new Address(customer, street, primary);
        }
    }

    // The tables. A customer and its addresses form one group, with one shared version.
    static final Map<String, Integer> versions = new HashMap<>();
    static final Map<Integer, Customer> customers = new HashMap<>();
    static final Map<Integer, Address> addresses = new TreeMap<>();
    static int checks = 0;

    // A session's copy of the customer and its addresses, with the version of the group.
    static class Group {
        final int customerId;
        final String lock;
        int version;
        final Customer customer;
        final Map<Integer, Address> addresses = new TreeMap<>();

        Group(int customerId) {
            this.customerId = customerId;
            Customer c = customers.get(customerId);
            this.lock = c.lock;
            this.version = versions.get(lock);
            this.customer = c.copy();
            for (Map.Entry<Integer, Address> e : Main.addresses.entrySet()) {
                if (e.getValue().customer == customerId) {
                    this.addresses.put(e.getKey(), e.getValue().copy());
                }
            }
        }

        // The rule of the group: at most one primary address for each customer.
        void makePrimary(int addressId) {
            for (Map.Entry<Integer, Address> e : addresses.entrySet()) {
                e.getValue().primary = e.getKey() == addressId;
            }
        }
    }

    static void save(Group group) throws ConflictException {
        checks++; // one check for the whole group, however many members changed
        int current = versions.get(group.lock);
        if (current != group.version) {
            throw new ConflictException("the group of customer " + group.customerId + " changed (version " + current + ")");
        }
        versions.put(group.lock, current + 1);
        group.version = current + 1;
        customers.put(group.customerId, group.customer.copy());
        for (Map.Entry<Integer, Address> e : group.addresses.entrySet()) {
            addresses.put(e.getKey(), e.getValue().copy());
        }
    }

    static void trySave(String who, Group group) {
        try {
            save(group);
            System.out.println(who + " saves: ok, group version " + group.version);
        } catch (ConflictException e) {
            System.out.println(who + " saves: conflict: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        versions.put("customer 1", 1);
        customers.put(1, new Customer("Acme Ltd", "customer 1"));
        addresses.put(1, new Address(1, "1 Elm Street", false));
        addresses.put(2, new Address(1, "9 Oak Road", false));

        Group ann = new Group(1);
        Group bob = new Group(1);
        System.out.println("customer 1: " + ann.customer.name + " with " + ann.addresses.size() + " addresses, group version " + ann.version);
        ann.makePrimary(1);
        System.out.println("ann makes 1 Elm Street the primary address");
        bob.makePrimary(2);
        System.out.println("bob makes 9 Oak Road the primary address");
        trySave("ann", ann);
        trySave("bob", bob);
        List<String> primary = new ArrayList<>();
        for (Address a : addresses.values()) {
            if (a.primary) {
                primary.add(a.street);
            }
        }
        System.out.println("primary addresses in the database: " + String.join(", ", primary));
        System.out.println("version checks: " + checks);
    }
}
```

```output
customer 1: Acme Ltd with 2 addresses, group version 1
ann makes 1 Elm Street the primary address
bob makes 9 Oak Road the primary address
ann saves: ok, group version 2
bob saves: conflict: the group of customer 1 changed (version 2)
primary addresses in the database: 1 Elm Street
version checks: 2
```

- Verdict: idiom. `Group` holds copies of the `Customer` and of each `Address`, and the lock name of the customer is the key into `versions`. `save` throws the checked `ConflictException` when the shared version changed.
- The addresses are in a `TreeMap`, so the program lists them in the order of their ids. With Jakarta Persistence, a lock with `OPTIMISTIC_FORCE_INCREMENT` on the customer increases its version when only an address changes.
