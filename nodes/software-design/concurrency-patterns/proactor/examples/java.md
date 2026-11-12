```java run id=proactor
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.function.BiConsumer;

public class Main {
    record Completion(BiConsumer<String, Integer> handler, String name, int size) {}

    static final Map<String, String> files = Map.of("a.txt", "hello", "b.txt", "abc");
    static final BlockingQueue<Completion> completions = new LinkedBlockingQueue<>();
    static final BlockingQueue<Object[]> operations = new LinkedBlockingQueue<>();

    @SuppressWarnings("unchecked")
    public static void main(String[] args) throws InterruptedException {
        Thread processor = new Thread(() -> {
            try {
                for (int i = 0; i < 2; i++) {
                    Object[] op = operations.take();
                    String name = (String) op[0];
                    completions.put(new Completion(
                        (BiConsumer<String, Integer>) op[1], name, files.get(name).length()));
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        processor.start();
        BiConsumer<String, Integer> onRead =
            (name, size) -> System.out.println(name + " done: " + size + " bytes");
        for (String name : new String[] {"a.txt", "b.txt"}) {
            System.out.println("start read " + name);
            operations.put(new Object[] {name, onRead});
        }
        for (int i = 0; i < 2; i++) {
            Completion c = completions.take();  // the Completion Dispatcher
            c.handler().accept(c.name(), c.size());
        }
        processor.join();
    }
}
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: stdlib. `LinkedBlockingQueue` joins the processor thread and the dispatcher. The completion is a record with its handler.
