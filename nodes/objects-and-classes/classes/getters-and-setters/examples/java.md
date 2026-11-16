```java run id=bean-accessors
public class Main {
  static final class Employee {
    private final String name; // no setter: a read-only property
    private String email;
    private boolean active = true;

    Employee(String name, String email) {
      this.name = name;
      setEmail(email); // the constructor uses the same check
    }

    public String getName() {
      return name;
    }

    public String getEmail() {
      return email;
    }

    public void setEmail(String email) {
      String clean = email.strip().toLowerCase();
      if (!clean.contains("@")) {
        throw new IllegalArgumentException("not an email address: " + clean);
      }
      this.email = clean;
    }

    public boolean isActive() { // a boolean getter starts with "is"
      return active;
    }

    public void setActive(boolean active) {
      this.active = active;
    }
  }

  record Badge(String name, int number) {} // accessors name() and number(), no setters

  public static void main(String[] args) {
    Employee e = new Employee("Ana", "ana@example.com");
    e.setEmail("  Ana@Example.COM ");
    e.setActive(false);
    System.out.println(e.getName() + " " + e.getEmail() + " " + e.isActive());
    try {
      e.setEmail("not an address");
    } catch (IllegalArgumentException ex) {
      System.out.println(ex.getMessage());
    }
    System.out.println(e.getEmail());

    Badge b = new Badge("Ana", 7);
    System.out.println(b.name() + " " + b.number());
  }
}
```

```output for=bean-accessors
Ana ana@example.com false
not an email address: not an address
ana@example.com
Ana 7
```

The class follows the JavaBeans names: `getEmail` and `setEmail` for a property `email`, `isActive` for a `boolean`, and only `getName` for the read-only `name`. Tools that read these names, such as frameworks that bind form fields or map rows to objects, find the properties by the `get`, `is` and `set` prefixes. The record `Badge` holds plain data, so it has accessors without the prefix and no setters.
