```javascript run id=endless-generator-and-labeled-break
// A generator with while (true) makes values only on request.
function* ids() {
  let n = 1;
  while (true) yield `id-${n++}`;
}
const gen = ids();
console.log(gen.next().value, gen.next().value, gen.next().value);

// for (;;) with a break: the first power of 2 over 1000.
let p = 1;
for (;;) {
  p *= 2;
  if (p > 1000) break;
}
console.log("power:", p);

// A labeled break leaves two endless loops at once.
let steps = 0;
outer: while (true) {
  while (true) {
    steps++;
    if (steps === 5) break outer;
  }
}
console.log("steps:", steps);
```

```output for=endless-generator-and-labeled-break
id-1 id-2 id-3
power: 1024
steps: 5
```

- Verdict: idiom. JavaScript writes an infinite loop as `while (true)` or `for (;;)`. The generator loops forever, but it stops at each `yield`, so the caller takes only three values. A plain `break` would leave only the inner loop, so the label `outer` names the loop to leave.
