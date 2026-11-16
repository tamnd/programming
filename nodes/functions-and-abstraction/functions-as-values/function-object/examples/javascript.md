```javascript run id=function-object-javascript
function makeMultiplier(factor) {
  function multiply(x) {
    multiply.calls += 1;
    return x * factor;
  }
  multiply.calls = 0;
  return multiply;
}

const triple = makeMultiplier(3);
console.log(triple(5));
console.log([1, 2, 3].map((x) => triple(x)).join(" "));
console.log(triple.calls);
```

```output for=function-object-javascript
15
3 6 9
4
```

- Verdict: native. A JavaScript function is an object, so it can have properties. The function `multiply` keeps the count in its own property `calls`.
