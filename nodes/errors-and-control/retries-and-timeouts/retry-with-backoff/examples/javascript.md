```javascript run id=retry-after
function seeded(seed) {
  let s = seed >>> 0;
  return () => {
    s = (Math.imul(s, 1664525) + 1013904223) >>> 0;
    return s / 2 ** 32;
  };
}

const clock = {
  now: 0,
  async sleep(ms) {
    this.now += ms;
  },
};

const responses = [
  { status: 503, retryAfter: 2 },
  { status: 503 },
  { status: 200, body: "report.pdf" },
];
async function fetchReport() {
  return responses.shift();
}

async function withRetry(op, { attempts = 5, base = 100, cap = 5000, random }) {
  for (let attempt = 0; attempt < attempts; attempt++) {
    const res = await op();
    if (res.status < 500 && res.status !== 429) return res;
    if (attempt === attempts - 1) return res;
    const backoff = Math.floor(random() * Math.min(cap, base * 2 ** attempt));
    const hint = res.retryAfter === undefined ? 0 : res.retryAfter * 1000;
    const delay = Math.max(backoff, hint);
    console.log(`t=${clock.now} ms: status ${res.status}; wait ${delay} ms`);
    await clock.sleep(delay);
  }
}

const res = await withRetry(fetchReport, { random: seeded(7) });
console.log(`t=${clock.now} ms: status ${res.status}, body ${res.body}`);
```

```output
t=0 ms: status 503; wait 2000 ms
t=2000 ms: status 503; wait 182 ms
t=2182 ms: status 200, body report.pdf
```

- Verdict: stdlib. JavaScript has no retry function in the language or in Node.js. The loop is ordinary code around an `async` call.
- A server can send the header `Retry-After` with a `503` response to say how long the service is expected to be unavailable. The value is a date or a number of seconds. The client here waits for the longer of the hint and its own backoff.
- `Math.random` cannot take a seed, so the example uses a small linear congruential generator. Its output is the same on each run.
