```java run id=key-generator-plugin
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
    public interface KeyGenerator {
        long nextKey();
    }

    // For unit tests: a counter in memory.
    public static class CounterKeyGenerator implements KeyGenerator {
        private long last = 0;
        public long nextKey() { return ++last; }
    }

    // For production: it stands for a database sequence.
    public static class SequenceKeyGenerator implements KeyGenerator {
        private long last = 1000;
        public long nextKey() { return ++last; }
    }

    static class PluginFactory {
        private final Map<String, String> config;
        PluginFactory(Map<String, String> config) { this.config = config; }

        // Load the class that the configuration names, and check that it implements the interface.
        <T> T get(Class<T> iface) {
            String className = config.get(iface.getSimpleName());
            try {
                Class<?> c = Class.forName(className);
                return iface.cast(c.getDeclaredConstructor().newInstance());
            } catch (ReflectiveOperationException e) {
                String simple = className.substring(className.lastIndexOf('$') + 1);
                throw new IllegalStateException("no plugin named " + simple, e);
            }
        }
    }

    public static void main(String[] args) {
        // One configuration for each deployment. Only the configuration names the classes.
        Map<String, Map<String, String>> configs = new LinkedHashMap<>();
        configs.put("test", Map.of("KeyGenerator", "Main$CounterKeyGenerator"));
        configs.put("production", Map.of("KeyGenerator", "Main$SequenceKeyGenerator"));
        configs.put("broken", Map.of("KeyGenerator", "Main$FileKeyGenerator"));

        for (var entry : configs.entrySet()) {
            KeyGenerator gen;
            try {
                gen = new PluginFactory(entry.getValue()).get(KeyGenerator.class);
            } catch (IllegalStateException e) {
                System.out.println(entry.getKey() + ": error: " + e.getMessage());
                continue;
            }
            String keys = gen.nextKey() + ", " + gen.nextKey() + ", " + gen.nextKey();
            System.out.println(entry.getKey() + ": keys " + keys + " from " + gen.getClass().getSimpleName());
        }
    }
}
```

```output
test: keys 1, 2, 3 from CounterKeyGenerator
production: keys 1001, 1002, 1003 from SequenceKeyGenerator
broken: error: no plugin named FileKeyGenerator
```

- Verdict: stdlib. `Class.forName` loads the class that the configuration names, and `iface.cast` checks that it implements the interface. `java.util.ServiceLoader` is the standard way to find providers that are deployed on the class path.
- The binary name of a nested class has a `$`, such as `Main$CounterKeyGenerator`.
