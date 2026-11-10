```java run id=register
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Main {
    interface Users {
        boolean exists(String email);
        void add(String email);
    }

    interface Mailer {
        void send(String to, String body);
    }

    static String register(String email, Users users, Mailer mailer) {
        if (users.exists(email)) {
            return "taken";
        }
        users.add(email);
        mailer.send(email, "Welcome!");
        return "created";
    }

    // A fake: a working store in memory.
    static final class FakeUsers implements Users {
        private final Set<String> emails = new HashSet<>();
        public boolean exists(String email) { return emails.contains(email); }
        public void add(String email) { emails.add(email); }
    }

    // A spy made at run time: a Proxy that records each call to any interface.
    @SuppressWarnings("unchecked")
    static <T> T spy(Class<T> type, List<String> calls) {
        return (T) Proxy.newProxyInstance(
            type.getClassLoader(),
            new Class<?>[] {type},
            (proxy, method, args) -> {
                calls.add(method.getName() + List.of(args));
                return null;
            });
    }

    public static void main(String[] args) {
        Users users = new FakeUsers();
        List<String> calls = new ArrayList<>();
        Mailer mailer = spy(Mailer.class, calls);
        System.out.println(register("ada@example.com", users, mailer));
        System.out.println(register("ada@example.com", users, mailer));
        System.out.println(calls);
    }
}
```

```output
created
taken
[send[ada@example.com, Welcome!]]
```

- Verdict: idiom. The standard library has no mock framework. The usual seam is an interface, and a test class implements it.
- `java.lang.reflect.Proxy.newProxyInstance` makes an object at run time that implements the given interfaces. Each call goes to one `InvocationHandler`, which here records the method name and the arguments.
- A proxy works only for interfaces, not for classes. A double for a class needs a subclass, so a `final` class or a `final` method cannot be replaced this way.
- `List.of(args)` copies the arguments of each call into the record. A handler for a method with no parameters gets `null` for `args`, so a general spy must check for it.
