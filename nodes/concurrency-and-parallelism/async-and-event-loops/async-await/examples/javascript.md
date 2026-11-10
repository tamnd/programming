```javascript run id=load
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

async function load(name, ms) {
  console.log("start", name);
  await sleep(ms);
  console.log("end", name);
  return name;
}

async function main() {
  const results = await Promise.all([load("slow", 200), load("fast", 100)]);
  console.log(results.join(", "));
}

main();
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. An `async function` returns a `Promise`, and `await` suspends the function until a promise settles.
- The body runs at once, up to the first `await`. The rest runs later, from the job queue of the host.
- `Promise.all` waits for all the promises and rejects as soon as one of them rejects. `Promise.allSettled` waits for all of them in every case.
- The browser, Node.js, Deno and Bun each give the event loop. The language gives only the job queue for promise reactions.
