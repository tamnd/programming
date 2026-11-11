```javascript run id=in-place
function reverseInPlace(xs) {
  for (let i = 0, j = xs.length - 1; i < j; i++, j--) {
    [xs[i], xs[j]] = [xs[j], xs[i]];
  }
}

const data = [1, 2, 3, 4, 5];
reverseInPlace(data);
console.log(data);

let count = 0;
for (const ch of "imperative") {
  if ("aeiou".includes(ch)) count += 1;
}
console.log(count);

const fixed = 1;
try {
  fixed = 2;
} catch (e) {
  console.log(e.name);
}
```

```output for=in-place
[ 5, 4, 3, 2, 1 ]
5
TypeError
```

- Verdict: native. JavaScript has `let` for variables that change and `const` for bindings that do not. A `const` binding can still refer to an object or array that changes, as `data` does here.
- Loops include `for`, `for...of`, `while` and `do...while`, and destructuring assignment swaps two values in one statement.
