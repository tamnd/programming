```java run id=cookie-cart
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HexFormat;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

public class Main {
    static final byte[] SECRET = "server secret, never sent to the client".getBytes(StandardCharsets.UTF_8);

    static String sign(String data) {
        try {
            Mac mac = Mac.getInstance("HmacSHA256");
            mac.init(new SecretKeySpec(SECRET, "HmacSHA256"));
            return HexFormat.of().formatHex(mac.doFinal(data.getBytes(StandardCharsets.UTF_8)));
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    // Serializes the session state into the value of a cookie: the data, a bar and the signature.
    static String encode(Map<String, Integer> cart) {
        List<String> parts = new ArrayList<>();
        cart.forEach((item, qty) -> parts.add(item + ":" + qty));
        String data = String.join(",", parts);
        return data + "|" + sign(data);
    }

    // Checks the signature and gives back the state. A changed cookie gives an error.
    static Map<String, Integer> decode(String cookie) {
        int bar = cookie.lastIndexOf('|');
        String data = cookie.substring(0, bar);
        byte[] tag = cookie.substring(bar + 1).getBytes(StandardCharsets.UTF_8);
        if (!MessageDigest.isEqual(tag, sign(data).getBytes(StandardCharsets.UTF_8))) {
            throw new IllegalArgumentException("bad signature");
        }
        Map<String, Integer> cart = new LinkedHashMap<>();
        for (String part : data.split(",")) {
            if (part.isEmpty()) {
                continue;
            }
            String[] kv = part.split(":");
            cart.put(kv[0], Integer.parseInt(kv[1]));
        }
        return cart;
    }

    // One request. The server keeps nothing between requests: the state comes in and goes out in the cookie.
    static String handleAdd(String cookie, String item) {
        Map<String, Integer> cart = cookie == null ? new LinkedHashMap<>() : decode(cookie);
        cart.merge(item, 1, Integer::sum);
        return encode(cart);
    }

    static String show(String cookie) {
        int bar = cookie.lastIndexOf('|');
        return cookie.substring(0, bar) + "|" + cookie.substring(bar + 1, bar + 9) + "... (" + cookie.length() + " bytes)";
    }

    public static void main(String[] args) {
        String cookie = null;
        for (String item : new String[] {"pen", "pen", "ink"}) {
            cookie = handleAdd(cookie, item);
            System.out.println("add " + item + ": cookie " + show(cookie));
        }
        String tampered = cookie.replace("ink:1", "ink:9");
        try {
            handleAdd(tampered, "pen");
        } catch (IllegalArgumentException e) {
            System.out.println("a client changes ink:1 to ink:9: " + e.getMessage());
        }
        List<String> lines = new ArrayList<>();
        decode(cookie).forEach((item, qty) -> lines.add(item + " " + qty));
        System.out.println("cart: " + String.join(", ", lines));
    }
}
```

```output
add pen: cookie pen:1|2c06beca... (70 bytes)
add pen: cookie pen:2|bdba40d9... (70 bytes)
add ink: cookie pen:2,ink:1|8bdcc52b... (76 bytes)
a client changes ink:1 to ink:9: bad signature
cart: pen 2, ink 1
```

- Verdict: stdlib. `Mac.getInstance("HmacSHA256")` signs the data, `HexFormat` (Java 17 and later) writes the signature in hex, and `MessageDigest.isEqual` compares the signatures in constant time.
- The cart is a `LinkedHashMap`, and `merge` adds one to the quantity of an item.
