```java run id=checkout
import java.util.List;

public class Main {
    record Item(String name, int price, int quantity) {}

    @FunctionalInterface
    interface Pricing {
        int total(List<Item> items);
    }

    static final class Checkout {
        private Pricing pricing;

        Checkout(Pricing pricing) {
            this.pricing = pricing;
        }

        void setPricing(Pricing pricing) {
            this.pricing = pricing;
        }

        int total(List<Item> items) {
            return pricing.total(items);
        }
    }

    static int regular(List<Item> items) {
        return items.stream().mapToInt(i -> i.price() * i.quantity()).sum();
    }

    static int tenPercentOff(List<Item> items) {
        return regular(items) * 90 / 100;
    }

    // 50 cents off each unit of a line with 5 or more units.
    static int bulk(List<Item> items) {
        return items.stream()
                .mapToInt(i -> (i.quantity() >= 5 ? i.price() - 50 : i.price()) * i.quantity())
                .sum();
    }

    record Named(String name, Pricing pricing) {}

    public static void main(String[] args) {
        List<Item> cart = List.of(new Item("book", 1500, 2), new Item("pen", 200, 5));
        List<Named> strategies = List.of(
                new Named("regular", Main::regular),
                new Named("ten percent off", Main::tenPercentOff),
                new Named("bulk", Main::bulk));
        Checkout checkout = new Checkout(Main::regular);
        for (Named s : strategies) {
            checkout.setPricing(s.pricing());
            System.out.println(s.name() + ": " + checkout.total(cart) + " cents");
        }
    }
}
```

```output
regular: 4000 cents
ten percent off: 3600 cents
bulk: 3750 cents
```

- Verdict: idiom. `Pricing` is a functional interface: it has one abstract method, so a lambda or a method reference such as `Main::bulk` is an instance of it.
- `@FunctionalInterface` makes the compiler reject a second abstract method. It is optional.
- `java.util.function.ToIntFunction<List<Item>>` would also work. A named interface gives the strategy a name in the type of the context and in stack traces.
- `java.util.Comparator` is the strategy of the JDK for order. `TreeMap`, `List.sort` and `Collections.sort` take one.
