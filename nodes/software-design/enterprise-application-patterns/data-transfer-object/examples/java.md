```java run id=assembler-and-wire
import java.util.ArrayList;
import java.util.List;

public class Main {
    static String cents(long n) {
        return String.format("%d.%02d", n / 100, n % 100);
    }

    // Domain objects on the server. Some data must never leave the server.
    record Line(String item, int qty, long price, long costPrice) {}

    record Order(int number, List<Line> lines) {
        long total() { return lines.stream().mapToLong(l -> l.qty() * l.price()).sum(); }
    }

    static class Customer {
        String name, email;
        final String passwordHash;
        final List<Order> orders;
        Customer(String name, String email, String passwordHash, List<Order> orders) {
            this.name = name;
            this.email = email;
            this.passwordHash = passwordHash;
            this.orders = orders;
        }
    }

    // Data transfer objects: only data, shaped for one remote call, and able to serialize themselves.
    record OrderSummaryDTO(int number, long total) {}

    static class CustomerDTO {
        String name, email;
        final List<OrderSummaryDTO> orders;
        CustomerDTO(String name, String email, List<OrderSummaryDTO> orders) {
            this.name = name;
            this.email = email;
            this.orders = orders;
        }

        // One value on each line: the name, the email, and then one line for each order.
        String toWire() {
            StringBuilder b = new StringBuilder(name + "\n" + email + "\n");
            for (OrderSummaryDTO o : orders) b.append(o.number()).append(' ').append(o.total()).append('\n');
            return b.toString();
        }

        static CustomerDTO fromWire(String text) {
            String[] lines = text.split("\n");
            List<OrderSummaryDTO> orders = new ArrayList<>();
            for (int i = 2; i < lines.length; i++) {
                String[] p = lines[i].split(" ");
                orders.add(new OrderSummaryDTO(Integer.parseInt(p[0]), Long.parseLong(p[1])));
            }
            return new CustomerDTO(lines[0], lines[1], orders);
        }
    }

    // The assembler moves the data between the domain objects and the DTOs, on the server.
    static class CustomerAssembler {
        static CustomerDTO toDTO(Customer c) {
            List<OrderSummaryDTO> orders = c.orders.stream().map(o -> new OrderSummaryDTO(o.number(), o.total())).toList();
            return new CustomerDTO(c.name, c.email, orders);
        }

        static void updateFrom(CustomerDTO dto, Customer c) {
            c.name = dto.name; // the orders are read only
            c.email = dto.email;
        }
    }

    public static void main(String[] args) {
        Customer ann = new Customer("Ann", "ann@example.com", "pbkdf2:9f86d081", List.of(
                new Order(7, List.of(new Line("lamp", 2, 2500, 1400), new Line("desk", 1, 7500, 5100),
                        new Line("pen", 4, 500, 90))),
                new Order(8, List.of(new Line("chair", 1, 4000, 2600)))));

        // Server: one call returns all the data that the screen needs.
        String wire = CustomerAssembler.toDTO(ann).toWire();
        System.out.println("password hash on the wire: " + (wire.contains(ann.passwordHash) ? "yes" : "no"));

        // Client: it reads the DTO, and changes its own copy.
        CustomerDTO dto = CustomerDTO.fromWire(wire);
        System.out.println("client: " + dto.name + " <" + dto.email + ">, " + dto.orders.size() + " orders");
        for (OrderSummaryDTO o : dto.orders) System.out.println("  order " + o.number() + ": " + cents(o.total()));
        dto.email = "ann@example.org";
        System.out.println("server after a change on the client: " + ann.email);

        // Client sends the DTO back; the assembler updates the domain object.
        CustomerAssembler.updateFrom(CustomerDTO.fromWire(dto.toWire()), ann);
        System.out.println("server after the update call: " + ann.email);
    }
}
```

```output
password hash on the wire: no
client: Ann <ann@example.com>, 2 orders
  order 7: 145.00
  order 8: 40.00
server after a change on the client: ann@example.com
server after the update call: ann@example.org
```

- Verdict: idiom. `OrderSummaryDTO` is a record. `CustomerDTO` is a class, because the client changes its email. In a real application, a library such as Jackson would serialize the DTOs to JSON. Here, `toWire` writes one value on each line, so the example needs only the standard library, and a name or an email that has a line break would break this simple format.
- The Core J2EE patterns call this pattern the Transfer Object.
