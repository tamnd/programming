```java run id=align-the-senders
import java.util.ArrayList;
import java.util.List;

public class Main {
    record User(String email, String phone) {}

    // Before: two classes for one job, with two interfaces.
    static final class EmailSenderBefore {
        final List<String> sent = new ArrayList<>();
        void send(String to, String subject, String body) { sent.add("email to " + to + ": " + subject + " | " + body); }
    }

    static final class SmsGatewayBefore {
        final List<String> sent = new ArrayList<>();
        void dispatch(String phone, String text) {
            sent.add("sms to " + phone + ": " + text.substring(0, Math.min(40, text.length())));
        }
    }

    static void alertBefore(Object channel, User user, String text) {
        if (channel instanceof EmailSenderBefore e) e.send(user.email(), "Alert", text);
        else if (channel instanceof SmsGatewayBefore s) s.dispatch(user.phone(), text);
        else throw new IllegalArgumentException("unknown channel");
    }

    // After: one interface, deliver(user, text), in both classes.
    interface Channel {
        void deliver(User user, String text);
    }

    static final class EmailSender implements Channel {
        final List<String> sent = new ArrayList<>();
        public void deliver(User user, String text) { sent.add("email to " + user.email() + ": Alert | " + text); }
    }

    static final class SmsGateway implements Channel {
        final List<String> sent = new ArrayList<>();
        public void deliver(User user, String text) {
            sent.add("sms to " + user.phone() + ": " + text.substring(0, Math.min(40, text.length())));
        }
    }

    static void alert(Channel channel, User user, String text) { channel.deliver(user, text); }

    public static void main(String[] args) {
        User ana = new User("ana@example.org", "+31600000000");
        String msg = "Your backup finished with 2 warnings.";

        EmailSenderBefore e0 = new EmailSenderBefore();
        SmsGatewayBefore s0 = new SmsGatewayBefore();
        EmailSender e1 = new EmailSender();
        SmsGateway s1 = new SmsGateway();
        alertBefore(e0, ana, msg);
        alertBefore(s0, ana, msg);
        for (Channel c : List.<Channel>of(e1, s1)) alert(c, ana, msg);

        if (!e1.sent.equals(e0.sent) || !s1.sent.equals(s0.sent)) throw new AssertionError("sent");
        System.out.println("deliver: tests pass");
    }
}
```

```output
deliver: tests pass
```

- `alertBefore` takes an `Object`, because the two classes have no common type. The compiler cannot stop a caller from passing something else, so the method needs a third branch that throws.
- `Channel` names the common interface, and `alert` takes a `Channel`. A wrong argument is now a compile error.
