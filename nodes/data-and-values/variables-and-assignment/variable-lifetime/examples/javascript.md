```javascript run id=block-and-closure
function makeCounter() {
  let count = 0;
  return () => ++count;
}

const a = makeCounter();
const b = makeCounter();
console.log(a(), a(), b());

{
  let inner = "block";
  console.log(inner);
}
console.log(typeof inner);
```

```output for=block-and-closure
1 2 1
block
undefined
```

- Verdict: native. A `let` variable lives for its block, and a `var` variable for its function. A closure keeps the variables that it uses alive, so each counter keeps its own `count` after `makeCounter` returns. Outside the block, the name `inner` does not exist, and `typeof` of an unknown name gives `"undefined"`.
