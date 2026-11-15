```java run id=claim-report
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;

public class Main {
    static final int THRESHOLD = 1024; // payloads larger than this go to the store
    static final Map<String, byte[]> store = new HashMap<>(); // stands for a blob store or a database
    static final List<String> channel = new ArrayList<>();

    static String hash(byte[] b) throws Exception {
        return "sha256:" + HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(b));
    }

    static void send(String kind, byte[] payload) throws Exception {
        String message, extra = "";
        if (payload.length <= THRESHOLD) {
            message = kind + ";inline;" + new String(payload, StandardCharsets.UTF_8);
        } else {
            String key = hash(payload);
            store.put(key, payload); // store first, then publish the claim check
            message = kind + ";claim;" + key + ";" + payload.length;
            extra = " for " + payload.length + " bytes in the store";
        }
        channel.add(message);
        System.out.println("sent " + kind + ": " + message.length() + " bytes on the channel" + extra);
    }

    static String receive(String message) throws Exception {
        String[] parts = message.split(";", 3);
        String kind = parts[0], mode = parts[1], rest = parts[2];
        if (mode.equals("inline")) return "received " + kind + ": " + rest;
        String[] claim = rest.split(";");
        String key = claim[0];
        byte[] payload = store.get(key);
        if (payload == null)
            return "received " + kind + ": claim " + key.substring(0, 19) + "... not found, to the dead letter channel";
        if (!hash(payload).equals(key) || payload.length != Integer.parseInt(claim[1]))
            return "received " + kind + ": payload does not match the claim check";
        long rows = new String(payload, StandardCharsets.US_ASCII).chars().filter(c -> c == '\n').count();
        return "received " + kind + ": " + rows + " rows, hash " + key.substring(7, 19) + " ok";
    }

    public static void main(String[] args) throws Exception {
        StringBuilder report = new StringBuilder();
        for (int i = 0; i < 12000; i++) report.append(String.format("row %05d;widget;12.50\n", i));
        send("invoice", "INV-17 total 12.50".getBytes(StandardCharsets.UTF_8));
        send("report", report.toString().getBytes(StandardCharsets.US_ASCII));
        for (String m : channel) System.out.println(receive(m));

        // The lifecycle rule of the store removes the payload; then the broker delivers the claim again.
        store.clear();
        System.out.println(receive(channel.get(1)));
    }
}
```

```output
sent invoice: 33 bytes on the channel
sent report: 91 bytes on the channel for 276000 bytes in the store
received invoice: INV-17 total 12.50
received report: 12000 rows, hash 63b98860796d ok
received report: claim sha256:63b98860796d... not found, to the dead letter channel
```

- Verdict: idiom. `MessageDigest.getInstance("SHA-256")` and `HexFormat` make the key; a `HashMap<String, byte[]>` stands for the blob store.
- `split(";", 3)` with a limit keeps the rest of the message in one part.
- `store.clear()` plays the lifecycle rule that removes the payload.
