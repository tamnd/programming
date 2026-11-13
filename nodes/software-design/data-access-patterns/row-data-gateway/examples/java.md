```java run id=product-rows
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    // A stand-in for one database table: the stored columns by id, and an id counter.
    static final class Table {
        final TreeMap<Integer, Object[]> rows = new TreeMap<>();
        int nextId = 1;
    }

    // A gateway to one row: the columns are fields, and the row saves itself.
    static final class ProductRow {
        private final Table table;
        int id;
        String name;
        int price;
        int stock;

        ProductRow(Table table, int id, String name, int price, int stock) {
            this.table = table;
            this.id = id;
            this.name = name;
            this.price = price;
            this.stock = stock;
        }

        int insert() {
            id = table.nextId++;
            update();
            return id;
        }

        void update() {
            table.rows.put(id, new Object[] {name, price, stock});
        }

        void delete() {
            table.rows.remove(id);
        }

        @Override
        public String toString() {
            return id + " " + name + " " + price + " " + stock;
        }
    }

    // Reads rows and makes one ProductRow for each of them.
    static final class ProductFinder {
        private final Table table;

        ProductFinder(Table table) {
            this.table = table;
        }

        private ProductRow load(int id, Object[] c) {
            return new ProductRow(table, id, (String) c[0], (Integer) c[1], (Integer) c[2]);
        }

        Optional<ProductRow> find(int id) {
            return Optional.ofNullable(table.rows.get(id)).map(c -> load(id, c));
        }

        List<ProductRow> findAll() {
            return table.rows.entrySet().stream().map(e -> load(e.getKey(), e.getValue())).toList();
        }

        List<ProductRow> findLowStock(int limit) {
            return findAll().stream().filter(p -> p.stock < limit).toList();
        }
    }

    static List<String> restock(ProductFinder finder, int limit, int amount) {
        List<String> done = new ArrayList<>();
        for (ProductRow product : finder.findLowStock(limit)) {
            int before = product.stock;
            product.stock += amount;
            product.update();
            done.add(product.name + " " + before + " -> " + product.stock);
        }
        return done;
    }

    public static void main(String[] args) {
        Table table = new Table();
        new ProductRow(table, 0, "pen", 120, 3).insert();
        int ink = new ProductRow(table, 0, "ink", 450, 40).insert();
        new ProductRow(table, 0, "pad", 300, 0).insert();
        ProductFinder finder = new ProductFinder(table);
        System.out.println("restocked: " + String.join("; ", restock(finder, 5, 20)));
        finder.find(ink).ifPresent(ProductRow::delete);
        System.out.println("rows: " + finder.findAll().stream().map(ProductRow::toString).collect(Collectors.joining(" | ")));
        System.out.println("find(9): " + finder.find(9).map(ProductRow::toString).orElse("none"));
    }
}
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: 1 pen 120 23 | 3 pad 300 20
find(9): none
```

- Verdict: idiom. The gateway has mutable fields, one per column, because a row gateway is changed in place and then saved.
- A `TreeMap` from id to the stored columns stands in for the table, because the test image has no database driver.
