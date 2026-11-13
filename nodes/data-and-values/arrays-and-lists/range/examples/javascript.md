```javascript run id=array-from-and-generator
// JavaScript has no range type. Array.from with a length and a map function is the common idiom.
console.log(Array.from({ length: 5 }, (_, i) => i), [...Array(3).keys()]);

// A generator gives a lazy half-open range with a step.
function* range(start, stop, step = 1) {
  if (step === 0) throw new RangeError("step must not be 0");
  for (let i = start; step > 0 ? i < stop : i > stop; i += step) yield i;
}
console.log([...range(2, 7)], [...range(10, 0, -3)], [...range(5, 5)]);

// for...of over the generator does not build an array, so a large range costs no memory.
let total = 0;
for (const i of range(0, 1_000_000)) total += i;
console.log(total);

// Iterator.range is a TC39 proposal. This runtime may not have it yet.
console.log(typeof Iterator.range);
```

```output
[ 0, 1, 2, 3, 4 ] [ 0, 1, 2 ]
[ 2, 3, 4, 5, 6 ] [ 10, 7, 4, 1 ] []
499999500000
undefined
```

- Verdict: idiom. JavaScript has no range type. Array.from with a length and Array(n).keys() make arrays of indices. A generator gives a lazy half-open range with a step, and a loop over it builds no array. Iterator.range is a TC39 proposal, and this runtime does not have it.
