```java run id=product-mapper
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.TreeMap;

public class Main {
    // A domain object. It has no storage code and no idea that a database exists.
    static final class Product {
        Integer id;
        final String name;
        final int price;
        int stock;
        final List<String> tags;

        Product(String name, int price, int stock, List<String> tags) {
            this.name = name;
            this.price = price;
            this.stock = stock;
            this.tags = new ArrayList<>(tags);
        }

        int sell(int quantity) {
            if (quantity > stock) throw new IllegalStateException(name + ": only " + stock + " in stock");
            stock -= quantity;
            return quantity * price;
        }

        @Override
        public String toString() {
            return id + " " + name + " " + price + " " + stock + " [" + String.join(", ", tags) + "]";
        }
    }

    // A stand-in for the database, because the test image has no database driver.
    // Two tables with their own column names: products and product_tags.
    record ProductRow(String title, int priceCents, int qty) {}
    record TagRow(int productId, String tag) {}

    static final class Database {
        final TreeMap<Integer, ProductRow> products = new TreeMap<>();
        final List<TagRow> productTags = new ArrayList<>();
        int nextId = 1;
    }

    // The mapper moves products between objects and the two tables.
    static final class ProductMapper {
        private final Database db;

        ProductMapper(Database db) {
            this.db = db;
        }

        Optional<Product> find(int id) {
            ProductRow row = db.products.get(id);
            if (row == null) return Optional.empty();
            List<String> tags = db.productTags.stream().filter(t -> t.productId() == id).map(TagRow::tag).sorted().toList();
            Product p = new Product(row.title(), row.priceCents(), row.qty(), tags);
            p.id = id;
            return Optional.of(p);
        }

        void insert(Product p) {
            p.id = db.nextId++;
            update(p);
        }

        void update(Product p) {
            db.products.put(p.id, new ProductRow(p.name, p.price, p.stock));
            deleteTags(p.id);
            for (String tag : p.tags) db.productTags.add(new TagRow(p.id, tag));
        }

        void delete(Product p) {
            deleteTags(p.id);
            db.products.remove(p.id);
        }

        private void deleteTags(int id) {
            db.productTags.removeIf(t -> t.productId() == id);
        }
    }

    public static void main(String[] args) {
        Database db = new Database();
        ProductMapper mapper = new ProductMapper(db);
        mapper.insert(new Product("pen", 120, 3, List.of("office")));
        Product ink = new Product("ink", 450, 40, List.of("office"));
        mapper.insert(ink);

        Product pen = mapper.find(1).orElseThrow();
        System.out.println("sold: " + pen.sell(2));
        pen.tags.add("sale");
        mapper.update(pen);
        System.out.println("object: " + mapper.find(1).orElseThrow());
        List<String> rows = new ArrayList<>();
        db.products.forEach((id, r) -> rows.add(id + " " + r.title() + " " + r.priceCents() + " " + r.qty()));
        System.out.println("products: " + String.join(" | ", rows));
        List<String> tagRows = db.productTags.stream().map(t -> t.productId() + " " + t.tag()).sorted().toList();
        System.out.println("product_tags: " + String.join(" | ", tagRows));
        mapper.delete(ink);
        System.out.println("after delete: " + db.products.size() + " product, " + db.productTags.size() + " tags");
        System.out.println("find(2): " + mapper.find(2).map(Product::toString).orElse("none"));
    }
}
```

```output
sold: 240
object: 1 pen 120 1 [office, sale]
products: 1 pen 120 1 | 2 ink 450 40
product_tags: 1 office | 1 sale | 2 office
after delete: 1 product, 2 tags
find(2): none
```

- Verdict: idiom. `Product` has no import of any storage type, and `find` returns an `Optional`, because a row may not exist.
- Two records, `ProductRow` and `TagRow`, are the stored rows of the stand-in tables, because the test image has no database driver.
