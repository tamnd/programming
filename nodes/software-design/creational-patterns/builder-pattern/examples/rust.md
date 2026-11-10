```rust run id=request
#[derive(Debug)]
struct Request {
    method: String,
    url: String,
    headers: Vec<String>,
    timeout: u32,
    retries: u32,
}

struct RequestBuilder {
    method: String,
    url: String,
    headers: Vec<String>,
    timeout: u32,
    retries: u32,
}

impl RequestBuilder {
    fn new(url: &str) -> Self {
        RequestBuilder { method: "GET".into(), url: url.into(), headers: Vec::new(), timeout: 30, retries: 0 }
    }

    fn method(&mut self, method: &str) -> &mut Self {
        self.method = method.into();
        self
    }

    fn header(&mut self, header: &str) -> &mut Self {
        self.headers.push(header.into());
        self
    }

    fn timeout(&mut self, seconds: u32) -> &mut Self {
        self.timeout = seconds;
        self
    }

    fn retries(&mut self, retries: u32) -> &mut Self {
        self.retries = retries;
        self
    }

    fn build(&self) -> Result<Request, String> {
        if self.timeout == 0 {
            return Err("timeout must be positive".into());
        }
        Ok(Request {
            method: self.method.clone(),
            url: self.url.clone(),
            headers: self.headers.clone(),
            timeout: self.timeout,
            retries: self.retries,
        })
    }
}

fn show(r: &Request) {
    println!("{} {} timeout={} retries={} headers=[{}]", r.method, r.url, r.timeout, r.retries, r.headers.join(", "));
}

fn main() {
    show(&RequestBuilder::new("https://example.com/orders").build().unwrap());
    let mut builder = RequestBuilder::new("https://example.com/orders");
    builder.method("POST").header("Accept: application/json").timeout(5);
    let flaky_network = true;
    if flaky_network {
        builder.retries(2); // a part set under a condition
    }
    show(&builder.build().unwrap());
    if let Err(error) = RequestBuilder::new("https://example.com/orders").timeout(0).build() {
        println!("error: {error}");
    }
    println!("{:?}", builder.build().map(|r| r.retries));
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
Ok(2)
```

- Verdict: idiom. Rust has no named, default or optional arguments, and the Rust API Guidelines recommend a builder for values with many inputs or optional configuration. `std::process::Command` and `std::thread::Builder` in the standard library are builders.
- The setters take and return `&mut Self`. This is the non-consuming form that the guidelines prefer: it allows a one-line chain and also a builder in a variable that gets parts under a condition.
- `build` takes `&self` and clones the parts, so the builder stays usable and the request owns its own data. The last line builds a second request from the same builder.
- `build` returns a `Result`, so the caller must handle an invalid configuration. A typestate builder, with one type for each stage, can make a missing required part a compile error.
