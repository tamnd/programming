```java run id=cleaner
import java.lang.ref.Cleaner;

public class Main {
    static final Cleaner CLEANER = Cleaner.create();

    // The state that the cleaning action needs. It must not refer to the Conn.
    record State(String name) implements Runnable {
        public void run() { System.out.println("release " + name); }
    }

    static final class Conn implements AutoCloseable {
        private final String name;
        private final Cleaner.Cleanable cleanable;

        Conn(String name) {
            this.name = name;
            this.cleanable = CLEANER.register(this, new State(name)); // a safety net
        }

        void use() { System.out.println("using " + name); }

        @Override public void close() {
            cleanable.clean(); // runs the action now, at most once
        }
    }

    public static void main(String[] args) {
        try (var a = new Conn("a")) {
            a.use();
        }
        var b = new Conn("b");
        b.use();
        b.close();
        b.close(); // the action already ran, so nothing happens
        System.out.println("end");
    }
}
```

```output for=cleaner
using a
release a
using b
release b
end
```

- Verdict: stdlib. Java has no destructors, and `Object.finalize` is deprecated for removal. A `Cleaner` registers an action that runs once, either when the program calls `clean` or, as a safety net, after the collector finds the object phantom reachable. The action must not refer to the object, so it is a separate class, here the record `State`, and not a lambda or inner class that captures `this`. `close` calls `clean`, and try-with-resources calls `close` at the end of the block.
