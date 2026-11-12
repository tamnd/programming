```java run id=give-the-base-the-method
import java.util.ArrayList;
import java.util.List;

public class Main {
    // Before: an abstract class with no operation, and casts in the client.
    abstract static class NoticeBefore {}

    static final class EmailBefore extends NoticeBefore {
        final String address;
        EmailBefore(String address) { this.address = address; }
    }

    static final class SmsBefore extends NoticeBefore {
        final String phone;
        SmsBefore(String phone) { this.phone = phone; }
    }

    static String logLineBefore(NoticeBefore n) {
        if (n instanceof EmailBefore) {
            return "email to " + ((EmailBefore) n).address;
        }
        if (n instanceof SmsBefore) {
            return "sms to " + ((SmsBefore) n).phone;
        }
        throw new IllegalArgumentException("unknown notice " + n.getClass().getSimpleName());
    }

    // After: the interface has the operation. A class that implements Notice
    // and has no describe method does not compile.
    interface Notice {
        String describe();
    }

    record Email(String address) implements Notice {
        public String describe() { return "email to " + address; }
    }

    record Sms(String phone) implements Notice {
        public String describe() { return "sms to " + phone; }
    }

    record Push(String device) implements Notice {
        public String describe() { return "push to " + device; }
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public static void main(String[] args) {
        check(logLineBefore(new EmailBefore("ada@example.com")).equals(new Email("ada@example.com").describe()), "email");
        List<Notice> notices = List.of(new Email("ada@example.com"), new Sms("+15550100"), new Push("tablet-7"));
        List<String> lines = new ArrayList<>();
        for (Notice n : notices) {
            lines.add(n.describe());
        }
        check(lines.equals(List.of("email to ada@example.com", "sms to +15550100", "push to tablet-7")), "lines");

        // A container of the top type: the compiler cannot check what comes out.
        List raw = new ArrayList();
        raw.add("ada@example.com");
        raw.add(42);
        boolean failed = false;
        try {
            for (Object o : raw) {
                String address = (String) o;
                check(address.contains("@"), "address");
            }
        } catch (ClassCastException e) {
            failed = true;
        }
        check(failed, "the cast should fail at run time");

        // A generic container keeps the type, so no cast is needed,
        // and raw.add(42) on a List<String> would not compile.
        List<String> addresses = new ArrayList<>();
        addresses.add("ada@example.com");
        for (String address : addresses) {
            check(address.contains("@"), "generic address");
        }
        System.out.println("notices: tests pass");
    }

    static void check(boolean ok, String what) {
        if (!ok) {
            throw new AssertionError(what);
        }
    }
}
```

```output
notices: tests pass
```

The raw `List` is the container of the top type that Wikipedia names: it holds `Object`, so each element must be cast back, and the wrong element fails with a `ClassCastException` at run time. The generic `List<String>` keeps the type, and the compiler rejects the wrong element. Since JDK 16, the cast after `instanceof` can be written as `n instanceof EmailBefore e`, which removes the repeated type name, but not the chain of tests.
