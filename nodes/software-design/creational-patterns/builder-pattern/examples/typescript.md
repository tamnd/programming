```typescript run id=request
interface RequestOptions {
  readonly method?: "GET" | "POST";
  readonly headers?: readonly string[];
  readonly timeout?: number;
  readonly retries?: number;
}

interface ApiRequest extends Required<RequestOptions> {
  readonly url: string;
}

function makeRequest(url: string, options: RequestOptions = {}): ApiRequest {
  const { method = "GET", headers = [], timeout = 30, retries = 0 } = options;
  if (timeout <= 0) {
    throw new RangeError("timeout must be positive");
  }
  return Object.freeze({ url, method, headers: [...headers], timeout, retries });
}

function show(request: ApiRequest): void {
  const headers = request.headers.join(", ");
  console.log(
    `${request.method} ${request.url} timeout=${request.timeout} retries=${request.retries} headers=[${headers}]`,
  );
}

show(makeRequest("https://example.com/orders"));
show(
  makeRequest("https://example.com/orders", {
    method: "POST",
    headers: ["Accept: application/json"],
    timeout: 5,
    retries: 2,
  }),
);
try {
  makeRequest("https://example.com/orders", { timeout: 0 });
} catch (error) {
  console.log(`error: ${(error as Error).message}`);
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: idiom. The options object of JavaScript, with a type. The product is named `ApiRequest`, because the DOM library of TypeScript already declares a global `Request`. Each optional part is a property marked `?`.
- The type checker rejects an object literal with a property that `RequestOptions` does not declare, so a misspelled option is a compile error, not a silent default.
- `Required<RequestOptions>` makes the product type from the options type, so each part is declared once.
- The method is a union of string literals, so `method: "PUT"` is a type error. A builder in Java needs an enum or a check in `build` for the same effect.
