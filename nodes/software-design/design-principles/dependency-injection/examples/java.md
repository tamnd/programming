```java run id=invoices
import java.util.ArrayList;
import java.util.List;

public class Main {
    interface Clock { int today(); }

    interface Mailer { void send(String to, String text); }

    record Invoice(int number, String customer, int due) {}

    static final class ReminderService {
        private final Clock clock;
        private final Mailer mailer;

        // Constructor injection: the object is complete when it exists.
        ReminderService(Clock clock, Mailer mailer) {
            this.clock = clock;
            this.mailer = mailer;
        }

        boolean remind(Invoice invoice) {
            int late = clock.today() - invoice.due();
            if (late <= 0) return false;
            mailer.send(invoice.customer(), "invoice " + invoice.number() + " is " + late + " days overdue");
            return true;
        }
    }

    record FixedClock(int day) implements Clock {
        public int today() { return day; }
    }

    static final class ConsoleMailer implements Mailer {
        public void send(String to, String text) { System.out.println("to " + to + ": " + text); }
    }

    static final class RecordingMailer implements Mailer {
        final List<String> sent = new ArrayList<>();
        public void send(String to, String text) { sent.add("to " + to + ": " + text); }
    }

    public static void main(String[] args) {
        // The composition root.
        var service = new ReminderService(new FixedClock(40), new ConsoleMailer());
        var invoices = List.of(new Invoice(17, "ana@example.com", 31), new Invoice(18, "bo@example.com", 45));
        long sent = invoices.stream().filter(service::remind).count();
        System.out.println("reminders sent: " + sent);

        // A test wires the same class to a recording double.
        var recorder = new RecordingMailer();
        new ReminderService(new FixedClock(50), recorder).remind(invoices.get(1));
        System.out.println("recorded: " + recorder.sent.get(0));

        // Clock has one method, so a lambda can be the clock.
        Clock lateClock = () -> 60;
        new ReminderService(lateClock, recorder).remind(invoices.get(0));
        System.out.println("recorded: " + recorder.sent.get(1));
    }
}
```

```output
to ana@example.com: invoice 17 is 9 days overdue
reminders sent: 1
recorded: to bo@example.com: invoice 18 is 5 days overdue
recorded: to ana@example.com: invoice 17 is 29 days overdue
```

- Verdict: idiom. Interfaces describe the roles, and `final` fields that the constructor sets hold the collaborators. Fowler gives this as a reason to prefer constructor injection: the fields can be immutable, and the object is valid as soon as it exists.
- `Clock` has one abstract method, so it is a functional interface, and the lambda `() -> 60` implements it. A role with one operation needs no class.
- Containers such as Spring create and wire these objects from annotations or configuration. The standard library has no container, and this program does not need one.
