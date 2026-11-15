```java run id=max-deliveries
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

public class Main {
    static final int MAX_DELIVERIES = 3;

    static final class Message {
        final String body;
        int deliveries;
        String reason;
        String source;

        Message(String body) {
            this.body = body;
        }

        String name() {
            return body.split(" ")[0];
        }
    }

    static void handle(String body) {
        String amount = body.split(" ")[1];
        if (!amount.chars().allMatch(Character::isDigit)) {
            throw new IllegalArgumentException("bad amount " + amount);
        }
        System.out.println("ok " + body.split(" ")[0] + " amount " + amount);
    }

    public static void main(String[] args) {
        Deque<Message> queue = new ArrayDeque<>();
        for (String b : List.of("pay-1 10", "pay-2 x", "pay-3 5")) {
            queue.add(new Message(b));
        }
        List<Message> deadLetters = new ArrayList<>();
        int processed = 0;
        while (!queue.isEmpty()) {
            Message msg = queue.poll();
            msg.deliveries++;
            try {
                handle(msg.body);
                processed++;
            } catch (IllegalArgumentException e) {
                System.out.println("fail " + msg.name() + " delivery " + msg.deliveries + ": " + e.getMessage());
                if (msg.deliveries < MAX_DELIVERIES) {
                    queue.add(msg);
                } else {
                    msg.reason = e.getMessage();
                    msg.source = "payments";
                    deadLetters.add(msg);
                }
            }
        }
        for (Message msg : deadLetters) {
            System.out.println("dead letter " + msg.name() + ": deliveries=" + msg.deliveries
                    + " reason=" + msg.reason + " source=" + msg.source);
        }
        System.out.println("processed " + processed + ", dead letters " + deadLetters.size());
    }
}
```

```output
ok pay-1 amount 10
fail pay-2 delivery 1: bad amount x
ok pay-3 amount 5
fail pay-2 delivery 2: bad amount x
fail pay-2 delivery 3: bad amount x
dead letter pay-2: deliveries=3 reason=bad amount x source=payments
processed 2, dead letters 1
```

- Verdict: idiom. Java has no dead letter channel. An `ArrayDeque` is the queue, a field of the message is the delivery count, and an `ArrayList` is the dead letter channel.
- The handler throws `IllegalArgumentException` for a bad message. A handler that catches all exceptions would also move messages that failed because of a temporary error, such as a lost database connection, after the same 3 deliveries.
- In Jakarta Messaging, the provider must set the property `JMSXDeliveryCount` to the number of delivery attempts. A receiver can use it to find a message that the broker gives again and again, and move that message itself.
