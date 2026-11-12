```java run id=one-object-for-one-thing
import java.util.HashMap;
import java.util.Map;

public class Main {
    static final class Customer {
        final String id;
        String email;
        Customer(String id, String email) { this.id = id; this.email = email; }
    }

    static final class CustomerRepository {
        private final Map<String, Customer> byId = new HashMap<>();
        Customer register(String id, String email) {
            return byId.computeIfAbsent(id, k -> new Customer(k, email));
        }
        Customer get(String id) {
            Customer c = byId.get(id);
            if (c == null) throw new IllegalArgumentException("no customer " + id);
            return c;
        }
    }

    record Order(Customer customer) {}

    public static void main(String[] args) {
        CustomerRepository repo = new CustomerRepository();
        repo.register("c1", "old@example.org");
        Order a = new Order(repo.get("c1"));
        Order b = new Order(repo.get("c1"));
        a.customer().email = "changed@example.org";
        if (!b.customer().email.equals("changed@example.org")) throw new AssertionError("update");
        if (a.customer() != b.customer()) throw new AssertionError("identity");
        System.out.println("change value to reference: tests pass");
    }
}
```

```output
change value to reference: tests pass
```
