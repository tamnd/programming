```javascript run id=trace
function f(name, value) {
  console.log("evaluate", name);
  return value;
}

console.log(f("a", 2) ** f("b", 3) ** f("c", 2));

let i = 0;
const values = [i++, i++, i];
console.log(values);
```

```output for=trace
evaluate a
evaluate b
evaluate c
512
[ 0, 1, 2 ]
```

- Verdict: native. ECMAScript evaluates the operands of an operator from left to right. Precedence and associativity only group the operators. `**` groups from the right, so the value is `2 ** (3 ** 2)`, but `a` is still evaluated first.
- The elements of an array literal and the arguments of a call are evaluated from left to right, with each side effect complete before the next element.

```javascript wrong id=await-plus
let total = 0;

async function add(amount) {
  total += await Promise.resolve(amount);
}

await Promise.all([add(1), add(2)]);
console.log(total);
```

```output for=await-plus
2
```

`total += x` reads `total` before it evaluates the right-hand side. Both calls read 0, then wait. Each one then writes its own sum, and the second write replaces the first. Await the value into a local variable first, then add it: `const n = await ...; total += n;`.
