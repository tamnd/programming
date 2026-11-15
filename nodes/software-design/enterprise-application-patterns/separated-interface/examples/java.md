```java run id=notifier-interface
import java.util.ArrayList;
import java.util.List;

public class Main {
    // ---- package domain: it defines the interface that it needs, and imports nothing below.

    interface Notifier {
        void send(String customer, String text);
    }

    static class OrderService {
        private final Notifier notifier;
        OrderService(Notifier notifier) { this.notifier = notifier; }

        void confirm(int number, String customer) {
            notifier.send(customer, "order " + number + " is confirmed");
        }
    }

    // ---- package infrastructure: it imports the domain and implements its interface.

    static class EmailNotifier implements Notifier {
        public void send(String customer, String text) {
            System.out.println("email to " + customer + ": " + text);
        }
    }

    // ---- package tests: another implementation of the same interface.

    static class RecordingNotifier implements Notifier {
        final List<String> sent = new ArrayList<>();
        public void send(String customer, String text) { sent.add(customer + ": " + text); }
    }

    // ---- main: the only code that knows both the domain and the implementations.

    public static void main(String[] args) {
        new OrderService(new EmailNotifier()).confirm(7, "ann");

        RecordingNotifier recorder = new RecordingNotifier();
        new OrderService(recorder).confirm(8, "bob");
        System.out.println("the test notifier recorded: " + String.join("; ", recorder.sent));
    }
}
```

```output
email to ann: order 7 is confirmed
the test notifier recorded: bob: order 8 is confirmed
```

- Verdict: native. `Notifier` is a Java interface. In a real program, it is in the package of the domain, and `EmailNotifier` is in a package that imports it. With modules, the domain module exports the interface and does not require the infrastructure module.
