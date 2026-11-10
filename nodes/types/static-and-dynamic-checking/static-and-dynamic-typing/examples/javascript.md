```javascript run id=types
const values = [3, "3", true, null, undefined, {}, [], () => 3];
console.log(values.map((v) => typeof v).join(" "));
console.log(Array.isArray([]));

console.log("5" + 2, "5" - 2, "5" * "2");
console.log(0 == "", 0 === "");
console.log(null == undefined, null === undefined);
```

```output
number string boolean object undefined object object function
true
52 3 10
true false
true false
```

- Verdict: native. JavaScript is dynamically typed. Each value has one of the language types, and `typeof` names it.
- `typeof null` gives `"object"`, and `typeof []` also gives `"object"`. Use `Array.isArray` for arrays and `=== null` for null.
- Operators convert their operands. `+` concatenates when one operand is a string, and `-` and `*` convert both operands to numbers.
- `==` converts before it compares, and `===` does not.
