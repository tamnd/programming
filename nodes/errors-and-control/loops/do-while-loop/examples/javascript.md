```javascript run id=continue-goes-to-the-test-and-retry
// continue jumps to the condition, not to the top of the body.
let i = 0;
const seen = [];
do {
  i++;
  if (i % 2 === 0) continue;
  seen.push(i);
} while (i < 7);
console.log(seen.join(" "));

// A retry loop: the body runs before the first test.
const results = ["timeout", "timeout", "ok"];
let attempt = 0;
let result;
do {
  result = results[attempt];
  attempt++;
  console.log(`attempt ${attempt}: ${result}`);
} while (result !== "ok" && attempt < 5);

// The condition is false at the start, but the body runs once.
let n = 10;
do {
  console.log("n =", n);
  n++;
} while (n < 5);
```

```output for=continue-goes-to-the-test-and-retry
1 3 5 7
attempt 1: timeout
attempt 2: timeout
attempt 3: ok
n = 10
```

- Verdict: native. `do { } while (condition);` tests after the body. A `continue` goes to the test `i < 7`, so the loop does not skip its exit, and `result` is declared before the loop because the condition must see it.
