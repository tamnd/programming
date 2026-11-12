```java run id=reactor
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.BiConsumer;

public class Main {
    static final Map<String, BiConsumer<String, String>> handlers = new HashMap<>();

    static void echo(String handle, String data) {
        if (!data.isEmpty()) {
            System.out.println(handle + ": " + data);
        } else {
            System.out.println("close " + handle);
            handlers.remove(handle);
        }
    }

    public static void main(String[] args) {
        handlers.put("listener", (handle, newHandle) -> {
            System.out.println("accept " + newHandle);
            handlers.put(newHandle, Main::echo);
        });
        // ready stands in for select: handle and data pairs in arrival order
        List<String[]> ready = List.of(
            new String[] {"listener", "conn-1"}, new String[] {"listener", "conn-2"},
            new String[] {"conn-1", "hello"}, new String[] {"conn-2", "hi"},
            new String[] {"conn-1", ""}, new String[] {"conn-2", ""});
        for (String[] event : ready) {
            handlers.get(event[0]).accept(event[0], event[1]);
        }
    }
}
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: idiom. The handler table maps handles to `BiConsumer` lambdas, and a method reference gives the echo handler.
