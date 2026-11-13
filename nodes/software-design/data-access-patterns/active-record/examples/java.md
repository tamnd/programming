```java run id=product-record
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.TreeMap;

public class Main {
    // What the table stores for one row.
    record Row(String name, int price, int stock) {}

    // A stand-in for the products table: rows by id, with an id counter.
    // The test image has no database driver.
    static final class Table {
        final TreeMap<Integer, Row> rows = new TreeMap<>();
        int nextId = 1;
    }

    // An Active Record: one object per row, with the data access and the domain logic together.
    static final class Product {
        static final Table TABLE = new Table();

        Integer id;
        String name;
        int price;
        int stock;

        Product(Integer id, String name, int price, int stock) {
            this.id = id;
            this.name = name;
            this.price = price;
            this.stock = stock;
        }

        // Data access: the finders are static, and saving is an instance method.
        static Optional<Product> find(int id) {
            Row r = TABLE.rows.get(id);
            return r == null ? Optional.empty() : Optional.of(new Product(id, r.name(), r.price(), r.stock()));
        }

        static List<Product> all() {
            List<Product> out = new ArrayList<>();
            for (int id : TABLE.rows.keySet()) out.add(find(id).orElseThrow());
            return out;
        }

        void validate() {
            if (stock < 0) throw new IllegalStateException(name + ": stock cannot be negative");
        }

        void save() {
            validate();
            if (id == null) id = TABLE.nextId++;
            TABLE.rows.put(id, new Row(name, price, stock));
        }

        void delete() {
            TABLE.rows.remove(id);
        }

        // Domain logic on the same object.
        boolean needsRestock() {
            return stock < 5;
        }

        int sell(int quantity) {
            if (quantity > stock) throw new IllegalStateException(name + ": only " + stock + " in stock");
            stock -= quantity;
            return quantity * price;
        }

        @Override
        public String toString() {
            return id + " " + name + " " + stock;
        }
    }

    public static void main(String[] args) {
        new Product(null, "pen", 120, 3).save();
        new Product(null, "ink", 450, 40).save();
        new Product(null, "pad", 300, 0).save();

        Product pen = Product.find(1).orElseThrow();
        System.out.println("sold 2 pens for " + pen.sell(2));
        pen.save();
        List<String> restocked = new ArrayList<>();
        for (Product product : Product.all()) {
            if (product.needsRestock()) {
                product.stock += 20;
                product.save();
                restocked.add(product.name);
            }
        }
        System.out.println("restocked: " + String.join(", ", restocked));
        List<String> rows = new ArrayList<>();
        for (Product product : Product.all()) rows.add(product.toString());
        System.out.println("rows: " + String.join(" | ", rows));
        Product ink = Product.find(2).orElseThrow();
        ink.stock -= 45;
        try {
            ink.save();
        } catch (IllegalStateException e) {
            System.out.println("not saved: " + e.getMessage());
        }
        System.out.println("find(9): " + Product.find(9).map(Product::toString).orElse("none"));
    }
}
```

```output
sold 2 pens for 240
restocked: pen, pad
rows: 1 pen 21 | 2 ink 40 | 3 pad 20
not saved: ink: stock cannot be negative
find(9): none
```

- Verdict: idiom. The finders are static methods and `find` returns an `Optional`, because a row may not exist.
- A `TreeMap` from id to a `Row` record stands in for the table, because the test image has no database driver.
