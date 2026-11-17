```javascript run id=deep
function depth(n) {
  return n === 0 ? 0 : 1 + depth(n - 1); // one frame for each step
}

function loop(n) {
  let count = 0;
  while (n > 0) { // one frame for the whole loop
    n--;
    count++;
  }
  return count;
}

console.log("depth(100) =", depth(100));
try {
  depth(1_000_000);
} catch (e) {
  console.log("depth(1000000):", e.constructor.name);
}
console.log("loop(1000000) =", loop(1_000_000));
```

```output for=deep
depth(100) = 100
depth(1000000): RangeError
loop(1000000) = 1000000
```

- Verdict: native. The engine has a fixed stack and throws an error when it runs out. Here it is `RangeError`, as in Chrome and Safari. Firefox throws `InternalError` instead.
- The program can catch the error and go on.
