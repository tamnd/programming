```java run id=map-a-customer
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    // The tables: each row is an array of column values.
    static final Object[][] CUSTOMER = {
        {1, "Ann", "Main St 1", "Oslo"},
        {2, "Bo", "High St 9", "Rome"},
    };
    static final Object[][] ORDERS = { // id, customer_id, total, kind, card
        {10, 1, 30, "card", "4242"},
        {11, 1, 20, "cash", null},
        {12, 2, 45, "card", "1881"},
    };
    static int statements = 0;

    // Each method stands in for one SQL statement.
    static Object[] selectCustomer(int id) {
        statements++;
        for (Object[] r : CUSTOMER) if ((int) r[0] == id) return r;
        return null;
    }

    static List<Object[]> selectOrders(int customerId) {
        statements++;
        List<Object[]> out = new ArrayList<>();
        for (Object[] r : ORDERS) if ((int) r[1] == customerId) out.add(r);
        return out;
    }

    static List<Integer> selectCustomerIds() {
        statements++;
        List<Integer> out = new ArrayList<>();
        for (Object[] r : CUSTOMER) out.add((int) r[0]);
        return out;
    }

    static List<String> joinTotals() { // select name, sum(total) ... group by
        statements++;
        List<String> out = new ArrayList<>();
        for (Object[] c : CUSTOMER) {
            int sum = 0;
            for (Object[] o : ORDERS) if (o[1].equals(c[0])) sum += (int) o[2];
            out.add(c[1] + " " + sum);
        }
        return out;
    }

    record Address(String street, String city) {} // A value object: no table, no key.

    static class Customer {
        final int id;
        final String name;
        final Address address;
        final List<Order> orders = new ArrayList<>();

        Customer(int id, String name, Address address) {
            this.id = id;
            this.name = name;
            this.address = address;
        }
    }

    static class Order {
        final int id;
        final int total;
        Customer customer;

        Order(int id, int total, Customer customer) {
            this.id = id;
            this.total = total;
            this.customer = customer;
        }
    }

    static class CardOrder extends Order {
        final String card;

        CardOrder(int id, int total, Customer customer, String card) {
            super(id, total, customer);
            this.card = card;
        }
    }

    static class CashOrder extends Order {
        CashOrder(int id, int total, Customer customer) {
            super(id, total, customer);
        }
    }

    static Customer load(int id) {
        Object[] r = selectCustomer(id);
        // Granularity: one row becomes two objects.
        Customer c = new Customer(id, (String) r[1], new Address((String) r[2], (String) r[3]));
        for (Object[] o : selectOrders(id)) {
            // Subtypes: a column chooses the class.
            Order order = o[3].equals("card")
                ? new CardOrder((int) o[0], (int) o[2], c, (String) o[4])
                : new CashOrder((int) o[0], (int) o[2], c);
            // Associations: the foreign key becomes two references.
            c.orders.add(order);
        }
        return c;
    }

    static String yn(boolean flag) {
        return flag ? "yes" : "no";
    }

    public static void main(String[] args) {
        Customer ann = load(1), bo = load(2);
        Address a = ann.address;
        System.out.println("granularity: customer row 1 -> Customer " + ann.name
            + " + Address " + a.street() + ", " + a.city());
        String kinds = ann.orders.stream()
            .map(o -> "order " + o.id + " " + o.getClass().getSimpleName())
            .collect(Collectors.joining(" | "));
        System.out.println("subtypes: " + kinds + " | card " + ((CardOrder) ann.orders.get(0)).card);
        String ids = ann.orders.stream().map(o -> String.valueOf(o.id)).collect(Collectors.joining(", "));
        System.out.println("associations: order 10 -> " + ann.orders.get(0).customer.name
            + " | Ann -> orders " + ids);

        // In the table, one update moves the order. In memory, both sides must change.
        Order moved = ann.orders.get(1);
        moved.customer = bo;
        System.out.println("moved order 11 on one side: Ann lists 11: " + yn(ann.orders.contains(moved))
            + " | Bo lists 11: " + yn(bo.orders.contains(moved)));

        Customer again = load(1);
        System.out.println("identity: same object: " + yn(again == ann) + " | same key: " + yn(again.id == ann.id));

        statements = 0;
        List<String> walk = new ArrayList<>();
        for (int id : selectCustomerIds()) {
            Customer c = load(id);
            walk.add(c.name + " " + c.orders.stream().mapToInt(o -> o.total).sum());
        }
        System.out.println("navigation: walk " + String.join(", ", walk) + " in " + statements + " statements");

        statements = 0;
        List<String> join = joinTotals();
        System.out.println("navigation: join " + String.join(", ", join) + " in " + statements + " statement");
    }
}
```

```output
granularity: customer row 1 -> Customer Ann + Address Main St 1, Oslo
subtypes: order 10 CardOrder | order 11 CashOrder | card 4242
associations: order 10 -> Ann | Ann -> orders 10, 11
moved order 11 on one side: Ann lists 11: yes | Bo lists 11: no
identity: same object: no | same key: yes
navigation: walk Ann 50, Bo 45 in 5 statements
navigation: join Ann 50, Bo 45 in 1 statement
```

Two arrays stand in for the tables, and each `select` method counts as one SQL statement. `List.contains` calls `equals`, and `Order` does not override it, so it compares references. The second `load(1)` gives a new `Customer`, so `again == ann` is false although the keys are equal.
