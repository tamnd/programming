```java run id=acceptor
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

public class Main {
    interface ServiceHandler { void open(LinkedList<String> channel); }

    static class StatusHandler implements ServiceHandler {
        LinkedList<String> channel;
        public void open(LinkedList<String> channel) {
            this.channel = channel;
            System.out.println("accept -> status handler open");
        }
        void poll() { System.out.println("status: got " + channel.removeFirst()); }
    }

    static class CommandHandler implements ServiceHandler {
        LinkedList<String> channel;
        public void open(LinkedList<String> channel) {
            this.channel = channel;
            System.out.println("connect done -> command handler open");
        }
        void send(String text) {
            System.out.println("command: send " + text);
            channel.add(text);
        }
    }

    static final Map<String, Acceptor<?>> listeners = new HashMap<>();

    static class Acceptor<H extends ServiceHandler> {
        final List<H> handlers = new ArrayList<>();
        final Supplier<H> make;
        Acceptor(String address, Supplier<H> make) {
            this.make = make;
            listeners.put(address, this);
            System.out.println("listen " + address);
        }
        void accept(LinkedList<String> channel) {
            H handler = make.get();
            handler.open(channel);
            handlers.add(handler);
        }
    }

    static <H extends ServiceHandler> H connect(String address, Supplier<H> make) {
        System.out.println("connect " + address);
        LinkedList<String> channel = new LinkedList<>();
        listeners.get(address).accept(channel);
        H handler = make.get();  // complete: activate when the connection is up
        handler.open(channel);
        return handler;
    }

    public static void main(String[] args) {
        Acceptor<StatusHandler> acceptor = new Acceptor<>("gateway:9000", StatusHandler::new);
        CommandHandler command = connect("gateway:9000", CommandHandler::new);
        command.send("reset");
        acceptor.handlers.get(0).poll();
    }
}
```

```output
listen gateway:9000
connect gateway:9000
accept -> status handler open
connect done -> command handler open
command: send reset
status: got reset
```

- Verdict: idiom. A constructor reference such as `StatusHandler::new` is the factory that the acceptor calls for each connection.
