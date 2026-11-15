```java run id=tap-audit
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Queue;

public class Main {
    static final class Message {
        final String id;
        String card;
        final int total;
        final Map<String, String> headers;

        Message(String id, String card, int total, Map<String, String> headers) {
            this.id = id;
            this.card = card;
            this.total = total;
            this.headers = headers;
        }

        Message deepCopy() {
            return new Message(id, card, total, new HashMap<>(headers));
        }
    }

    static final class WireTap {
        final Queue<Message> main = new ArrayDeque<>();
        final Queue<Message> tap = new ArrayDeque<>();
        boolean enabled = true;

        void send(Message m) {
            if (enabled) {
                tap.add(m.deepCopy());
            }
            main.add(m);
        }
    }

    public static void main(String[] args) {
        var w = new WireTap();
        String[][] orders = {{"o1", "4111111111111111", "30"}, {"o2", "5500000000000004", "12"},
                {"o3", "4111111111111111", "7"}, {"o4", "340000000000009", "45"}};
        for (String[] o : orders) {
            var headers = new HashMap<String, String>();
            headers.put("source", "web");
            w.send(new Message(o[0], o[1], Integer.parseInt(o[2]), headers));
            if (o[0].equals("o3")) {
                w.enabled = false; // an operator turns the tap off
            }
        }
        List<Message> seen = new ArrayList<>();
        for (Message m; (m = w.main.poll()) != null; ) {
            seen.add(m);
            System.out.printf("main: %s card %s total %d%n", m.id, m.card, m.total);
        }
        int audited = 0;
        for (Message m; (m = w.tap.poll()) != null; ) {
            m.card = "*".repeat(m.card.length() - 4) + m.card.substring(m.card.length() - 4);
            m.headers.put("audit", "yes");
            audited++;
            System.out.printf("audit: %s card %s total %d%n", m.id, m.card, m.total);
        }
        System.out.printf("main: %d messages, audit: %d messages (tap off after o3)%n", seen.size(), audited);
        boolean unchanged = seen.stream().noneMatch(m -> m.card.contains("*") || m.headers.containsKey("audit"));
        System.out.println("main messages unchanged: " + unchanged);
    }
}
```

```output
main: o1 card 4111111111111111 total 30
main: o2 card 5500000000000004 total 12
main: o3 card 4111111111111111 total 7
main: o4 card 340000000000009 total 45
audit: o1 card ************1111 total 30
audit: o2 card ************0004 total 12
audit: o3 card ************1111 total 7
main: 4 messages, audit: 3 messages (tap off after o3)
main messages unchanged: true
```

- Verdict: idiom. Two `ArrayDeque` objects are the channels, and `deepCopy` makes a new `HashMap` for the headers.
- The audit consumer changes `card` on its copy, and the main messages keep the full number.
- `noneMatch` checks that no main message got a change from the tap.
