```java run id=cycle
import java.lang.ref.WeakReference;

public class Main {
    static class Item {
        Item peer;
    }

    public static void main(String[] args) {
        Item a = new Item();
        Item b = new Item();
        a.peer = b;
        b.peer = a;
        WeakReference<Item> probe = new WeakReference<>(a);
        System.out.println("alive while referenced: " + (probe.get() != null));
        a = null;
        b = null;
        System.gc();
        System.out.println("collected after gc: " + (probe.get() == null));
    }
}
```

```output
alive while referenced: true
collected after gc: true
```

- Verdict: native. The JVM has a tracing collector. On server-class machines HotSpot selects G1, a generational collector that copies live objects out of the regions that it collects.
- `System.gc()` only suggests a collection. In this run HotSpot collected the pair, but the API does not promise that.
- `WeakReference.get()` returns `null` after the collector clears the reference.
- Use try-with-resources to close files and sockets. Finalization is deprecated for removal.
