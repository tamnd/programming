```java run id=four-kinds-of-lazy-load
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

public class Main {
    // A stand-in for the database: the tables and a count of SELECT statements.
    static final Map<Integer, String[]> CUSTOMERS = Map.of(1, new String[] {"Ann", "Oslo"}, 2, new String[] {"Bo", "Rome"});
    static final int[][] ORDERS = {{10, 1}, {11, 2}, {12, 1}};
    static int selects = 0;

    static String[] selectCustomer(int id) {
        selects++;
        return CUSTOMERS.get(id);
    }

    interface Customer {
        int id();
        String name();
        String city();
    }

    record RealCustomer(int id, String name, String city) implements Customer {
        static RealCustomer load(int id) {
            String[] row = selectCustomer(id);
            return new RealCustomer(id, row[0], row[1]);
        }
    }

    // 1. Lazy initialization: a marker value (null) means "not loaded".
    static class Order {
        final int id;
        final int customerId;
        private Customer customer = null;

        Order(int id, int customerId) {
            this.id = id;
            this.customerId = customerId;
        }

        Customer customer() {
            if (customer == null) customer = RealCustomer.load(customerId);
            return customer;
        }
    }

    // 2. Virtual proxy: it implements Customer and loads the real one on first use.
    static class CustomerProxy implements Customer {
        private final int id;
        private Customer real = null;

        CustomerProxy(int id) {
            this.id = id;
        }

        private Customer real() {
            if (real == null) real = RealCustomer.load(id);
            return real;
        }

        public int id() { return id; }
        public String name() { return real().name(); }
        public String city() { return real().city(); }
    }

    // 3. Value holder: the client asks the holder for the value.
    static class ValueHolder<T> {
        private final Supplier<T> loader;
        private boolean loaded = false;
        private T value;

        ValueHolder(Supplier<T> loader) {
            this.loader = loader;
        }

        T getValue() {
            if (!loaded) {
                value = loader.get();
                loaded = true;
            }
            return value;
        }
    }

    // 4. Ghost: the real class, with only the key filled in until first use.
    static class GhostCustomer implements Customer {
        private final int id;
        private String name;
        private String city;
        private boolean loaded = false;

        GhostCustomer(int id) {
            this.id = id;
        }

        private void load() {
            if (!loaded) {
                String[] row = selectCustomer(id);
                name = row[0];
                city = row[1];
                loaded = true;
            }
        }

        public int id() { return id; }
        public String name() { load(); return name; }
        public String city() { load(); return city; }
    }

    public static void main(String[] args) {
        selects = 0;
        Order order = new Order(10, 1);
        String line = "lazy initialization: selects " + selects + " | " + order.customer().name();
        System.out.println(line + " | selects " + selects + " | " + order.customer().name() + " | selects " + selects);

        selects = 0;
        Customer proxy = new CustomerProxy(2);
        line = "virtual proxy: id " + proxy.id() + " | selects " + selects + " | " + proxy.name();
        System.out.println(line + " | selects " + selects + " | " + proxy.city() + " | selects " + selects);

        selects = 0;
        ValueHolder<Customer> holder = new ValueHolder<>(() -> RealCustomer.load(1));
        System.out.println("value holder: selects " + selects + " | " + holder.getValue().name() + " | selects " + selects);

        selects = 0;
        Customer ghost = new GhostCustomer(1);
        line = "ghost: id " + ghost.id() + " | selects " + selects + " | " + ghost.name() + " " + ghost.city();
        System.out.println(line + " | selects " + selects);

        selects = 1; // One SELECT reads the orders.
        List<String> names = new ArrayList<>();
        for (int[] row : ORDERS) names.add(new Order(row[0], row[1]).customer().name());
        System.out.println("lazy: " + String.join(", ", names) + " | selects " + selects);

        selects = 1; // One SELECT with a JOIN reads the orders and their customers.
        names.clear();
        for (int[] row : ORDERS) names.add(CUSTOMERS.get(row[1])[0]);
        System.out.println("eager: " + String.join(", ", names) + " | selects " + selects);
    }
}
```

```output
lazy initialization: selects 0 | Ann | selects 1 | Ann | selects 1
virtual proxy: id 2 | selects 0 | Bo | selects 1 | Rome | selects 1
value holder: selects 0 | Ann | selects 1
ghost: id 1 | selects 0 | Ann Oslo | selects 1
lazy: Ann, Bo, Ann | selects 4
eager: Ann, Bo, Ann | selects 1
```

- Verdict: idiom. `Customer` is an interface, so `RealCustomer`, `CustomerProxy` and `GhostCustomer` can stand in for one another. The value holder takes a `Supplier`.
- A `Map` stands in for the customer table, and `selectCustomer` counts the statements. Java evaluates the parts of a string concatenation from left to right, so each line shows the count before and after the load.
