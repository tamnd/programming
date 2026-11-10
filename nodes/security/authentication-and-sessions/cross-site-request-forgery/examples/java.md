```java run id=token
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.SecureRandom;
import java.util.HexFormat;
import java.util.Map;

public class Main {
    record Session(String user, String csrf) {}

    static final SecureRandom RANDOM = new SecureRandom();

    static String newToken() {
        byte[] bytes = new byte[32];
        RANDOM.nextBytes(bytes);
        return HexFormat.of().formatHex(bytes);
    }

    static final Map<String, Session> SESSIONS = Map.of("c0ffee", new Session("alice", newToken()));

    static String changeEmail(String cookie, String formToken, String email) {
        Session s = SESSIONS.get(cookie);
        if (s == null) {
            return "401 not logged in";
        }
        boolean match = MessageDigest.isEqual(
                formToken.getBytes(StandardCharsets.UTF_8),
                s.csrf().getBytes(StandardCharsets.UTF_8));
        if (!match) {
            return "403 missing or wrong CSRF token";
        }
        return "200 email of " + s.user() + " is " + email;
    }

    public static void main(String[] args) {
        System.out.println(changeEmail("c0ffee", SESSIONS.get("c0ffee").csrf(), "alice@example.net"));
        System.out.println(changeEmail("c0ffee", "", "mallory@attacker.example"));
    }
}
```

```output for=token
200 email of alice is alice@example.net
403 missing or wrong CSRF token
```

`SecureRandom` makes the token, and `MessageDigest.isEqual` compares the two byte arrays in a time that depends only on the length of the first one.

- Verdict: idiom. The Java standard library has an HTTP server but no CSRF check. Web frameworks such as Spring Security have one, and it is on by default there.
- Do not turn off the framework's CSRF protection for a browser application that uses cookies. CodeQL query `java/spring-disabled-csrf-protection` reports code that does.
- Use `SecureRandom` for tokens, never `java.util.Random`.
