```java run id=sessions-on-two-servers
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // One server's sessions, kept in its memory as serialized text, with the time of their last use.
    static class SessionStore {
        static final int TIMEOUT = 30; // minutes with no request, then the session ends

        record Entry(String text, int lastUse) {}

        final String name;
        final Map<String, Entry> sessions = new TreeMap<>();
        int count = 0;

        SessionStore(String name) {
            this.name = name;
        }

        String newId() {
            // A real server makes a random ID with a secure generator, such as java.security.SecureRandom.
            count++;
            return name + count;
        }

        Map<String, Integer> load(String sid, int now) {
            Entry e = sessions.get(sid);
            if (e == null || now - e.lastUse() > TIMEOUT) {
                return null;
            }
            return deserialize(e.text());
        }

        void save(String sid, Map<String, Integer> cart, int now) {
            sessions.put(sid, new Entry(serialize(cart), now));
        }

        List<String> sweep(int now) {
            List<String> idle = new ArrayList<>();
            for (Map.Entry<String, Entry> e : sessions.entrySet()) {
                if (now - e.getValue().lastUse() > TIMEOUT) {
                    idle.add(e.getKey());
                }
            }
            idle.forEach(sessions::remove);
            return idle;
        }
    }

    static String serialize(Map<String, Integer> cart) {
        List<String> parts = new ArrayList<>();
        cart.forEach((item, qty) -> parts.add(item + ":" + qty));
        return String.join(",", parts);
    }

    static Map<String, Integer> deserialize(String text) {
        Map<String, Integer> cart = new LinkedHashMap<>();
        for (String part : text.split(",")) {
            if (!part.isEmpty()) {
                String[] kv = part.split(":");
                cart.put(kv[0], Integer.parseInt(kv[1]));
            }
        }
        return cart;
    }

    static String show(Map<String, Integer> cart) {
        List<String> parts = new ArrayList<>();
        cart.forEach((item, qty) -> parts.add(item + " " + qty));
        return String.join(", ", parts);
    }

    static String clock(int minute) {
        return String.format("10:%02d", minute);
    }

    // One request. The client sends only its session ID; the state is on the server.
    static String add(SessionStore server, String sid, String who, String item, int now) {
        Map<String, Integer> cart = sid == null ? null : server.load(sid, now);
        String status;
        if (cart == null) {
            status = sid == null ? "" : "session " + sid + " is unknown here, ";
            sid = server.newId();
            cart = new LinkedHashMap<>();
            status += "new session " + sid;
        } else {
            status = "session " + sid;
        }
        cart.merge(item, 1, Integer::sum);
        server.save(sid, cart, now);
        System.out.println(clock(now) + " " + who + " adds " + item + " on server " + server.name + ": " + status + ", cart " + show(cart));
        return sid;
    }

    public static void main(String[] args) {
        SessionStore a = new SessionStore("a");
        SessionStore b = new SessionStore("b");
        String ann = add(a, null, "ann", "pen", 0);
        add(a, null, "bob", "ink", 1);
        add(a, ann, "ann", "pen", 2);
        add(b, ann, "ann", "ink", 3); // a load balancer sends this request to the other server
        add(a, ann, "ann", "ink", 30);
        System.out.println("10:40 server a removes idle sessions: " + String.join(", ", a.sweep(40)));
        a.sessions.forEach((sid, e) -> System.out.println("server a keeps " + sid + " = '" + e.text() + "', last used at " + clock(e.lastUse())));
    }
}
```

```output
10:00 ann adds pen on server a: new session a1, cart pen 1
10:01 bob adds ink on server a: new session a2, cart ink 1
10:02 ann adds pen on server a: session a1, cart pen 2
10:03 ann adds ink on server b: session a1 is unknown here, new session b1, cart ink 1
10:30 ann adds ink on server a: session a1, cart pen 2, ink 1
10:40 server a removes idle sessions: a2
server a keeps a1 = 'pen:2,ink:1', last used at 10:30
```

- Verdict: idiom. The store is a `TreeMap` of `Entry` records (Java 16 and later). In a servlet container, `HttpSession` gives the same store, and a server with many threads would use a `ConcurrentHashMap`.
- `sweep` collects the idle IDs first and removes them after the loop, because a `TreeMap` must not change while a loop goes through it.
