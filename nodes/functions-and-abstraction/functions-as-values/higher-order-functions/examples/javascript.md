```javascript run id=hof
const twice = (f, x) => f(f(x));
const compose = (f, g) => (x) => f(g(x));

const words = ["pear", "fig", "banana"];
console.log(twice((n) => n * 3, 2));
console.log(words.map((w) => w.length));
console.log(words.filter((w) => w.length > 3));
console.log([1, 2, 3, 4].reduce((total, n) => total + n, 0));
console.log([...words].sort((a, b) => a.length - b.length));
console.log([10, 9, 1].sort());
console.log([10, 9, 1].sort((a, b) => a - b));
const addThenDouble = compose((n) => n * 2, (n) => n + 1);
console.log(addThenDouble(5));
```

```output for=hof
18
[ 4, 3, 6 ]
[ 'pear', 'banana' ]
10
[ 'fig', 'pear', 'banana' ]
[ 1, 10, 9 ]
[ 1, 9, 10 ]
12
```

- Verdict: native. Functions are objects. Function declarations, function expressions and arrow functions all make values that code can pass and return.
- `map`, `filter`, `forEach` and `some` call the callback with three arguments: the element, its index and the array. `reduce` adds the accumulator in front, for four arguments. A function that has an optional second parameter can get the index by mistake (see Pitfalls).
- `sort` without a comparator converts the elements to strings and compares the strings, so `[10, 9, 1]` sorts as `1, 10, 9`. A comparator returns a negative number, zero or a positive number.
- `sort` changes the array in place. The spread copy `[...words]` keeps the original order of `words`.
