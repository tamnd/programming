```javascript run id=curry
const add = (a) => (b) => a + b;
const addTen = add(10);
console.log(addTen(5), add(1)(2));

function greet(greeting, name) {
  return `${greeting}, ${name}`;
}

const hello = greet.bind(null, "hello");
console.log(hello("ann"));

const curry = (f) =>
  function next(...args) {
    return args.length >= f.length ? f(...args) : (...more) => next(...args, ...more);
  };

const volume = curry((l, w, h) => l * w * h);
console.log(volume(2)(3)(4), volume(2, 3)(4), volume(2, 3, 4));
```

```output for=curry
15 3
hello, ann
24 24 24
```

- Verdict: native. Arrow functions make curried functions short. `Function.prototype.bind` fixes the `this` value and any number of leading arguments.
- A generic `curry` helper uses the `length` property of the function, the number of declared parameters. A rest parameter or a parameter with a default value is not counted, so such a helper fails for those functions.

```javascript wrong id=too-many
const add = (a) => (b) => a + b;
console.log(typeof add(1, 2));
```

```output for=too-many
function
```

`add` takes one argument and ignores the second. The call returns the inner function, not a number. A curried function must be called one argument at a time: `add(1)(2)`.
