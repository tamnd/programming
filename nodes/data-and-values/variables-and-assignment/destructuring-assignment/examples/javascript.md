```javascript run id=array-and-object
const [a, , c = 30, ...rest] = [10, 20, undefined, 40, 50];
console.log(a, c, rest);

const { name, age: years = 0, ...other } = { name: "Ada", city: "London", role: "dev" };
console.log(name, years, other);

let x = 1;
let y = 2;
[x, y] = [y, x];
console.log(x, y);
```

```output for=array-and-object
10 30 [ 40, 50 ]
Ada 0 { city: 'London', role: 'dev' }
2 1
```

- Verdict: native. An array pattern takes items by position, and an empty place skips one. An object pattern takes properties by name, and `age: years` reads `age` into the name `years`. A default replaces `undefined`, and a rest element collects the other items or properties.
