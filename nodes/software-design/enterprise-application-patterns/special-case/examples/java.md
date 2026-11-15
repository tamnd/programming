```java run id=unknown-customer
import java.util.List;
import java.util.Map;

public class Main {
    static class Customer {
        final int id;
        final String name;
        final String plan;

        Customer(int id, String name, String plan) {
            this.id = id;
            this.name = name;
            this.plan = plan;
        }

        boolean isKnown() { return true; }

        String billLine() { return "customer " + id + ": " + name + ", plan " + plan; }
    }

    // The special case: the same interface, with the behavior for a customer with no record.
    static class UnknownCustomer extends Customer {
        UnknownCustomer(int id) { super(id, "current resident", "basic"); }

        @Override
        boolean isKnown() { return false; }
    }

    static final Map<Integer, String[]> ROWS = Map.of(
            1, new String[] {"Ann", "premium"},
            2, new String[] {"Bob", "basic"});

    static Customer findCustomer(int id) {
        String[] row = ROWS.get(id);
        if (row == null) return new UnknownCustomer(id); // not null
        return new Customer(id, row[0], row[1]);
    }

    public static void main(String[] args) {
        List<Customer> customers = List.of(findCustomer(1), findCustomer(2), findCustomer(9));
        for (Customer c : customers) System.out.println(c.billLine()); // no test for null
        System.out.println("known customers: " + customers.stream().filter(Customer::isKnown).count());
    }
}
```

```output
customer 1: Ann, plan premium
customer 2: Bob, plan basic
customer 9: current resident, plan basic
known customers: 2
```

- Verdict: idiom. `UnknownCustomer` is a subclass, so the compiler checks that the finder returns a `Customer` and that callers use only its methods.
- `Optional<Customer>` is the other common choice. It makes the caller handle the missing case, while a special case gives the caller a default behavior.
