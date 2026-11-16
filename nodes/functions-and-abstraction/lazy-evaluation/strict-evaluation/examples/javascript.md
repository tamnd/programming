```javascript run id=strict
function trace(label, value) {
  console.log("evaluating", label);
  return value;
}

function pick(flag, left, right) {
  console.log("inside pick");
  return flag ? left : right;
}

console.log(pick(trace("flag", true), trace("left", 1), trace("right", 2)));
console.log(trace("x", false) && trace("y", true));
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating x
false
```

- Verdict: native. A call evaluates its argument list from left to right and then calls the function. The arguments are values when the body starts.
- `&&`, `||`, `??`, `?.` and `?:` evaluate an operand only when the result needs it.
