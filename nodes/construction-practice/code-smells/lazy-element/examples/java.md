```java run id=inline-the-layers
public class Main {
    // Before: a subclass that adds nothing, and a method that only forwards.
    static class Person {
        final String first;
        final String last;

        Person(String first, String last) {
            this.first = first;
            this.last = last;
        }

        String fullName() {
            return first + " " + last;
        }
    }

    static class NamedPerson extends Person {
        NamedPerson(String first, String last) {
            super(first, last);
        }

        String displayName() {
            return fullName(); // forwards, and adds nothing
        }
    }

    static String greetingBefore(NamedPerson p) {
        return "Hello, " + p.displayName();
    }

    // After: the hierarchy is collapsed, and the caller uses fullName.
    static String greeting(Person p) {
        return "Hello, " + p.fullName();
    }

    public static void main(String[] args) {
        NamedPerson before = new NamedPerson("Ada", "Lovelace");
        Person after = new Person("Ada", "Lovelace");
        if (!greetingBefore(before).equals("Hello, Ada Lovelace")) throw new AssertionError("before");
        if (!greeting(after).equals(greetingBefore(before))) throw new AssertionError("after");
        System.out.println("greeting: tests pass");
    }
}
```

```output
greeting: tests pass
```

- `NamedPerson` has no field and no rule of its own, so Collapse Hierarchy merges it into `Person`. Each `new NamedPerson(...)` becomes `new Person(...)`.
- `displayName` is a second name for `fullName`. Inline Function replaces each call with `fullName()`, and the method goes away with the subclass.
