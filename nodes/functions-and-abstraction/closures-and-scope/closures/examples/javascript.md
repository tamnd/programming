```javascript run id=counter
function makeCounter() {
  let count = 0;
  return () => {
    count += 1;
    return count;
  };
}

const first = makeCounter();
const second = makeCounter();
console.log(first(), first(), first());
console.log(second());
```

```output for=counter
1 2 3
1
```

- Verdict: native. Every function in JavaScript is a closure over the bindings of the scope where it was made.
- The closure captures the binding, not a copy, so assignments inside and outside the closure see each other.
- A `let` or `const` loop variable gets a fresh binding for each iteration. A `var` variable has one binding for the whole function (see Pitfalls on the concept page).
- The private fields of a class (`#count`) are the other usual way to hide state.
