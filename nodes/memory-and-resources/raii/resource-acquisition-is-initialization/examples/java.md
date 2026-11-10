```java run id=try-with-resources
public class Main {
    record Resource(String name, boolean failOnClose) implements AutoCloseable {
        Resource {
            System.out.println("open " + name);
        }

        @Override
        public void close() {
            System.out.println("close " + name);
            if (failOnClose) {
                throw new IllegalStateException("close failed: " + name);
            }
        }
    }

    public static void main(String[] args) {
        try (var source = new Resource("source", false);
                var target = new Resource("target", true)) {
            System.out.println("copy " + source.name() + " -> " + target.name());
            throw new IllegalArgumentException("disk full");
        } catch (IllegalArgumentException error) {
            System.out.println("caught: " + error.getMessage());
            for (Throwable suppressed : error.getSuppressed()) {
                System.out.println("suppressed: " + suppressed.getMessage());
            }
        }
    }
}
```

```output
open source
open target
copy source -> target
close target
close source
caught: disk full
suppressed: close failed: target
```

- Verdict: native. try-with-resources closes every resource it declares, in the reverse order of declaration, whether the block completes or throws.
- The resource type must implement `AutoCloseable`. A record can do this and keep its fields final.
- The body throws first, then the close of `target` throws too. Java keeps the first exception and attaches the second with `addSuppressed`.
- `getSuppressed()` returns the exceptions that were suppressed, so no error is lost.
