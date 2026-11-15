```typescript run id=pair
const yn = (b: boolean): string => (b ? "yes" : "no");

function minMax(values: number[]): [lo: number, hi: number] {
  return [Math.min(...values), Math.max(...values)];
}

const pair = minMax([7, 2, 9, 4]);
console.log("pair: " + JSON.stringify(pair));
const [lo, hi] = pair;
console.log(`lo: ${lo} hi: ${hi}`);
console.log(`second part: ${pair[1]}`);

const other: [number, number] = [2, 9];
const samePair = (a: readonly number[], b: readonly number[]): boolean =>
  a.length === b.length && a.every((x, i) => x === b[i]);
console.log("equal pairs: " + yn(samePair(pair, other)));
console.log(`length in the type: ${pair.length}`);
```

```output for=pair
pair: [2,9]
lo: 2 hi: 9
second part: 9
equal pairs: yes
length in the type: 2
```

- Verdict: native. A tuple type such as `[lo: number, hi: number]` fixes the number of parts and the type of each one.
- The type checker knows that `pair.length` is `2` and refuses `pair[2]`.
- At run time a tuple is a plain array.
