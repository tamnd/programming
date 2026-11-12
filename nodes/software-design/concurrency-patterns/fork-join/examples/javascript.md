```javascript run id=fork-join
// The tasks share one thread. Each await lets another task run.
const THRESHOLD = 10000; // a range this small is summed directly
let leaves = 0;
let largest = 0;
const tick = () => new Promise((resolve) => setImmediate(resolve));

// Returns the sum of lo, lo + 1, ..., hi - 1.
async function solve(lo, hi) {
  if (hi - lo <= THRESHOLD) {
    await tick();
    leaves++;
    largest = Math.max(largest, hi - lo);
    let s = 0;
    for (let i = lo; i < hi; i++) s += i;
    return s;
  }
  const mid = Math.floor((lo + hi) / 2);
  const left = solve(lo, mid); // fork: start the left half
  const right = await solve(mid, hi); // solve the right half
  return (await left) + right; // join: wait for the left half
}

async function main() {
  const n = 1000000;
  const total = await solve(1, n + 1);
  console.log("sum:", total);
  console.log("leaf tasks:", leaves);
  console.log("largest leaf, at most 10000:", largest <= THRESHOLD ? "True" : "False");
  console.log("matches n(n+1)/2:", total === (n * (n + 1)) / 2 ? "True" : "False");
}

main();
```

```output
sum: 500000500000
leaf tasks: 128
largest leaf, at most 10000: True
matches n(n+1)/2: True
```

- Verdict: idiom. The tasks share one thread, so this shows the fork and join steps with promises but not parallel speed-up.
