```javascript run id=block-scope-and-labeled-blocks
// let and const are scoped to the block; var is scoped to the function.
{
  let a = 1;
  var b = 2;
}
console.log(typeof a, b);

// A labeled block can be left early with break.
outer: {
  console.log("in the block");
  if (b === 2) break outer;
  console.log("not printed");
}

// Each iteration of a for loop with let has its own binding.
const fs = [];
for (let i = 0; i < 3; i++) { fs.push(() => i); }
console.log(fs.map((f) => f()));

// After =>, a { starts a block, so value: is a label and the function returns undefined.
const r = (() => { value: 1 })();
console.log(r);

```

```output for=block-scope-and-labeled-blocks
undefined 2
in the block
[ 0, 1, 2 ]
undefined
```

- Verdict: native. `let` is scoped to the block and `var` to the function, so `a` is undefined after the block and `b` is `2`. A labeled block can be left with `break`. A `for` loop with `let` makes a new binding for each iteration. After `=>`, a `{` starts a block, so `value:` is a label and the function returns `undefined`.
