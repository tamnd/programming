```java run id=hide-the-storage
import java.util.Map;

public class Main {
    // After: a record. The compact constructor checks the data once.
    record Product(String sku, int priceCents, String currency) {
        Product {
            if (priceCents < 0) throw new IllegalArgumentException("bad price " + priceCents);
        }
        String priceText() { // derived, not stored
            return String.format("%d.%02d %s", priceCents / 100, priceCents % 100, currency);
        }
    }

    public static void main(String[] args) {
        // Before: a map, read by key, with a cast at each use.
        Map<String, Object> raw = Map.of("sku", "T-100", "priceCents", 1999, "currency", "EUR");
        int cents = (Integer) raw.get("priceCents");
        String before = String.format("%d.%02d %s", cents / 100, cents % 100, raw.get("currency"));

        Product p = new Product((String) raw.get("sku"), cents, (String) raw.get("currency"));
        if (!p.priceText().equals(before)) throw new AssertionError("behaviour changed");
        try {
            new Product("T-101", -1, "EUR");
            throw new AssertionError("expected the check to fail");
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
        }
        System.out.println(p); // records generate toString, equals and hashCode
        System.out.println("encapsulate record: tests pass");
    }
}
```

```output
bad price -1
Product[sku=T-100, priceCents=1999, currency=EUR]
encapsulate record: tests pass
```
