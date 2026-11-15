```java run id=translate-orders
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    // The format of the shop.
    record Order(int id, String customer, int totalCents, String date) {}

    static final Pattern AMOUNT = Pattern.compile("(\\d+)(?:\\.(\\d\\d))?");

    // Translate one partner line (id;customer;amount;day/month/year) into an Order.
    static Order translate(String line) {
        String[] f = line.split(";");
        if (f.length != 4) throw new IllegalArgumentException("want 4 fields");
        Matcher m = AMOUNT.matcher(f[2]);
        if (!m.matches()) throw new IllegalArgumentException("bad amount " + f[2]);
        int cents = Integer.parseInt(m.group(1)) * 100 + (m.group(2) == null ? 0 : Integer.parseInt(m.group(2)));
        String[] d = f[3].split("/");
        if (d.length != 3) throw new IllegalArgumentException("bad date " + f[3]);
        int id = Integer.parseInt(f[0].replaceFirst("^P-", ""));
        return new Order(id, f[1], cents, d[2] + "-" + d[1] + "-" + d[0]);
    }

    public static void main(String[] args) {
        List<String> partner = List.of(
            "P-17;Lan Pham;12.50;03/11/2026",
            "P-18;Hoa Le;8;05/11/2026",
            "P-19;Minh Tran;7,25;04/11/2026");
        for (String line : partner) {
            try {
                Order o = translate(line);
                System.out.printf("order %d: customer=%s total_cents=%d date=%s%n", o.id(), o.customer(), o.totalCents(), o.date());
            } catch (IllegalArgumentException e) {
                System.out.printf("invalid: %s (%s)%n", line, e.getMessage());
            }
        }
    }
}
```

```output
order 17: customer=Lan Pham total_cents=1250 date=2026-11-03
order 18: customer=Hoa Le total_cents=800 date=2026-11-05
invalid: P-19;Minh Tran;7,25;04/11/2026 (bad amount 7,25)
```

- Verdict: idiom. A record is the target format, and a static method maps a partner line to a new record.
- The method throws `IllegalArgumentException` with a reason for a line that it cannot read; `NumberFormatException` is a subclass, so a bad id also goes to the invalid channel.
- Frameworks such as Apache Camel and Spring Integration call a method of this form as a translator in a route.
