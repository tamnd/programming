```javascript wrong id=collect-body
const http = require("node:http");

// The handler keeps every chunk, however many arrive.
const server = http.createServer((req, res) => {
  const chunks = [];
  req.on("data", (chunk) => chunks.push(chunk));
  req.on("end", () => {
    const body = Buffer.concat(chunks);
    res.end(`stored ${body.length} bytes`);
  });
});

server.listen(0, "127.0.0.1", async () => {
  const url = `http://127.0.0.1:${server.address().port}/`;
  // 3 MiB stands for a body of any size.
  const resp = await fetch(url, { method: "POST", body: "x".repeat(3 * 1024 * 1024) });
  console.log(resp.status, await resp.text());
  server.close();
});
```

```output for=collect-body
200 stored 3145728 bytes
```

The handler keeps every chunk until the end of the body, so the client decides how much memory the server uses.

```javascript run id=capped-body
const http = require("node:http");

const MAX_BODY = 1024 * 1024; // 1 MiB

const server = http.createServer((req, res) => {
  const chunks = [];
  let size = 0;
  let rejected = false;
  req.on("data", (chunk) => {
    if (rejected) return; // the rest is read and dropped, not kept
    size += chunk.length;
    if (size > MAX_BODY) {
      rejected = true;
      chunks.length = 0;
      res.writeHead(413);
      res.end(`body larger than ${MAX_BODY} bytes`);
      return;
    }
    chunks.push(chunk);
  });
  req.on("end", () => {
    if (!rejected) res.end(`stored ${Buffer.concat(chunks).length} bytes`);
  });
});

// Limits on time and on header size.
server.headersTimeout = 5_000;
server.requestTimeout = 30_000;
server.maxHeadersCount = 100;

server.listen(0, "127.0.0.1", async () => {
  const url = `http://127.0.0.1:${server.address().port}/`;
  for (const size of [3 * 1024 * 1024, 1000]) {
    const resp = await fetch(url, { method: "POST", body: "x".repeat(size) });
    console.log(resp.status, await resp.text());
  }
  server.close();
});
```

```output for=capped-body
413 body larger than 1048576 bytes
200 stored 1000 bytes
```

The handler counts the bytes, and it stops keeping them after 1 MiB. It answers 413 at once.

- Verdict: stdlib. `node:http` gives the chunks, and the handler counts them. Body parsers in web frameworks have a size limit option; check its value.
- The demo reads and drops the rest of a large body so that the client gets the 413 response. A server that closes the connection at once saves more, but a client that is still sending can get a write error before it reads the response.
- `server.headersTimeout` and `server.requestTimeout` limit the time to receive the headers and the whole request. `server.maxHeadersCount` limits the number of headers.
- Keep regular expressions that see untrusted input free of nested quantifiers, and limit the length of the input first, as in the Python example.
