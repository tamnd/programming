```javascript run id=hoisting-of-declarations
// A function declaration is hoisted with its body, so the call before it works.
console.log("area(2) =", area(2));
function area(r) { return 3 * r * r; }

// A var declaration is hoisted, but its value is not, so x is undefined here.
console.log("x before =", x);
var x = 5;
console.log("x after =", x);

// A class declaration is hoisted but stays in the temporal dead zone.
try { new Point(); } catch (e) { console.log(e.name + ":", e.message); }
class Point { constructor() { this.x = 0; } }

// A function expression is a value, so only the var name is hoisted.
try { later(); } catch (e) { console.log(e.name + ":", e.message); }
var later = function () { return 1; };

// A declaration in a block: let is local to the block, var is not.
{ let inner = 1; var outer = 2; }
console.log(typeof inner, typeof outer);
```

```output for=hoisting-of-declarations
area(2) = 12
x before = undefined
x after = 5
ReferenceError: Cannot access 'Point' before initialization
TypeError: later is not a function
undefined number
```

- Verdict: native. A function declaration is hoisted with its body, so the call before it works. A `var` is hoisted without its value, so it reads as `undefined`. A `class` declaration is in the temporal dead zone until it runs. A function expression assigned to a `var` is not hoisted, so the early call fails with `TypeError`. `let` is local to its block, and `var` is not.
