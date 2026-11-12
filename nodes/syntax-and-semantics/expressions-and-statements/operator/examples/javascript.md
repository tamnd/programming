```javascript run id=conversions-and-nullish-operators
// + adds numbers and joins strings, and it converts the operands first.
console.log(1 + 2, "1" + 2, 1 + "2", [1] + [2]);

// - has no string form, so it converts strings to numbers.
console.log("5" - 2);

// ?? gives the right side only for null or undefined; || also for 0 and "".
console.log(0 ?? 7, 0 || 7);

// ?. stops at null or undefined and gives undefined.
const user = { name: "ada" };
console.log(user.address?.city);

// typeof is an operator with a name, and typeof null is "object".
console.log(typeof 42, typeof null);

// Unary minus before ** is a syntax error, so the grouping must be explicit.
try { eval("-2 ** 2"); } catch (e) { console.log(e.name); }
console.log((-2) ** 2, -(2 ** 2));

// The comma operator evaluates both operands and gives the right one.
let i = (1, 2);
console.log(i);
```

```output for=conversions-and-nullish-operators
3 12 12 12
3
0 7
undefined
number object
SyntaxError
4 -4
2
```

- Verdict: native. `+` joins strings when one operand is a string, and it converts arrays to strings first, while `-` converts strings to numbers. `??` replaces only `null` and `undefined`, and `||` also replaces `0`. `?.` stops at `undefined`. A unary minus before `**` is a syntax error. The comma operator gives its right operand.
