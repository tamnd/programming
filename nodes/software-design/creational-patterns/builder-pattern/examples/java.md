```java run id=request
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Request(String method, String url, List<String> headers, int timeout, int retries) {}

    static final class RequestBuilder {
        private final String url;
        private String method = "GET";
        private final List<String> headers = new ArrayList<>();
        private int timeout = 30;
        private int retries = 0;

        RequestBuilder(String url) {
            this.url = url;
        }

        RequestBuilder method(String method) {
            this.method = method;
            return this;
        }

        RequestBuilder header(String header) {
            headers.add(header);
            return this;
        }

        RequestBuilder timeout(int seconds) {
            this.timeout = seconds;
            return this;
        }

        RequestBuilder retries(int retries) {
            this.retries = retries;
            return this;
        }

        Request build() {
            if (timeout <= 0) {
                throw new IllegalStateException("timeout must be positive");
            }
            return new Request(method, url, List.copyOf(headers), timeout, retries);
        }
    }

    static void show(Request request) {
        System.out.printf("%s %s timeout=%d retries=%d headers=[%s]%n", request.method(), request.url(),
                request.timeout(), request.retries(), String.join(", ", request.headers()));
    }

    public static void main(String[] args) {
        show(new RequestBuilder("https://example.com/orders").build());
        show(new RequestBuilder("https://example.com/orders")
                .method("POST")
                .header("Accept: application/json")
                .timeout(5)
                .retries(2)
                .build());
        try {
            new RequestBuilder("https://example.com/orders").timeout(0).build();
        } catch (IllegalStateException error) {
            System.out.println("error: " + error.getMessage());
        }
    }
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: idiom. Java has no named or default arguments, so the builder of Effective Java is the usual form for a class with many optional parts. The required part, the URL, is a parameter of the builder's constructor.
- Each setter returns `this`, so the calls chain, and each call names the part that it sets. `timeout(5).retries(2)` cannot be read the wrong way, as `new Request(url, 5, 2)` can.
- `build` checks the parts as a whole and then makes an immutable record. `List.copyOf` copies the headers, so a later `header` call on the builder does not change a request that was already built.
- `java.net.http.HttpRequest.newBuilder` in the standard library has the same shape: setters that return the builder, and a `build` that returns an immutable request.
