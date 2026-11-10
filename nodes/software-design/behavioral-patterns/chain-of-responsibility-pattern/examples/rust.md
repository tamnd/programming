```rust run id=pipeline
use std::cell::RefCell;
use std::collections::HashMap;

#[derive(Clone, PartialEq, Eq, Hash)]
struct Request {
    user: Option<String>,
    path: String,
}

type Response = (u16, String);

trait Handler {
    fn handle(&self, request: &Request, next: Next) -> Response;
}

// Next is the rest of the chain: the handlers that are left, and the last step.
#[derive(Clone, Copy)]
struct Next<'a> {
    rest: &'a [Box<dyn Handler>],
    last: &'a dyn Fn(&Request) -> Response,
}

impl Next<'_> {
    fn run(self, request: &Request) -> Response {
        match self.rest.split_first() {
            Some((first, rest)) => first.handle(request, Next { rest, last: self.last }),
            None => (self.last)(request),
        }
    }
}

struct Log;
impl Handler for Log {
    fn handle(&self, request: &Request, next: Next) -> Response {
        let user = request.user.as_deref().unwrap_or("nobody");
        println!("log: {} from {}", request.path, user);
        next.run(request)
    }
}

struct Auth;
impl Handler for Auth {
    fn handle(&self, request: &Request, next: Next) -> Response {
        match request.user {
            None => (401, "sign in first".to_string()),
            Some(_) => next.run(request),
        }
    }
}

#[derive(Default)]
struct Cache {
    saved: RefCell<HashMap<Request, Response>>,
}
impl Handler for Cache {
    fn handle(&self, request: &Request, next: Next) -> Response {
        if let Some((status, body)) = self.saved.borrow().get(request) {
            return (*status, format!("{body} (cached)"));
        }
        let response = next.run(request);
        self.saved.borrow_mut().insert(request.clone(), response.clone());
        response
    }
}

fn app(request: &Request) -> Response {
    (200, format!("report for {}", request.user.as_deref().unwrap_or("")))
}

fn main() {
    let handlers: Vec<Box<dyn Handler>> = vec![Box::new(Log), Box::new(Auth), Box::new(Cache::default())];
    let serve = Next { rest: &handlers, last: &app };
    for user in [Some("ada"), None, Some("ada")] {
        let request = Request { user: user.map(String::from), path: "/report".to_string() };
        let (status, body) = serve.run(&request);
        println!("{status} {body}");
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

- Verdict: idiom. The handlers live in a `Vec`, and `Next` is a view of the rest of the chain: a slice of the handlers that are left and the last step. A handler calls `next.run` to pass the request on.
- `Next` is `Copy`, because it holds only two references. No handler owns its successor, so the chain needs no `Rc` and no `Option<Box<...>>` field. See [[rust/ownership|ownership]].
- `handle` takes `&self`, so the cache keeps its map in a `RefCell` to change it. The `borrow()` in the `if let` ends before `next.run`, so the `borrow_mut()` later does not panic.
- Outside the standard library, the `tower` crate defines middleware as a `Layer` that wraps a `Service`. That is the shape of the Go example.
