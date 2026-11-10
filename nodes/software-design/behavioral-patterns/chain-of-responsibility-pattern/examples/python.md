```python run id=pipeline
from dataclasses import dataclass


@dataclass(frozen=True)
class Request:
    user: str | None
    path: str


# Each handler takes the request and the rest of the chain. It answers, or it calls next.
def log(request, next):
    print(f"log: {request.path} from {request.user or 'nobody'}")
    return next(request)


def auth(request, next):
    if request.user is None:
        return 401, "sign in first"
    return next(request)


def make_cache():
    saved = {}

    def cache(request, next):
        key = (request.user, request.path)
        if key in saved:
            status, body = saved[key]
            return status, body + " (cached)"
        saved[key] = next(request)
        return saved[key]

    return cache


def app(request):
    return 200, f"report for {request.user}"


def chain(handlers, last):
    # Build the chain from the end: each link holds the handler and the link after it.
    call = last
    for handler in reversed(handlers):
        call = lambda request, handler=handler, next=call: handler(request, next)
    return call


serve = chain([log, auth, make_cache()], app)
for request in [Request("ada", "/report"), Request(None, "/report"), Request("ada", "/report")]:
    status, body = serve(request)
    print(status, body)
```

```output
log: /report from ada
200 report for ada
log: /report from nobody
401 sign in first
log: /report from ada
200 report for ada (cached)
```

- Verdict: idiom. Each handler is a function that takes the request and `next`, the rest of the chain. This is the middleware form used by web frameworks.
- `chain` builds the links from the end. The default arguments `handler=handler, next=call` bind the values of this pass of the loop, because a closure reads a variable when it runs, not when it is made.
- A handler that returns without calling `next` ends the chain. `auth` does this for a request with no user, so the cache and the application never see it.
- The cache holds its dictionary in a closure, so each chain made with `make_cache()` has its own cache.
