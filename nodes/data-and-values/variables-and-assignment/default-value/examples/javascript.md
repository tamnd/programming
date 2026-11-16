```javascript run id=undefined-and-defaults
let x;
const obj = {};
console.log(x, obj.missing, [1, 2][5]);

function area(w, h = w) {
  return w * h;
}
console.log(area(3), area(3, 4));

const count = 0;
console.log(count || 10, count ?? 10);
```

```output for=undefined-and-defaults
undefined undefined undefined
9 12
10 0
```

- Verdict: native. `undefined` is the default value of a variable, a missing property and an element past the end of an array. A default parameter can use an earlier parameter. `||` replaces any falsy value, so it replaces a count of 0, but `??` replaces only `null` and `undefined`.
