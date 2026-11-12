```java run id=extract-the-promotion
import java.util.List;

public class Main {
    record Item(int cents, int qty) {}

    static int sumCents(List<Item> items) {
        return items.stream().mapToInt(i -> i.cents() * i.qty()).sum();
    }

    // Before: the promotion fields are null for most orders.
    static final class OrderBefore {
        private final List<Item> items;
        private String promoCode;
        private int promoPercent;
        private String promoUntil;

        OrderBefore(List<Item> items) { this.items = items; }

        void applyPromo(String code, int percent, String until) {
            promoCode = code;
            promoPercent = percent;
            promoUntil = until;
        }

        int total(String day) {
            int sum = sumCents(items);
            if (promoCode != null && day.compareTo(promoUntil) <= 0) {
                return sum - Math.round(sum * promoPercent / 100f);
            }
            return sum;
        }

        String label() { return promoCode == null ? "no promotion" : "promotion " + promoCode; }
    }

    // After: an interface with one record for the real case and one constant for the empty case.
    sealed interface Promotion permits Discount, NoPromotion {
        int apply(int sum, String day);
        String label();
    }

    record Discount(String code, int percent, String until) implements Promotion {
        public int apply(int sum, String day) {
            return day.compareTo(until) <= 0 ? sum - Math.round(sum * percent / 100f) : sum;
        }
        public String label() { return "promotion " + code; }
    }

    enum NoPromotion implements Promotion {
        INSTANCE;
        public int apply(int sum, String day) { return sum; }
        public String label() { return "no promotion"; }
    }

    record Order(List<Item> items, Promotion promotion) {
        Order(List<Item> items) { this(items, NoPromotion.INSTANCE); }
        int total(String day) { return promotion.apply(sumCents(items), day); }
        String label() { return promotion.label(); }
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        List<Item> items = List.of(new Item(1250, 2), new Item(499, 1));
        OrderBefore plainBefore = new OrderBefore(items);
        OrderBefore promoBefore = new OrderBefore(items);
        promoBefore.applyPromo("SPRING", 10, "2026-04-30");
        Order plain = new Order(items);
        Order promo = new Order(items, new Discount("SPRING", 10, "2026-04-30"));

        for (String day : List.of("2026-04-01", "2026-05-01")) {
            check(plain.total(day) == plainBefore.total(day), "plain " + day);
            check(promo.total(day) == promoBefore.total(day), "promo " + day);
        }
        check(promo.total("2026-04-01") == 2699, "discount");
        check(promo.label().equals(promoBefore.label()), "label");
        check(plain.label().equals("no promotion"), "empty label");
        System.out.println("promotion: tests pass");
    }
}
```

```output
promotion: tests pass
```

- The `sealed` interface lists the two cases. A `switch` over a `Promotion` can then cover both without a default branch.
- `NoPromotion` is an enum with one constant, which is a common Java form for a special case object that has no state.
- The `Order` record has no field that can be null. Its second constructor supplies the empty case.
