```javascript run id=let-and-const
let count = 1;
count += 1;
let value = 5;
value = "five";
console.log(count, typeof value, value);

const items = [1, 2];
items.push(3);
console.log(items);
try {
  items = [];
} catch (e) {
  console.log(e.name);
}
```

```output for=let-and-const
2 string five
[ 1, 2, 3 ]
TypeError
```

- Verdict: native. `let` declares a variable in a block, and the variable can hold a value of any type. `const` declares a name that cannot get a new value, but the array that it refers to can still change. The assignment `items = []` throws a `TypeError` when it runs.
