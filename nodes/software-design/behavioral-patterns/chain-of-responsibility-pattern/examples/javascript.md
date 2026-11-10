```javascript run id=pipeline
// Each handler takes the request and next, the rest of the chain.
const log = (request, next) => {
  console.log(`log: ${request.path} from ${request.user ?? "nobody"}`);
  return next(request);
};

const auth = (request, next) => (request.user ? next(request) : [401, "sign in first"]);

function makeCache() {
  const saved = new Map();
  return (request, next) => {
    const key = `${request.user} ${request.path}`;
    if (saved.has(key)) {
      const [status, body] = saved.get(key);
      return [status, `${body} (cached)`];
    }
    const response = next(request);
    saved.set(key, response);
    return response;
  };
}

const app = (request) => [200, `report for ${request.user}`];

// reduceRight builds the chain from the end, so the first handler runs first.
const chain = (handlers, last) =>
  handlers.reduceRight((next, handler) => (request) => handler(request, next), last);

const serve = chain([log, auth, makeCache()], app);
for (const request of [
  { user: "ada", path: "/report" },
  { user: null, path: "/report" },
  { user: "ada", path: "/report" },
]) {
  const [status, body] = serve(request);
  console.log(status, body);
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

- Verdict: idiom. A handler is a function of the request and `next`. Express and Koa use this shape for middleware, with a response object or a context in place of the return value.
- `reduceRight` folds the list from the end. Each step wraps the chain so far in one more handler.
- Each call of the arrow function in `reduceRight` has its own `next` and `handler` parameters, so the links do not share a variable. The Python example needs default arguments to get the same effect.
