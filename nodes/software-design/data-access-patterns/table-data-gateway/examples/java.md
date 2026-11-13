```java run id=products-gateway
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    // One row of the products table, as plain data.
    record ProductRow(int id, String name, int price, int stock) {}

    // A stand-in for one database table: rows by id and an id counter.
    static final class Table {
        final TreeMap<Integer, ProductRow> rows = new TreeMap<>();
        int nextId = 1;
    }

    // The gateway holds every operation on the products table.
    static final class ProductGateway {
        private final Table table;

        ProductGateway(Table table) {
            this.table = table;
        }

        Optional<ProductRow> find(int id) {
            return Optional.ofNullable(table.rows.get(id));
        }

        List<ProductRow> findAll() {
            return List.copyOf(table.rows.values());
        }

        List<ProductRow> findLowStock(int limit) {
            return table.rows.values().stream().filter(r -> r.stock() < limit).toList();
        }

        int insert(String name, int price, int stock) {
            int id = table.nextId++;
            table.rows.put(id, new ProductRow(id, name, price, stock));
            return id;
        }

        void updateStock(int id, int stock) {
            table.rows.computeIfPresent(id, (k, r) -> new ProductRow(r.id(), r.name(), r.price(), stock));
        }

        void delete(int id) {
            table.rows.remove(id);
        }
    }

    // A transaction script: the rule is here, the data access is in the gateway.
    static List<String> restock(ProductGateway products, int limit, int amount) {
        List<String> done = new ArrayList<>();
        for (ProductRow row : products.findLowStock(limit)) {
            products.updateStock(row.id(), row.stock() + amount);
            done.add(row.name() + " " + row.stock() + " -> " + (row.stock() + amount));
        }
        return done;
    }

    public static void main(String[] args) {
        ProductGateway products = new ProductGateway(new Table());
        products.insert("pen", 120, 3);
        int ink = products.insert("ink", 450, 40);
        products.insert("pad", 300, 0);
        System.out.println("restocked: " + String.join("; ", restock(products, 5, 20)));
        products.delete(ink);
        System.out.println("rows: " + products.findAll().stream()
                .map(r -> r.id() + " " + r.name() + " " + r.price() + " " + r.stock())
                .collect(Collectors.joining(" | ")));
        System.out.println("find(9): " + products.find(9).map(ProductRow::toString).orElse("none"));
    }
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The gateway returns `ProductRow` records, which are immutable, so a change goes through `updateStock`.
- A `TreeMap` stands in for the table, because the test image has no database driver. The keys stay in order, so `findAll` needs no sort.
