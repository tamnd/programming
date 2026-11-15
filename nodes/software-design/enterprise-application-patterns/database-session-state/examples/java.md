```java run id=shop-on-two-servers
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.StringJoiner;
import java.util.TreeMap;

// The "database" is a set of tables that every server shares.
// A real system keeps these tables in a database server and uses JDBC.
public class Main {
    static final int TIMEOUT = 30; // minutes with no request before a session ends

    static class Session {
        final String user;
        int lastUsed;
        Session(String user, int lastUsed) { this.user = user; this.lastUsed = lastUsed; }
    }

    static class Line {
        final String key; // the session ID for a cart line, the order ID for an order line
        final String item;
        int qty;
        Line(String key, String item, int qty) { this.key = key; this.item = item; this.qty = qty; }
    }

    static final Map<String, Session> sessions = new TreeMap<>();
    static List<Line> cartLines = new ArrayList<>(); // the pending data
    static final List<String> orders = new ArrayList<>();
    static final List<Line> orderLines = new ArrayList<>(); // the committed business data

    static String hhmm(int t) { return String.format("%d:%02d", t / 60, t % 60); }

    static void start(String sid, String user, int now) { sessions.put(sid, new Session(user, now)); }

    // Return the user of the session, or null if it is unknown or idle.
    static String touch(String sid, int now) {
        Session s = sessions.get(sid);
        if (s == null || now - s.lastUsed > TIMEOUT) return null;
        s.lastUsed = now;
        return s.user;
    }

    static String cart(String sid) {
        StringJoiner j = new StringJoiner(", ");
        for (Line l : cartLines) if (l.key.equals(sid)) j.add(l.item + " " + l.qty);
        return j.toString();
    }

    static void add(String server, String sid, String item, int now) {
        String user = touch(sid, now);
        Line found = null;
        for (Line l : cartLines) if (l.key.equals(sid) && l.item.equals(item)) found = l;
        if (found != null) found.qty += 1;
        else cartLines.add(new Line(sid, item, 1));
        System.out.println(hhmm(now) + " " + user + " adds " + item + " on server " + server + ": cart " + cart(sid));
    }

    // The pending cart lines become an order. A database does this in one transaction.
    static void checkout(String server, String sid, int now) {
        String user = touch(sid, now);
        String lines = cart(sid);
        orders.add(user);
        String oid = String.valueOf(orders.size());
        List<Line> rest = new ArrayList<>();
        for (Line l : cartLines) {
            if (l.key.equals(sid)) orderLines.add(new Line(oid, l.item, l.qty));
            else rest.add(l);
        }
        cartLines = rest;
        System.out.println(hhmm(now) + " " + user + " checks out on server " + server + ": order " + oid + " with " + lines);
    }

    static void report() {
        Map<String, Integer> sums = new TreeMap<>();
        for (Line l : orderLines) sums.merge(l.item, l.qty, Integer::sum);
        StringJoiner j = new StringJoiner(", ");
        sums.forEach((item, qty) -> j.add(item + " " + qty));
        System.out.println("sales: " + j);
    }

    static void purge(int now) {
        List<String> ids = new ArrayList<>();
        for (Map.Entry<String, Session> e : sessions.entrySet())
            if (now - e.getValue().lastUsed > TIMEOUT) ids.add(e.getKey());
        cartLines.removeIf(l -> ids.contains(l.key));
        for (String id : ids) sessions.remove(id);
        System.out.println(hhmm(now) + " purge sessions idle for more than " + TIMEOUT + " minutes: " + String.join(", ", ids));
    }

    public static void main(String[] args) {
        start("s1", "ann", 600);
        start("s2", "bob", 601);
        add("a", "s1", "pen", 600);
        add("a", "s2", "ink", 601);
        add("b", "s1", "pen", 603); // server b reads the rows that server a wrote
        add("a", "s1", "ink", 604);
        checkout("b", "s1", 625);
        report();
        purge(640);
        System.out.println("rows left: sessions " + sessions.size() + ", cart lines " + cartLines.size()
            + ", orders " + orders.size() + ", order lines " + orderLines.size());
    }
}
```

```output
10:00 ann adds pen on server a: cart pen 1
10:01 bob adds ink on server a: cart ink 1
10:03 ann adds pen on server b: cart pen 2
10:04 ann adds ink on server a: cart pen 2, ink 1
10:25 ann checks out on server b: order 1 with pen 2, ink 1
sales: ink 1, pen 2
10:40 purge sessions idle for more than 30 minutes: s2
rows left: sessions 1, cart lines 0, orders 1, order lines 2
```

- Verdict: stdlib. JDBC, in the `java.sql` package, is part of the standard library, but a real server also needs the driver of its database. The example keeps the tables in shared collections, so it runs with no driver.
- A `TreeMap` keeps the session IDs in order, so `purge` lists the idle sessions in a stable order.
