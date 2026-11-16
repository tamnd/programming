```javascript run id=anonymous-function-javascript
const words = ["house", "car", "bike"];
console.log([...words].sort((a, b) => a.length - b.length).join(" "));

console.log(
  [1, 2, 3]
    .map(function (x) {
      return x * x;
    })
    .join(" "),
);

const n = 5;
const add = (x) => x + n;
console.log(add(10));
```

```output for=anonymous-function-javascript
car bike house
1 4 9
15
```

- Verdict: native. The sort uses an arrow function, and the map uses a function expression. The arrow function `add` uses the constant `n`.
