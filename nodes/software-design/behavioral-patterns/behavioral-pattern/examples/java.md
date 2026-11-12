```java run id=kinds
import java.util.List;
import java.util.function.IntUnaryOperator;

public class Main {
    static abstract class Checkout {
        int total(List<Integer> prices) {
            return discount(prices.stream().mapToInt(Integer::intValue).sum());
        }
        abstract int discount(int amount);
    }

    static class SaleCheckout extends Checkout {
        int discount(int amount) { return amount * 9 / 10; }
    }

    static class Till {
        IntUnaryOperator discount;
        Till(IntUnaryOperator discount) { this.discount = discount; }
        int total(List<Integer> prices) {
            return discount.applyAsInt(prices.stream().mapToInt(Integer::intValue).sum());
        }
    }

    public static void main(String[] args) {
        List<Integer> prices = List.of(40, 60);
        System.out.println("class pattern: " + new SaleCheckout().total(prices));
        Till till = new Till(amount -> amount * 9 / 10);
        System.out.println("object pattern: " + till.total(prices));
        till.discount = amount -> amount - 20;
        System.out.println("changed at run time: " + till.total(prices));
    }
}
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: idiom. The strategy has the functional interface type `IntUnaryOperator`, and a lambda gives its one method.
