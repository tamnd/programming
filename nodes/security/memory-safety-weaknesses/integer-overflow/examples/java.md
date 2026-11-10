```java wrong id=wrap
public class Main {
    // Returns true if the order costs no more than the budget.
    static boolean orderFits(int price, int quantity, int budget) {
        int total = price * quantity; // wraps on overflow
        return total <= budget;
    }

    public static void main(String[] args) {
        System.out.println("3 items: " + orderFits(100, 3, 1000));
        System.out.println("42949673 items: " + orderFits(100, 42949673, 1000));
    }
}
```

```output for=wrap
3 items: true
42949673 items: true
```

The Java Language Specification says that an overflowing multiplication keeps the low-order bits of the product. Here they give 4, and the large order fits.

```java run id=multiply-exact
public class Main {
    // Returns true if the order costs no more than the budget.
    static boolean orderFits(int price, int quantity, int budget) {
        if (price < 0 || quantity < 0) {
            return false;
        }
        try {
            return Math.multiplyExact(price, quantity) <= budget;
        } catch (ArithmeticException e) {
            System.out.println("refused: " + e.getMessage());
            return false;
        }
    }

    public static void main(String[] args) {
        System.out.println("3 items: " + orderFits(100, 3, 1000));
        System.out.println("42949673 items: " + orderFits(100, 42949673, 1000));
    }
}
```

```output for=multiply-exact
3 items: true
refused: integer overflow
42949673 items: false
```

- Verdict: stdlib. Arithmetic on `int` and `long` wraps with no error. `Math.addExact`, `subtractExact`, `multiplyExact`, `incrementExact`, `negateExact` and `toIntExact` throw `ArithmeticException` on overflow.
- `java.math.BigInteger` has no fixed width. Use it when the values have no natural limit.
- A cast such as `(int) longValue` keeps the low-order bits. Use `Math.toIntExact` to get an exception.
