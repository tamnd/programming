```java run id=shop
import java.util.HashMap;
import java.util.Map;

public class Main {
    static final class Inventory {
        private final Map<String, Integer> stock;

        Inventory(Map<String, Integer> stock) {
            this.stock = new HashMap<>(stock);
        }

        boolean reserve(String item) {
            int n = stock.getOrDefault(item, 0);
            if (n == 0) {
                return false;
            }
            stock.put(item, n - 1);
            return true;
        }

        void release(String item) {
            stock.merge(item, 1, Integer::sum);
        }

        int count(String item) {
            return stock.getOrDefault(item, 0);
        }
    }

    static final class Payments {
        boolean charge(String card, double amount) {
            return !card.equals("expired");
        }
    }

    static final class Shipping {
        private int parcels;

        int schedule(String item) {
            return ++parcels;
        }
    }

    // The facade gets its subsystem objects through the constructor, so a test can pass others.
    static final class Shop {
        private final Inventory inventory;
        private final Payments payments;
        private final Shipping shipping;

        Shop(Inventory inventory, Payments payments, Shipping shipping) {
            this.inventory = inventory;
            this.payments = payments;
            this.shipping = shipping;
        }

        String placeOrder(String item, String card, double price) {
            if (!inventory.reserve(item)) {
                return "failed: " + item + " is out of stock";
            }
            if (!payments.charge(card, price)) {
                inventory.release(item);
                return "failed: card declined, " + item + " back in stock";
            }
            return String.format("ok: %s, paid %.2f, parcel %d", item, price, shipping.schedule(item));
        }

        int stock(String item) {
            return inventory.count(item);
        }
    }

    public static void main(String[] args) {
        Shop shop = new Shop(new Inventory(Map.of("book", 2, "lamp", 0)), new Payments(), new Shipping());
        System.out.println(shop.placeOrder("book", "visa", 12.50));
        System.out.println(shop.placeOrder("book", "expired", 12.50));
        System.out.println(shop.placeOrder("lamp", "visa", 30.00));
        System.out.println("stock of book: " + shop.stock("book"));
    }
}
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. `Shop` is the facade. It holds the three subsystem objects and has one method for each task that a client needs.
- The constructor takes the subsystem objects, so the code that builds the program chooses them, and a test can pass fakes. This is dependency injection, and it keeps the facade from hiding which objects it uses.
- `String.format` with `%.2f` uses the default locale. A program that must print a dot in every locale passes `Locale.ROOT`.
- A Java module can export the package of the facade and not export the packages of the subsystem. Code in other modules then cannot use the subsystem classes at all.
