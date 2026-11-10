```typescript run id=pipeline
type Request = { user: string | null; path: string };
type Response = { status: number; body: string };
type Next = (request: Request) => Response;
type Handler = (request: Request, next: Next) => Response;

const log: Handler = (request, next) => {
  console.log(`log: ${request.path} from ${request.user ?? "nobody"}`);
  return next(request);
};

const auth: Handler = (request, next) =>
  request.user === null ? { status: 401, body: "sign in first" } : next(request);

function makeCache(): Handler {
  const saved = new Map<string, Response>();
  return (request, next) => {
    const key = `${request.user} ${request.path}`;
    const hit = saved.get(key);
    if (hit !== undefined) {
      return { ...hit, body: `${hit.body} (cached)` };
    }
    const response = next(request);
    saved.set(key, response);
    return response;
  };
}

const app: Next = (request) => ({ status: 200, body: `report for ${request.user}` });

function chain(handlers: Handler[], last: Next): Next {
  return handlers.reduceRight<Next>((next, handler) => (request) => handler(request, next), last);
}

const serve = chain([log, auth, makeCache()], app);
const requests: Request[] = [
  { user: "ada", path: "/report" },
  { user: null, path: "/report" },
  { user: "ada", path: "/report" },
];
for (const request of requests) {
  const { status, body } = serve(request);
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

- Verdict: idiom. The types `Handler` and `Next` say what a link is: a function that receives the request and the rest of the chain, and must return a `Response`.
- The last link is a `Next`, not a `Handler`. It takes no successor, so it must answer, and a request cannot fall off the end of the chain.
- `reduceRight<Next>` names the type of the value that the fold builds: at each step, the rest of the chain is a `Next`.
