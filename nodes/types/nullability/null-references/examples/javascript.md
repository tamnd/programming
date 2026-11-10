```javascript run id=absent
const user = { name: "Ada", address: null };

console.log(user.address?.city);
console.log(user.address?.city ?? "unknown");
console.log(user.phone);
console.log(user.phone ?? "no phone");

console.log(null == undefined, null === undefined);
console.log(typeof null, typeof undefined);

const count = 0;
console.log(count || 10, count ?? 10);
```

```output
undefined
unknown
undefined
no phone
true false
object undefined
10 0
```

- Verdict: native. The language has two values for absence. `null` is the intentional absence of an object value. `undefined` is the value of a variable or a property that has no value.
- Reading a property of `null` or `undefined` throws a `TypeError` (see Pitfalls on the concept page). The optional chaining operator `?.` returns `undefined` instead when its base is `null` or `undefined`.
- The `??` operator returns its right operand only when its left operand is `null` or `undefined`. The `||` operator also replaces `0`, `""` and `false`, as the last line shows.
- `null == undefined` is `true`, so `x == null` tests for both values. `typeof null` is `"object"`.
