```java run id=split-the-checkout
import java.util.ArrayList;
import java.util.List;

public class Main {
    // Before: one class, two jobs, and fields with prefixes.
    static final class CheckoutBefore {
        private final List<Integer> cartItems = new ArrayList<>();
        private Integer cartCoupon;
        private String shipName, shipStreet, shipCity, shipCountry;

        void addItem(String name, int cents) { cartItems.add(cents); }
        void applyCoupon(int percent) { cartCoupon = percent; }
        int cartTotal() {
            int sum = cartItems.stream().mapToInt(Integer::intValue).sum();
            return cartCoupon == null ? sum : sum - Math.round(sum * cartCoupon / 100f);
        }
        void setAddress(String name, String street, String city, String country) {
            shipName = name; shipStreet = street; shipCity = city; shipCountry = country;
        }
        String shippingLabel() { return shipName + "\n" + shipStreet + "\n" + shipCity + ", " + shipCountry; }
        boolean isDomestic() { return "NL".equals(shipCountry); }
    }

    // After: each group of fields has a class, and the checkout uses both.
    static final class Cart {
        private final List<Integer> items = new ArrayList<>();
        private Integer coupon;
        void add(String name, int cents) { items.add(cents); }
        void applyCoupon(int percent) { coupon = percent; }
        int total() {
            int sum = items.stream().mapToInt(Integer::intValue).sum();
            return coupon == null ? sum : sum - Math.round(sum * coupon / 100f);
        }
    }

    record Address(String name, String street, String city, String country) {
        String label() { return name + "\n" + street + "\n" + city + ", " + country; }
    }

    record Checkout(Cart cart, Address address) {
        boolean isDomestic() { return address.country().equals("NL"); }
    }

    public static void main(String[] args) {
        CheckoutBefore before = new CheckoutBefore();
        before.addItem("lamp", 3999);
        before.addItem("bulb", 499);
        before.applyCoupon(10);
        before.setAddress("Ada", "Keizersgracht 1", "Amsterdam", "NL");

        Cart cart = new Cart();
        cart.add("lamp", 3999);
        cart.add("bulb", 499);
        cart.applyCoupon(10);
        Checkout after = new Checkout(cart, new Address("Ada", "Keizersgracht 1", "Amsterdam", "NL"));

        if (after.cart().total() != before.cartTotal() || after.cart().total() != 4048) throw new AssertionError("total");
        if (!after.address().label().equals(before.shippingLabel())) throw new AssertionError("label");
        if (after.isDomestic() != before.isDomestic()) throw new AssertionError("domestic");
        System.out.println("checkout: tests pass");
    }
}
```

```output
checkout: tests pass
```

- `Address` is a record, so it gets its constructor, accessors, `equals` and `hashCode` from one line, and its fields are final.
- In the first version the four address fields can be `null` until `setAddress` runs. In the second version an `Address` always has all four.
