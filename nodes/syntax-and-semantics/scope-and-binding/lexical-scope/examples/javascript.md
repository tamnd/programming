```javascript run id=scopes
const level = "module";

function outer() {
  const level = "outer";
  function inner() {
    return level;
  }
  return inner();
}

function readsModule() {
  return level;
}

function caller() {
  const level = "caller";
  return readsModule();
}

console.log(outer());
console.log(caller());

function blocks() {
  var fromVar = "function";
  let fromLet = "function";
  if (true) {
    var fromVar = "block";
    let fromLet = "block";
  }
  return `var: ${fromVar}, let: ${fromLet}`;
}
console.log(blocks());
```

```output for=scopes
outer
module
var: block, let: function
```

- Verdict: native. `let`, `const` and `class` declarations have block scope. `var` declarations have function scope, so a `var` in a block is the same variable as a `var` of the same name in the function.
- A `var` declaration is hoisted: it is processed before the code of its scope runs. A `let` or `const` variable is in a temporal dead zone from the start of its block until its declaration runs.

```javascript fails expect=exception:ReferenceError id=tdz
let limit = 10;

function check(value) {
  if (value > limit) {
    return "too big";
  }
  let limit = 5;
  return "ok";
}

console.log(check(7));
```

The `let limit` in the function makes `limit` a block-scoped name for the whole function body. The read in the `if` comes before the declaration, so it is in the temporal dead zone and throws a `ReferenceError`. It does not fall back to the outer `limit`.
