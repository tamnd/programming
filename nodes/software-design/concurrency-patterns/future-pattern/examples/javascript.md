```javascript run id=future-pattern
const PRICES = { apple: 12, bread: 7, cheese: 30 };

// The lookups wait here with the caller, so the output does not depend on timing.
let arrived = 0;
let openMeeting;
const meeting = new Promise((resolve) => (openMeeting = resolve));
function arrive() {
  arrived++;
  if (arrived === 4) openMeeting();
  return meeting;
}

let settled = 0;
async function lookup(item) {
  await arrive(); // continues when all 3 lookups and the caller are here
  return PRICES[item];
}

async function main() {
  // Each call returns a promise at once.
  const futures = Object.keys(PRICES).map((item) => lookup(item));
  for (const f of futures) f.then(() => settled++);
  console.log("lookups started:", futures.length);
  console.log("done right after the calls:", settled > 0 ? "True" : "False");
  await arrive(); // the caller is free to do other work here
  console.log("all three lookups ran at once:", arrived === 4 ? "True" : "False");
  const prices = await Promise.all(futures); // wait for each result

  console.log("prices: [" + prices.join(", ") + "]");
  console.log("total:", prices.reduce((a, b) => a + b, 0));
}

main();
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: native. An `async` function returns a `Promise` at once, and `await` and `Promise.all` wait for the values. The lookups are tasks on one event loop, so they wait at the same point but do not run in parallel.
