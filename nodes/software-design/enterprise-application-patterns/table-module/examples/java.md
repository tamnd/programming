```java run id=one-object-for-all-orders
import java.util.List;

public class Main {
    static class ModuleError extends RuntimeException {
        ModuleError(String message) { super(message); }
    }

    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    // The rows of the record set. In JDBC, a ResultSet fills them; in ADO.NET, a DataSet holds them.
    record CustomerRow(int id, String name, String tier) {}
    record OrderRow(int id, int customerId) {}
    record OrderLineRow(int orderId, String item, int qty, long priceCents) {}

    // The record set: an in-memory copy of the rows of several tables.
    record RecordSet(List<CustomerRow> customer, List<OrderRow> orders, List<OrderLineRow> orderLine) {}

    static RecordSet loadRecordSet() {
        return new RecordSet(
                List.of(new CustomerRow(1, "Ann", "gold"), new CustomerRow(2, "Bob", "regular"),
                        new CustomerRow(3, "Cy", "staff")),
                List.of(new OrderRow(1, 1), new OrderRow(2, 2), new OrderRow(3, 3)),
                List.of(new OrderLineRow(1, "lamp", 2, 2500), new OrderLineRow(1, "desk", 1, 7500),
                        new OrderLineRow(2, "lamp", 1, 2500), new OrderLineRow(2, "pen", 3, 500),
                        new OrderLineRow(3, "lamp", 1, 2500), new OrderLineRow(3, "pen", 3, 500)));
    }

    // One table module for each table. One instance handles all the rows of its table.
    static class CustomerModule {
        private final List<CustomerRow> rows;

        CustomerModule(RecordSet recordSet) { rows = recordSet.customer(); }

        CustomerRow row(int customerId) {
            return rows.stream().filter(r -> r.id() == customerId).findFirst()
                    .orElseThrow(() -> new ModuleError("no customer " + customerId));
        }

        List<String> namesWithTier(String tier) {
            return rows.stream().filter(r -> r.tier().equals(tier)).map(CustomerRow::name).toList();
        }
    }

    static class OrderModule {
        private final List<OrderRow> rows;
        private final List<OrderLineRow> lines;
        private final CustomerModule customers;

        OrderModule(RecordSet recordSet, CustomerModule customers) {
            rows = recordSet.orders();
            lines = recordSet.orderLine();
            this.customers = customers;
        }

        OrderRow row(int orderId) {
            return rows.stream().filter(r -> r.id() == orderId).findFirst()
                    .orElseThrow(() -> new ModuleError("no order " + orderId));
        }

        long subtotal(int orderId) {
            row(orderId);
            return lines.stream().filter(l -> l.orderId() == orderId).mapToLong(l -> l.qty() * l.priceCents()).sum();
        }

        long total(int orderId) {
            long subtotal = subtotal(orderId);
            String tier = customers.row(row(orderId).customerId()).tier();
            if (tier.equals("gold") && subtotal >= 10000) return subtotal - subtotal / 10;
            if (tier.equals("staff")) return subtotal - subtotal / 5;
            return subtotal;
        }

        long countOver(long limit) {
            return rows.stream().filter(r -> total(r.id()) > limit).count();
        }
    }

    public static void main(String[] args) {
        RecordSet recordSet = loadRecordSet();
        CustomerModule customers = new CustomerModule(recordSet);
        OrderModule orders = new OrderModule(recordSet, customers);
        System.out.println("record set: " + recordSet.customer().size() + " customers, " + recordSet.orders().size()
                + " orders, " + recordSet.orderLine().size() + " order lines");
        for (int orderId : new int[] {1, 2, 3}) {
            System.out.println("order " + orderId + ": subtotal " + cents(orders.subtotal(orderId)) + ", total "
                    + cents(orders.total(orderId)));
        }
        System.out.println("gold customers: " + String.join(", ", customers.namesWithTier("gold")));
        System.out.println("orders over 35.00: " + orders.countOver(3500));
        try {
            orders.total(9);
        } catch (ModuleError e) {
            System.out.println("error: " + e.getMessage());
        }
    }
}
```

```output
record set: 3 customers, 3 orders, 6 order lines
order 1: subtotal 125.00, total 112.50
order 2: subtotal 40.00, total 40.00
order 3: subtotal 40.00, total 32.00
gold customers: Ann
orders over 35.00: 2
error: no order 9
```

- Verdict: idiom. Records hold the rows, and `RecordSet` holds the lists. In a Java application, a JDBC `ResultSet` or a `CachedRowSet` would give the rows. In C#, an ADO.NET `DataSet` holds the tables of a record set.
