```javascript wrong id=fetch-any-url
import http from "node:http";

// A server on 127.0.0.1 that stands for an internal service.
const internal = http.createServer((req, res) => res.end("internal admin page: 3 users, debug mode on"));
await new Promise((resolve) => internal.listen(0, "127.0.0.1", resolve));
const admin = `http://127.0.0.1:${internal.address().port}`;

async function preview(url) {
  const response = await fetch(url);
  return response.text();
}

console.log(await preview(`${admin}/admin`));
internal.close();
```

```output for=fetch-any-url
internal admin page: 3 users, debug mode on
```

`fetch` sends the request to any host in the URL, and it follows redirects by default.

```javascript run id=allow-list
import http from "node:http";

async function start(handler) {
  const server = http.createServer(handler);
  await new Promise((resolve) => server.listen(0, "127.0.0.1", resolve));
  server.unref();
  return `http://127.0.0.1:${server.address().port}`;
}

const admin = await start((req, res) => res.end("internal admin page: 3 users, debug mode on"));
const media = await start((req, res) => {
  if (req.url === "/moved") {
    res.writeHead(302, { Location: `${admin}/admin` }).end();
  } else {
    res.end("a picture of a cat");
  }
});

// The operator chooses the destinations. In production, this set holds
// origins such as https://media.example.com.
const ALLOWED_ORIGINS = new Set([media]);

async function preview(input) {
  let url;
  try {
    url = new URL(input);
  } catch {
    return "refused: not a URL";
  }
  if (!ALLOWED_ORIGINS.has(url.origin)) {
    return "refused: destination not allowed";
  }
  try {
    const response = await fetch(url, { redirect: "error" });
    return await response.text();
  } catch {
    return "refused: the request failed or redirected";
  }
}

console.log(await preview(`${media}/cat.txt`));
console.log(await preview(`${admin}/admin`));
console.log(await preview("file:///tmp/settings.ini"));
console.log(await preview(`${media}/moved`));
```

```output for=allow-list
a picture of a cat
refused: destination not allowed
refused: destination not allowed
refused: the request failed or redirected
```

`new URL` parses the input, and `url.origin` holds the scheme, the host and the port in a normal form. The check compares it with a set of allowed origins. `redirect: "error"` makes `fetch` reject the promise when the server answers with a redirect.

- Verdict: stdlib. Node.js has `fetch` and the `URL` class as globals. The default of the `redirect` option is `follow`, so set it to `error` or `manual` for requests to destinations from input.
- Compare `url.origin` or `url.hostname`, not the input text. A check with `startsWith` on the URL text is weak, because the same host has many spellings.
- `fetch` in Node.js has no hook that checks the resolved address before the connection. For a feature that must reach any public host, check the address at connect time. One way is a custom `lookup` function for `node:http` that resolves the name with `node:dns` and refuses internal addresses. Another way is an egress proxy that applies the rules.
- CodeQL has `js/request-forgery`.
