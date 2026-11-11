```javascript run id=gcd-loop
function gcd(a, b) {
  while (b !== 0) {
    [a, b] = [b, a % b]; // update both parts of the state at once
    console.log(a, b);
  }
  return a;
}

console.log(1071, 462);
console.log("gcd", gcd(1071, 462));
process.exitCode = 0;
```

```output for=gcd-loop
1071 462
462 147
147 21
21 0
gcd 21
```

```javascript wrong id=order-of-updates
function gcd(a, b) {
  while (b !== 0) {
    a = b; // a now holds the new value
    b = a % b; // so this computes b % b, which is 0
    console.log(a, b);
  }
  return a;
}

console.log(1071, 462);
console.log("gcd", gcd(1071, 462));
```

```output for=order-of-updates
1071 462
462 0
gcd 462
```

- Verdict: native. A JavaScript program is a script or a module that the engine runs from top to bottom, in Node.js from a file and in a browser from a `<script>` element. Destructuring assignment updates both parts of the state at once. `process.exitCode` sets the exit status in Node.js.
- The wrong block updates the state one variable at a time. After `a = b`, the old value of `a` is gone, so `a % b` is `b % b`, which is 0, and the loop stops after one step with the wrong answer.
