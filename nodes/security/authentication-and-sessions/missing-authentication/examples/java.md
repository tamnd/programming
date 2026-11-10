```java wrong id=admin-context
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import java.util.List;

public class Main {
    static final List<String> USERS = new ArrayList<>(List.of("ana", "bao", "chi"));
    static final String TOKEN = java.util.UUID.randomUUID().toString();

    static void reply(HttpExchange ex, int code, String body) throws IOException {
        byte[] b = body.getBytes();
        ex.sendResponseHeaders(code, b.length);
        ex.getResponseBody().write(b);
        ex.close();
    }

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        server.createContext("/users", ex -> {
            if (!("Bearer " + TOKEN).equals(ex.getRequestHeaders().getFirst("Authorization"))) {
                reply(ex, 401, "login required");
                return;
            }
            reply(ex, 200, USERS.toString());
        });
        // Added later for the support team. It has no check.
        server.createContext("/admin/delete-user", ex -> {
            String name = ex.getRequestURI().getQuery().replace("name=", "");
            USERS.remove(name);
            reply(ex, 200, "deleted " + name);
        });
        server.start();

        HttpClient client = HttpClient.newBuilder().proxy(HttpClient.Builder.NO_PROXY).build();
        String base = "http://127.0.0.1:" + server.getAddress().getPort();
        for (String path : List.of("/users", "/admin/delete-user?name=bao")) {
            HttpResponse<String> r = client.send(
                HttpRequest.newBuilder(URI.create(base + path)).POST(HttpRequest.BodyPublishers.noBody()).build(),
                HttpResponse.BodyHandlers.ofString());
            System.out.println(path + " " + r.statusCode() + " " + r.body());
        }
        System.out.println(USERS);
        server.stop(0);
    }
}
```

```output for=admin-context
/users 401 login required
/admin/delete-user?name=bao 200 deleted bao
[ana, chi]
```

The context `/users` checks the token in its handler. The context `/admin/delete-user` was added later, and its handler has no check. The first request with no token gets 401. The second one deletes a user.

```java run id=authenticator
import com.sun.net.httpserver.Authenticator;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpPrincipal;
import com.sun.net.httpserver.HttpServer;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class Main {
    static final List<String> USERS = new ArrayList<>(List.of("ana", "bao", "chi"));
    static final String TOKEN = java.util.UUID.randomUUID().toString();
    static final Set<String> PUBLIC = Set.of("/health");

    // The authenticator runs before the handler of the context.
    static final Authenticator BEARER = new Authenticator() {
        @Override
        public Result authenticate(HttpExchange ex) {
            String got = ex.getRequestHeaders().getFirst("Authorization");
            byte[] a = String.valueOf(got).getBytes(StandardCharsets.UTF_8);
            byte[] b = ("Bearer " + TOKEN).getBytes(StandardCharsets.UTF_8);
            if (MessageDigest.isEqual(a, b)) {
                return new Success(new HttpPrincipal("operator", "app"));
            }
            return new Failure(401);
        }
    };

    // Every route goes through this method. A route is protected unless it is in PUBLIC.
    static void route(HttpServer server, String path, HttpHandler handler) {
        var context = server.createContext(path, handler);
        if (!PUBLIC.contains(path)) {
            context.setAuthenticator(BEARER);
        }
    }

    static void reply(HttpExchange ex, int code, String body) throws IOException {
        byte[] b = body.getBytes(StandardCharsets.UTF_8);
        ex.sendResponseHeaders(code, b.length);
        ex.getResponseBody().write(b);
        ex.close();
    }

    public static void main(String[] args) throws Exception {
        HttpServer server = HttpServer.create(new InetSocketAddress("127.0.0.1", 0), 0);
        route(server, "/health", ex -> reply(ex, 200, "ok"));
        route(server, "/users", ex -> reply(ex, 200, USERS.toString()));
        route(server, "/admin/delete-user", ex -> {
            String name = ex.getRequestURI().getQuery().replace("name=", "");
            USERS.remove(name);
            reply(ex, 200, "deleted " + name);
        });
        server.start();

        HttpClient client = HttpClient.newBuilder().proxy(HttpClient.Builder.NO_PROXY).build();
        String base = "http://127.0.0.1:" + server.getAddress().getPort();
        String[][] calls = {
            {"/health", ""},
            {"/users", ""},
            {"/admin/delete-user?name=bao", ""},
            {"/admin/delete-user?name=bao", "Bearer " + TOKEN},
        };
        for (String[] call : calls) {
            var builder = HttpRequest.newBuilder(URI.create(base + call[0]))
                .POST(HttpRequest.BodyPublishers.noBody());
            if (!call[1].isEmpty()) {
                builder.header("Authorization", call[1]);
            }
            HttpResponse<String> r = client.send(builder.build(), HttpResponse.BodyHandlers.ofString());
            String who = call[1].isEmpty() ? "no token  " : "with token";
            System.out.println(who + " " + call[0] + " " + r.statusCode() + " " + r.body());
            if (call[0].startsWith("/admin")) {
                System.out.println(USERS);
            }
        }
        server.stop(0);
    }
}
```

```output for=authenticator
no token   /health 200 ok
no token   /users 401
no token   /admin/delete-user?name=bao 401
[ana, bao, chi]
with token /admin/delete-user?name=bao 200 deleted bao
[ana, chi]
```

The method `route` is the only way to add a context. It sets the authenticator on every context that is not in `PUBLIC`. The handlers no longer contain a check, so a new handler cannot forget it.

- Verdict: stdlib. `com.sun.net.httpserver` in the module `jdk.httpserver` has the server, and `HttpContext.setAuthenticator` applies an `Authenticator` to every request of a context. `java.net.http.HttpClient` is the client.
- The documentation of `setAuthenticator` says that once an authenticator is set on a context, all client requests must be authenticated. The package also has `BasicAuthenticator` for HTTP Basic authentication.
- `MessageDigest.isEqual` compares the token. Its implementation note says that the time depends only on the length of the first array, not on the contents of either array.
- The token is made at start for the demonstration. A real service reads it from a secret store. See [[hard-coded-credentials]].
