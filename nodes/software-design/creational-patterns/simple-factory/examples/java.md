```java run id=simple-factory
import java.util.List;

public class Main {
    interface Notifier {
        String send(String user, String text);
    }

    static final class EmailNotifier implements Notifier {
        public String send(String user, String text) {
            return "mail to " + user + "@example.com: " + text;
        }
    }

    static final class SmsNotifier implements Notifier {
        public String send(String user, String text) {
            return "text to the phone of " + user + ": " + text;
        }
    }

    static final class PushNotifier implements Notifier {
        public String send(String user, String text) {
            return "push to the app of " + user + ": " + text;
        }
    }

    /** The one place that knows the concrete classes. */
    static Notifier makeNotifier(String channel) {
        return switch (channel) {
            case "email" -> new EmailNotifier();
            case "sms" -> new SmsNotifier();
            case "push" -> new PushNotifier();
            default -> throw new IllegalArgumentException("unknown channel: " + channel);
        };
    }

    public static void main(String[] args) {
        for (String channel : List.of("email", "sms", "push", "fax")) {
            Notifier notifier;
            try {
                notifier = makeNotifier(channel);
            } catch (IllegalArgumentException e) {
                System.out.println(e.getMessage());
                continue;
            }
            System.out.println(notifier.getClass().getSimpleName() + ": "
                    + notifier.send("ann", "order shipped"));
        }
    }
}
```

```output
EmailNotifier: mail to ann@example.com: order shipped
SmsNotifier: text to the phone of ann: order shipped
PushNotifier: push to the app of ann: order shipped
unknown channel: fax
```

- Verdict: idiom. A static method with a `switch` expression is the simple factory. The return type is the interface, so callers do not depend on the concrete classes.
- With an `enum` for the channel in place of a `String`, a `switch` expression that covers every constant needs no `default` branch, and the compiler reports a missing constant.
