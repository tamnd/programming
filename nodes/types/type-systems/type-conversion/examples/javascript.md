```javascript run id=coercion
console.log(1 + "2", "3" - 1, "3" * "4");
console.log(Number(""), Number(" 42 "), Number("42abc"));
console.log(parseInt("42abc", 10), parseFloat("2.5kg"));
console.log([] + [], [1, 2] + [3]);
console.log(0 == "", "0" == false, null == 0, null == undefined);
console.log(0 === "", "0" === false, NaN == NaN);
console.log(Boolean("0"), Boolean(""), Boolean([]));
console.log(String(123) + 1, Number(true) + 1);
```

```output for=coercion
12 2 12
0 42 NaN
42 2.5
 1,23
true true false true
false false false
true false true
1231 2
```

Mixing a BigInt and a Number in arithmetic is the one case where JavaScript refuses to convert.

```javascript fails expect=exception:TypeError id=bigint-mix
const big = 2n ** 64n;
console.log(big + 1);
```

The fix is an explicit conversion: `big + 1n`, or `Number(big) + 1` if the loss of precision is acceptable.

- Verdict: native. Almost every operator converts its operands. `+` concatenates if either operand is a string after `ToPrimitive`, and the other arithmetic operators convert to numbers.
- `Number(text)` rejects the whole string if any part is not numeric, but treats an empty string as 0. `parseInt` reads a prefix and stops at the first character that is not a digit.
- An array converts to a string of its elements joined by commas, so `[1, 2] + [3]` is `"1,23"` and `[] + []` is the empty string.
- `==` converts by the IsLooselyEqual algorithm, and `===` never converts. `NaN` is not equal to itself under either operator.
