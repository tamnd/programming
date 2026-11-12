```java run id=language-features
import java.util.ArrayList;
import java.util.List;
import java.util.function.Supplier;

public class Main {
    // An enum with one constant is a singleton.
    enum Registry {
        INSTANCE;
        int lookups;
    }

    record Request(String url, String method, int retries) {
        Request withMethod(String newMethod) {
            return new Request(url, newMethod, retries);
        }
    }

    public static void main(String[] args) {
        // A static factory method: List.of returns an unmodifiable list.
        List<String> names = List.of("ann", "bob");
        try {
            names.add("cy");
        } catch (UnsupportedOperationException e) {
            System.out.println("static factory: the list from List.of is unmodifiable");
        }

        // A constructor reference is a factory object.
        Supplier<List<String>> factory = ArrayList::new;
        List<String> a = factory.get();
        List<String> b = factory.get();
        System.out.println("factory: " + a.getClass().getSimpleName() + ", a new object each time: " + (a != b));

        Registry.INSTANCE.lookups++;
        Registry.INSTANCE.lookups++;
        System.out.println("singleton: " + Registry.INSTANCE.lookups + " lookups on one instance");

        // A record copies with one change through a method that you write.
        Request r = new Request("https://example.com/", "GET", 2);
        System.out.println("changed copy: " + r.withMethod("POST"));
    }
}
```

```output
static factory: the list from List.of is unmodifiable
factory: ArrayList, a new object each time: true
singleton: 2 lookups on one instance
changed copy: Request[url=https://example.com/, method=POST, retries=2]
```

- Verdict: idiom. Java writes the patterns in classes. The standard library uses static factory methods such as `List.of`, a constructor reference such as `ArrayList::new` is a factory object of type `Supplier`, and an enum with one constant is the safe form of a singleton. A class with many optional fields usually gets a builder, because Java has no named or default arguments.
