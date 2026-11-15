```java run id=logical-and-physical-layers
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class Main {
    // The error that the application kernel sees, whatever the database says.
    static class AccessError extends RuntimeException {
        AccessError(String m) { super(m); }
    }

    // ---- Physical access layer: knows the tables. ----

    // The example has no database: Database holds tables as lists of rows.
    static class Database {
        final Map<String, List<Map<String, Object>>> tables = new LinkedHashMap<>();

        List<Map<String, Object>> rows(String table) { return tables.get(table); }

        void insert(String table, Object... kv) {
            Map<String, Object> row = new LinkedHashMap<>();
            for (int i = 0; i < kv.length; i += 2) row.put((String) kv[i], kv[i + 1]);
            tables.computeIfAbsent(table, t -> new ArrayList<>()).add(row);
        }

        void commit() {}
    }

    static Database normalized() {
        Database db = new Database();
        db.insert("customer", "id", 1, "name", "Acme Ltd");
        db.insert("orders", "id", 10, "customer_id", 1);
        db.insert("order_line", "order_id", 10, "item", "bolt", "qty", 2);
        db.insert("order_line", "order_id", 10, "item", "nut", "qty", 5);
        return db;
    }

    // After tuning: the customer name is copied into the orders table.
    static Database denormalized() {
        Database db = new Database();
        db.insert("orders", "id", 10, "customer_name", "Acme Ltd");
        db.insert("order_line", "order_id", 10, "item", "bolt", "qty", 2);
        db.insert("order_line", "order_id", 10, "item", "nut", "qty", 5);
        return db;
    }

    record Read(String customer, Map<String, Integer> lines) {}

    // The protocol of a physical view of orders: read an order, write a line.
    abstract static class OrderView {
        final Database db;

        OrderView(Database db) { this.db = db; }

        abstract String tables();

        abstract Read read(int orderId);

        Map<String, Integer> lines(int orderId) {
            Map<String, Integer> lines = new TreeMap<>();
            for (Map<String, Object> r : db.rows("order_line")) {
                if (r.get("order_id").equals(orderId)) lines.put((String) r.get("item"), (Integer) r.get("qty"));
            }
            return lines;
        }

        void writeLine(int orderId, String item, int qty) {
            for (Map<String, Object> r : db.rows("order_line")) {
                if (r.get("order_id").equals(orderId) && r.get("item").equals(item)) r.put("qty", qty);
            }
        }
    }

    // Reads an order from the normalized tables.
    static class NormalizedOrderView extends OrderView {
        NormalizedOrderView(Database db) { super(db); }

        String tables() { return "customer, orders, order_line"; }

        Read read(int orderId) {
            for (Map<String, Object> o : db.rows("orders")) {
                if (!o.get("id").equals(orderId)) continue;
                for (Map<String, Object> c : db.rows("customer")) {
                    if (c.get("id").equals(o.get("customer_id"))) return new Read((String) c.get("name"), lines(orderId));
                }
            }
            return null;
        }
    }

    // Reads an order from the tuned tables: one table less to read.
    static class DenormalizedOrderView extends OrderView {
        DenormalizedOrderView(Database db) { super(db); }

        String tables() { return "orders, order_line"; }

        Read read(int orderId) {
            for (Map<String, Object> o : db.rows("orders")) {
                if (o.get("id").equals(orderId)) return new Read((String) o.get("customer_name"), lines(orderId));
            }
            return null;
        }
    }

    // ---- Logical access layer: the interface of the application kernel. ----

    // The logical view of an order. It knows nothing of tables.
    static class Order {
        final int id;
        final String customer;
        final Map<String, Integer> lines;
        final TreeSet<String> changed = new TreeSet<>();

        Order(int id, String customer, Map<String, Integer> lines) {
            this.id = id;
            this.customer = customer;
            this.lines = new TreeMap<>(lines);
        }

        void setQty(String item, int qty) {
            lines.put(item, qty);
            changed.add(item);
        }
    }

    static class AccessLayer {
        final OrderView physical;
        Map<Integer, Order> cache = new LinkedHashMap<>();
        int reads;

        AccessLayer(OrderView physical) { this.physical = physical; }

        void start() {
            cache = new LinkedHashMap<>();
            reads = 0;
        }

        Order getOrder(int orderId) {
            if (!cache.containsKey(orderId)) {
                Read r = physical.read(orderId);
                reads++;
                if (r == null) throw new AccessError("no order " + orderId);
                cache.put(orderId, new Order(orderId, r.customer(), r.lines()));
            }
            return cache.get(orderId);
        }

        List<String> commit() {
            List<String> written = new ArrayList<>();
            for (Order o : cache.values()) {
                for (String item : o.changed) {
                    physical.writeLine(o.id, item, o.lines.get(item));
                    written.add("order " + o.id + " " + item + " x " + o.lines.get(item));
                }
                o.changed.clear();
            }
            physical.db.commit();
            return written;
        }
    }

    // ---- Application kernel: uses only the logical access layer. ----

    static String invoice(AccessLayer layer, int orderId) {
        Order o = layer.getOrder(orderId);
        List<String> parts = new ArrayList<>(List.of(o.customer));
        o.lines.forEach((item, qty) -> parts.add(item + " x " + qty));
        return String.join(" | ", parts);
    }

    static void run(String name, OrderView view) {
        AccessLayer layer = new AccessLayer(view);
        System.out.println("physical layout: " + name);
        layer.start();
        System.out.println("invoice 10: " + invoice(layer, 10));
        System.out.println("invoice 10: " + invoice(layer, 10));
        System.out.println("physical reads: " + layer.reads + ", tables " + view.tables());
        layer.getOrder(10).setQty("bolt", 3);
        System.out.println("commit: " + String.join(", ", layer.commit()));
        layer.start();
        System.out.println("invoice 10 in a new transaction: " + invoice(layer, 10));
        try {
            invoice(layer, 99);
        } catch (AccessError e) {
            System.out.println("error: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        run("normalized", new NormalizedOrderView(normalized()));
        run("denormalized", new DenormalizedOrderView(denormalized()));
    }
}
```

```output
physical layout: normalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables customer, orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
physical layout: denormalized
invoice 10: Acme Ltd | bolt x 2 | nut x 5
invoice 10: Acme Ltd | bolt x 2 | nut x 5
physical reads: 1, tables orders, order_line
commit: order 10 bolt x 3
invoice 10 in a new transaction: Acme Ltd | bolt x 3 | nut x 5
error: no order 99
```

- Verdict: idiom. `OrderView` is an abstract class with the shared line access, and each layout is a subclass. In a JDBC application, the physical views would hold the `PreparedStatement` code, and `Database` would hold the `Connection`.
- `AccessError` is an unchecked exception, as Spring's `DataAccessException` is.
