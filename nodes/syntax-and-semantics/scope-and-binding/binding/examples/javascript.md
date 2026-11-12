```javascript run id=const-tdz-and-loop-bindings
// const fixes the binding, not the object.
const xs = [1, 2];
xs.push(3);
try {
  xs = [];
} catch (e) {
  console.log(xs, e.name);
}

// A let binding exists from the start of its block, but it cannot be read before
// its declaration runs. This gap is the temporal dead zone.
try {
  console.log(later);
} catch (e) {
  console.log(e.name);
}
let later = 1;

// var makes one binding for the whole function; let in a for loop makes a new one per iteration.
const withVar = [], withLet = [];
for (var i = 0; i < 3; i++) withVar.push(() => i);
for (let j = 0; j < 3; j++) withLet.push(() => j);
console.log(withVar.map((f) => f()), withLet.map((f) => f()));

// this is bound when the function is called, unless bind fixes it.
const user = { name: "ann", hi() { return this?.name; } };
const loose = user.hi;
const fixed = user.hi.bind(user);
console.log(loose(), fixed());
```

```output for=const-tdz-and-loop-bindings
[ 1, 2, 3 ] TypeError
ReferenceError
[ 3, 3, 3 ] [ 0, 1, 2 ]
undefined ann
```

- Verdict: native. `const` fixes the binding, so the program can change the array but cannot assign a new one. A `let` binding cannot be read before its declaration runs. A `var` in a `for` loop is one binding for all the closures, and `let` makes a new binding for each iteration. `this` is bound when the function is called, and `bind` fixes it.
