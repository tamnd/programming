```javascript run id=five-loop-statements-and-labels
const out = [];
for (let i = 0; i < 3; i++) out.push(i);
console.log("for:", out.join(" "));

let n = 100;
let steps = 0;
while (n > 1) {
  n = Math.floor(n / 2);
  steps++;
}
console.log("while:", steps);

// The body of do...while runs once even when the condition is false.
let tries = 0;
do {
  tries++;
} while (tries < 0);
console.log("do...while:", tries);

for (const ch of "abc") console.log("for...of:", ch);
for (const key in { x: 1, y: 2 }) console.log("for...in:", key);

[10, 20].forEach((v, i) => console.log("forEach:", i, v));

// A label lets break leave the outer loop.
outer: for (let a = 0; a < 3; a++) {
  for (let b = 0; b < 3; b++) {
    if (a * b === 2) {
      console.log("break outer at", a, b);
      break outer;
    }
  }
}
```

```output for=five-loop-statements-and-labels
for: 0 1 2
while: 6
do...while: 1
for...of: a
for...of: b
for...of: c
for...in: x
for...in: y
forEach: 0 10
forEach: 1 20
break outer at 1 2
```

- Verdict: native. JavaScript has five loop statements: `for`, `while`, `do...while`, `for...of` over the values of an iterable and `for...in` over the property names of an object. The `do...while` body runs once although its condition is false at the start. `forEach` takes the body as a function. `break outer` leaves both loops at the first pair whose product is 2.
