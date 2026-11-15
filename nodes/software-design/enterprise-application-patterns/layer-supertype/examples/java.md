```java run id=domain-object-supertype
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Main {
    record Rule(boolean ok, String message) {}

    // The layer supertype: every domain object has an ID, equality by ID and a check.
    abstract static class DomainObject {
        final long id;
        DomainObject(long id) { this.id = id; }

        abstract String name();
        abstract List<Rule> rules(); // each subclass gives its own rules

        String check() {
            List<String> problems = new ArrayList<>();
            for (Rule r : rules()) if (!r.ok()) problems.add(r.message());
            return problems.isEmpty() ? "valid" : "invalid: " + String.join(", ", problems);
        }

        @Override public boolean equals(Object other) {
            return other != null && other.getClass() == getClass() && ((DomainObject) other).id == id;
        }

        @Override public int hashCode() { return Objects.hash(getClass().getName(), id); }
    }

    static class Customer extends DomainObject {
        final String name;
        Customer(long id, String name) { super(id); this.name = name; }
        String name() { return name; }
        List<Rule> rules() { return List.of(new Rule(!name.isEmpty(), "name is empty")); }
    }

    static class Product extends DomainObject {
        final String name;
        final long priceCents;
        Product(long id, String name, long priceCents) { super(id); this.name = name; this.priceCents = priceCents; }
        String name() { return name; }
        List<Rule> rules() {
            return List.of(new Rule(!name.isEmpty(), "name is empty"), new Rule(priceCents >= 0, "price is negative"));
        }
    }

    static String yesNo(boolean b) { return b ? "yes" : "no"; }

    public static void main(String[] args) {
        for (DomainObject o : List.of(new Customer(1, "ann"), new Product(2, "pen", 150), new Product(3, "", -5))) {
            String name = o.name().isEmpty() ? "(no name)" : o.name();
            System.out.println(o.getClass().getSimpleName().toLowerCase() + " " + o.id + " " + name + ": " + o.check());
        }
        System.out.println("customer 1 equals customer 1 loaded again: " + yesNo(new Customer(1, "ann").equals(new Customer(1, "ann"))));
        System.out.println("customer 1 equals product 1: " + yesNo(new Customer(1, "ann").equals(new Product(1, "ann", 0))));
    }
}
```

```output
customer 1 ann: valid
product 2 pen: valid
product 3 (no name): invalid: name is empty, price is negative
customer 1 equals customer 1 loaded again: yes
customer 1 equals product 1: no
```

- Verdict: native. `DomainObject` is an abstract class. It overrides `equals` and `hashCode` for all the domain classes, and `check` uses the abstract method `rules`. `getClass() ==` keeps a customer and a product with the same ID apart.
