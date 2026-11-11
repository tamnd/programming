```javascript run id=closures-and-stack-limit
function makeCounter() {
  let count = 0; // kept alive by the closure after the call returns
  return () => ++count;
}
const c = makeCounter();
c();
console.log(c());

function depth(n) {
  try {
    return depth(n + 1);
  } catch (e) {
    return [n, e];
  }
}
const [n, err] = depth(0);
console.log(n > 1000, err instanceof RangeError, err.message);

function sum(n) {
  let total = 0;
  for (let i = 1; i <= n; i++) total += i; // a loop uses one frame
  return total;
}
console.log(sum(1_000_000));
```

```output for=closures-and-stack-limit
2
true true Maximum call stack size exceeded
500000500000
```

- Verdict: native. A JavaScript engine pushes an execution context on the call stack for each call and keeps objects on a heap that the garbage collector manages. A variable captured by a closure stays alive after its function returns, so `count` keeps its value between calls.
- The size of the stack is up to the engine. When recursion goes too deep, the engine throws a `RangeError` with the message `Maximum call stack size exceeded`, and the program can catch it. A loop uses one frame however many times it runs.
