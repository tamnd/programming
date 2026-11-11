```javascript run id=promises
const ready = new Promise((resolve) => resolve("ready"));
ready.then((value) => console.log("then:", value));
console.log("sync code runs first");

const delay = (ms, value) => new Promise((resolve) => setTimeout(() => resolve(value), ms));

const results = await Promise.all([delay(20, "a"), delay(10, "b")]);
console.log(results);

const settled = await Promise.allSettled([delay(5, "ok"), Promise.reject(new Error("failed"))]);
console.log(settled.map((r) => r.status));
```

```output for=promises
sync code runs first
then: ready
[ 'a', 'b' ]
[ 'fulfilled', 'rejected' ]
```

- Verdict: native. A `Promise` is pending, fulfilled or rejected. The executor function passed to the constructor gets `resolve` and `reject`, which are the promise side. `then` and `await` are the future side.
- A handler always runs after the current code, as a microtask, even when the promise is already fulfilled.
- `Promise.all` keeps the order of its inputs and rejects as soon as one input rejects. `Promise.allSettled` waits for all of them.

```javascript wrong id=foreach
const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

const saved = [];
[1, 2, 3].forEach(async (n) => {
  await delay(5);
  saved.push(n);
});
console.log("saved:", saved.length);
```

```output for=foreach
saved: 0
```

`forEach` ignores the promises that the async callback returns, so the code after it does not wait. Use `for (const n of items) await ...` for one at a time, or `await Promise.all(items.map(async (n) => ...))` for all at once.
