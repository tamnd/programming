```java run id=report
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.function.Supplier;

public class Main {
    interface Report {
        String render();
    }

    static final class DiskReport implements Report {
        private final int orders;

        DiskReport() {
            System.out.println("load: read the report from disk");
            orders = 42;
        }

        public String render() {
            return "report: " + orders + " orders";
        }
    }

    // A virtual proxy: the handler makes the real object on the first call.
    static final class Lazy implements InvocationHandler {
        private final Supplier<Report> factory;
        private Report real;

        Lazy(Supplier<Report> factory) {
            this.factory = factory;
        }

        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            if (real == null) {
                real = factory.get();
            }
            try {
                return method.invoke(real, args);
            } catch (InvocationTargetException e) {
                throw e.getCause(); // rethrow what the real method threw
            }
        }
    }

    // A protection proxy: the handler checks the role before it forwards.
    record Guard(Report inner, String role) implements InvocationHandler {
        public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
            if (method.getName().equals("render") && !role.equals("admin")) {
                return "access denied";
            }
            return method.invoke(inner, args);
        }
    }

    static Report proxy(InvocationHandler handler) {
        return (Report) Proxy.newProxyInstance(
                Report.class.getClassLoader(), new Class<?>[] {Report.class}, handler);
    }

    public static void main(String[] args) {
        Report report = proxy(new Lazy(DiskReport::new));
        System.out.println("proxy made");
        System.out.println(report.render());
        System.out.println(report.render());
        for (String role : new String[] {"guest", "admin"}) {
            System.out.println(role + ": " + proxy(new Guard(report, role)).render());
        }
    }
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: stdlib. `java.lang.reflect.Proxy.newProxyInstance` makes at run time an object that implements the given interfaces. Each call on it goes to the `invoke` method of an `InvocationHandler`.
- A dynamic proxy works only for interfaces. To proxy a class, a program needs a subclass or a library that generates bytecode.
- `Method.invoke` wraps an exception from the real method in an `InvocationTargetException`. The lazy handler unwraps it, so the caller sees the original exception.
- One handler serves every method of the interface. This fits proxies that do the same work for each call, such as logging, timing or a check.
