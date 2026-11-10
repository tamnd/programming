```java run id=pipeline
import java.util.HashMap;
import java.util.Map;

public class Main {
    record Request(String user, String path) {}

    record Response(int status, String body) {}

    // The classic form: each handler holds a reference to its successor.
    abstract static class Handler {
        private Handler next;

        Handler then(Handler next) {
            this.next = next;
            return next;
        }

        Response handle(Request request) {
            if (next == null) {
                return new Response(404, "no handler for " + request.path());
            }
            return next.handle(request);
        }
    }

    static class Log extends Handler {
        @Override
        Response handle(Request request) {
            String user = request.user() == null ? "nobody" : request.user();
            System.out.println("log: " + request.path() + " from " + user);
            return super.handle(request);
        }
    }

    static class Auth extends Handler {
        @Override
        Response handle(Request request) {
            if (request.user() == null) {
                return new Response(401, "sign in first");
            }
            return super.handle(request);
        }
    }

    static class Cache extends Handler {
        private final Map<Request, Response> saved = new HashMap<>();

        @Override
        Response handle(Request request) {
            Response hit = saved.get(request);
            if (hit != null) {
                return new Response(hit.status(), hit.body() + " (cached)");
            }
            Response response = super.handle(request);
            saved.put(request, response);
            return response;
        }
    }

    static class App extends Handler {
        @Override
        Response handle(Request request) {
            return new Response(200, "report for " + request.user());
        }
    }

    public static void main(String[] args) {
        Handler first = new Log();
        first.then(new Auth()).then(new Cache()).then(new App());

        Request[] requests = {
            new Request("ada", "/report"), new Request(null, "/report"), new Request("ada", "/report"),
        };
        for (Request request : requests) {
            Response response = first.handle(request);
            System.out.println(response.status() + " " + response.body());
        }
    }
}
```

```output
log: /report from ada
200 report for ada
log: /report from nobody
401 sign in first
log: /report from ada
200 report for ada (cached)
```

- Verdict: idiom. This is the structure from Design Patterns: an abstract `Handler` with a successor field, and concrete handlers that override `handle`.
- The default `handle` in the base class passes the request to the successor. A concrete handler calls `super.handle` to pass it on, and returns its own `Response` to stop.
- `then` returns its argument, so the calls chain in the order that the requests flow.
- The base class answers 404 when there is no successor. Without this default, a request that no handler takes would fall off the end with no answer.
- A record has `equals` and `hashCode` from its components, so `Request` works as a key of the cache.
- The Servlet API has the same shape in `Filter` and `FilterChain`, but it is not part of the Java SE standard library.
