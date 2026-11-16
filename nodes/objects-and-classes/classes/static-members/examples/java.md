```java run id=static-fields-and-methods
public class Main {
  static final class Order {
    static final double TAX_RATE = 0.2; // a constant: static and final
    private static int created = 0; // a class variable, one for the class

    private final int id; // an instance variable, one per order
    private final int amount;

    Order(int amount) {
      created++;
      this.id = created;
      this.amount = amount;
    }

    static Order fromText(String text) { // a static factory method; no this
      return new Order(Integer.parseInt(text));
    }

    static int created() {
      return created;
    }

    double total() {
      return amount * (1 + TAX_RATE);
    }
  }

  public static void main(String[] args) {
    Order a = new Order(100);
    Order b = Order.fromText("50");
    System.out.println(a.id + " " + b.id + " " + Order.created());
    System.out.println(a.total() + " " + Order.TAX_RATE);
  }
}
```

```output for=static-fields-and-methods
1 2 2
120.0 0.2
```

- Verdict: native. A `static` field is a class variable and a `static` method is a class method. Code calls them on the class name: `Order.created()`, `Order.TAX_RATE`.
- A static method has no `this`, so `fromText` cannot read `id` or `amount`; it makes an instance and returns it. `main` is itself a static method, because the program has no `Main` object when it starts.
- A static method is not a Python class method: it does not get the class as a parameter, and a subclass cannot override it, only hide it.
