```java run id=invoice
import java.util.List;

public class Main {
    record Item(String name, int price, int quantity) {}

    static String invoiceBefore(String customer, List<Item> items) {
        StringBuilder text = new StringBuilder("Invoice for " + customer + "\n");
        for (Item item : items) {
            text.append(String.format("  %s: %d x %d.%02d\n",
                    item.name(), item.quantity(), item.price() / 100, item.price() % 100));
        }
        // compute the total
        int total = 0;
        for (Item item : items) {
            total += item.price() * item.quantity();
        }
        text.append(String.format("Total: %d.%02d", total / 100, total % 100));
        return text.toString();
    }

    static String formatCents(int cents) {
        return String.format("%d.%02d", cents / 100, cents % 100);
    }

    static int totalCents(List<Item> items) {
        return items.stream().mapToInt(item -> item.price() * item.quantity()).sum();
    }

    static String invoiceAfter(String customer, List<Item> items) {
        StringBuilder text = new StringBuilder("Invoice for " + customer + "\n");
        for (Item item : items) {
            text.append("  " + item.name() + ": " + item.quantity() + " x " + formatCents(item.price()) + "\n");
        }
        text.append("Total: " + formatCents(totalCents(items)));
        return text.toString();
    }

    interface Invoice {
        String make(String customer, List<Item> items);
    }

    static void check(String name, Invoice invoice) {
        List<Item> items = List.of(new Item("pen", 250, 2), new Item("pad", 1205, 1));
        String expected = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05";
        if (!invoice.make("Ada", items).equals(expected)
                || !invoice.make("Bob", List.of()).equals("Invoice for Bob\nTotal: 0.00")) {
            throw new AssertionError(name + " failed");
        }
        System.out.println(name + ": tests pass");
    }

    public static void main(String[] args) {
        check("invoiceBefore", Main::invoiceBefore);
        check("invoiceAfter", Main::invoiceAfter);
        System.out.println(invoiceAfter("Ada", List.of(new Item("pen", 250, 2), new Item("pad", 1205, 1))));
    }
}
```

```output for=invoice
invoiceBefore: tests pass
invoiceAfter: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring makes a method, usually `private` or `static` when it uses no fields.
- A lambda can read only local variables that are effectively final, and a method cannot assign the locals of its caller. A fragment that assigns two locals must return a small record, such as `record Totals(int count, int cents)`.
- A fragment that uses fields of the object becomes an instance method. The fields need not become parameters, because the method can read them.
- IntelliJ IDEA and Eclipse do Extract Method as one command.
