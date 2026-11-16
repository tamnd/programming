```javascript run id=truncated
const yesNo = (b) => (b ? "yes" : "no");
const floorMod = (a, n) => ((a % n) + n) % n;

console.log(`-7 / 3: ${-7 / 3}`);
console.log(`Math.trunc(-7 / 3): ${Math.trunc(-7 / 3)}`);
console.log(`Math.floor(-7 / 3): ${Math.floor(-7 / 3)}`);
console.log(`-7 % 3: ${-7 % 3}`);
console.log(`floorMod(-7, 3): ${floorMod(-7, 3)}`);
console.log(`-7n / 3n: ${-7n / 3n}`);
console.log(`-7 % 2 === 1: ${yesNo(-7 % 2 === 1)}`);
console.log(`-7 % 2 !== 0: ${yesNo(-7 % 2 !== 0)}`);
```

```output for=truncated
-7 / 3: -2.3333333333333335
Math.trunc(-7 / 3): -2
Math.floor(-7 / 3): -3
-7 % 3: -1
floorMod(-7, 3): 2
-7n / 3n: -2
-7 % 2 === 1: no
-7 % 2 !== 0: yes
```

- Verdict: idiom. JavaScript has no integer division for numbers, so a program uses `Math.trunc` or `Math.floor` on the float quotient. `%` is truncated, and a floored remainder needs a helper.
- `BigInt` division truncates toward zero.
- The odd test `-7 % 2 === 1` fails, because the remainder is -1.
