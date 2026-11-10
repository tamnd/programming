```javascript run id=request
function makeRequest(url, { method = "GET", headers = [], timeout = 30, retries = 0 } = {}) {
  if (timeout <= 0) {
    throw new RangeError("timeout must be positive");
  }
  return Object.freeze({ url, method, headers: Object.freeze([...headers]), timeout, retries });
}

function show(request) {
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
  console.log(`error: ${error.message}`);
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: idiom. An options object with destructuring and default values gives named, optional parts. The required part, the URL, stays a positional parameter.
- A default value in JavaScript is evaluated at each call that leaves the argument out, so `headers = []` gives each call a fresh array. Python evaluates a default once.
- `= {}` after the pattern lets a caller leave out the whole options object.
- A misspelled option, such as `timout: 5`, is ignored with no error. TypeScript catches it, because an object literal may not have properties that its type does not declare.
- `Object.freeze` makes the result read-only, so the product does not change after it is made.
