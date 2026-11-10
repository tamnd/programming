```java run id=repository
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    interface Repository {
        String get(String key);

        String name();
    }

    static final class Store implements Repository {
        public String get(String key) {
            System.out.println("store: load " + key);
            return "value-" + key;
        }

        public String name() {
            return "store";
        }
    }

    static final class Logged implements Repository {
        private final Repository inner;

        Logged(Repository inner) {
            this.inner = inner;
        }

        public String get(String key) {
            System.out.println("log: get " + key);
            return inner.get(key);
        }

        public String name() {
            return inner.name();
        }
    }

    static final class Cached implements Repository {
        private final Repository inner;
        private final Map<String, String> cache = new HashMap<>();

        Cached(Repository inner) {
            this.inner = inner;
        }

        public String get(String key) {
            String value = cache.get(key);
            if (value == null) {
                value = inner.get(key);
                cache.put(key, value);
            }
            return value;
        }

        public String name() {
            return inner.name();
        }
    }

    public static void main(String[] args) {
        Repository repo = new Logged(new Cached(new Store()));
        for (String key : List.of("a", "a", "b")) {
            System.out.println(repo.get(key));
        }
        System.out.println("name: " + repo.name());
    }
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. This is the form of Design Patterns: an interface, a concrete component, and decorators that implement the interface and hold a reference to it.
- The classes are `final`. A decorator does not need to extend the class it wraps, so it can wrap a final class such as `Store`.
- `java.io` uses the same form. `FilterInputStream` holds the wrapped stream in a field and forwards to it, and `BufferedInputStream` extends it to add a buffer.
- `cache.computeIfAbsent(key, inner::get)` is shorter. The explicit form is used here to keep each step visible.
