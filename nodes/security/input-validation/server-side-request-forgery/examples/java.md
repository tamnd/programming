```java run id=allow-list
import com.sun.net.httpserver.HttpServer;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.util.Set;

public class Main {
    static String start(String body, String location) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        server.createContext("/", exchange -> {
            byte[] bytes = body.getBytes(StandardCharsets.UTF_8);
            if (location != null && exchange.getRequestURI().getPath().equals("/moved")) {
                exchange.getResponseHeaders().add("Location", location);
                exchange.sendResponseHeaders(302, -1);
            } else {
                exchange.sendResponseHeaders(200, bytes.length);
                exchange.getResponseBody().write(bytes);
            }
            exchange.close();
        });
        server.start();
        return "http://127.0.0.1:" + server.getAddress().getPort();
    }

    // NEVER is the default policy. The code sets it so that a reader sees it.
    static final HttpClient CLIENT = HttpClient.newBuilder()
            .followRedirects(HttpClient.Redirect.NEVER)
            .proxy(HttpClient.Builder.NO_PROXY)
            .build();

    static Set<String> allowed;

    static String preview(String input) throws Exception {
        URI uri = URI.create(input);
        String origin = uri.getScheme() + "://" + uri.getHost() + ":" + uri.getPort();
        if (!allowed.contains(origin)) {
            return "refused: destination not allowed";
        }
        HttpRequest request = HttpRequest.newBuilder(uri).build();
        HttpResponse<String> response = CLIENT.send(request, HttpResponse.BodyHandlers.ofString());
        if (response.statusCode() != 200) {
            return "refused: upstream answered " + response.statusCode();
        }
        return response.body();
    }

    public static void main(String[] args) throws Exception {
        String admin = start("internal admin page: 3 users, debug mode on", null);
        String media = start("a picture of a cat", admin + "/admin");
        allowed = Set.of(media);

        System.out.println(preview(media + "/cat.txt"));
        System.out.println(preview(admin + "/admin"));
        System.out.println(preview("file:///tmp/settings.ini"));
        System.out.println(preview(media + "/moved"));
        System.exit(0);
    }
}
```

```output for=allow-list
a picture of a cat
refused: destination not allowed
refused: destination not allowed
refused: upstream answered 302
```

Without the check in `preview`, `CLIENT.send` returns the internal page. The check builds the origin from the parsed `URI` and compares it with the allowed set. The redirect stops at the first response, because the policy is `NEVER`.

- Verdict: stdlib. `java.net.http.HttpClient` uses the redirect policy `NEVER` unless the program chooses another one. `Redirect.NORMAL` and `Redirect.ALWAYS` follow redirects, so a client with one of them needs the destination check on each step.
- `java.net.URL.openStream` opens `file:` and `jar:` URLs as well as `http:` URLs. Do not pass a URL from input to it.
- `URI.getHost` returns null for a URL with no host, and the check then fails, as it should.
- XML parsers in Java can fetch external entities and DTDs from URLs in a document. Turn off external entities for input from outside, because they are another route to the same requests.
- CodeQL has `java/ssrf`.
