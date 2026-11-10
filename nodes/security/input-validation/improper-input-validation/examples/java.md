```java wrong id=parse-int
import java.util.Map;

public class Main {
    static final Map<String, Integer> PRICES = Map.of("pen", 150, "book", 1200); // in cents

    static int orderTotal(String product, String quantity) {
        return PRICES.get(product) * Integer.parseInt(quantity);
    }

    public static void main(String[] args) {
        System.out.println(orderTotal("book", "2"));
        System.out.println(orderTotal("book", "-3"));
        System.out.println(orderTotal("book", "٣")); // ARABIC-INDIC DIGIT THREE
    }
}
```

```output for=parse-int
2400
-3600
3600
```

```java run id=record
import java.util.Map;
import java.util.regex.Pattern;

public class Main {
    static final Map<String, Integer> PRICES = Map.of("pen", 150, "book", 1200); // in cents
    static final Pattern QUANTITY = Pattern.compile("[0-9]{1,3}");

    record Order(String product, int quantity) {
        Order {
            if (!PRICES.containsKey(product)) {
                throw new IllegalArgumentException("unknown product");
            }
            if (quantity < 1 || quantity > 100) {
                throw new IllegalArgumentException("quantity must be from 1 to 100");
            }
        }

        static Order parse(String product, String quantity) {
            if (!QUANTITY.matcher(quantity).matches()) {
                throw new IllegalArgumentException("quantity must be 1 to 3 ASCII digits");
            }
            return new Order(product, Integer.parseInt(quantity));
        }

        int total() {
            return PRICES.get(product) * quantity;
        }
    }

    public static void main(String[] args) {
        String[][] inputs = {{"book", "2"}, {"book", "-3"}, {"book", "٣"}, {"book", "0"}, {"boat", "1"}};
        for (String[] in : inputs) {
            try {
                System.out.println(Order.parse(in[0], in[1]).total());
            } catch (IllegalArgumentException e) {
                System.out.println("rejected: " + e.getMessage());
            }
        }
    }
}
```

```output for=record
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be from 1 to 100
rejected: unknown product
```

- Verdict: stdlib. `java.util.regex` checks the form, `Integer.parseInt` converts, and a record with a compact constructor keeps the range rule.
- `Integer.parseInt` accepts every character for which `Character.digit` returns a value, so it accepts digits from other scripts, such as `٣`. It also accepts a leading `+` or `-`.
- `Matcher.matches` requires the whole input to match. `find` searches for a match anywhere, which is not a check of the whole value.
- The compact constructor of the record runs for every `new Order(...)`, so no code can make an `Order` with a quantity of `0`, even code that does not call `parse`.
- Jakarta Bean Validation adds annotations such as `@Min` and `@Pattern` for the same rules. It is a library, not part of Java SE.
