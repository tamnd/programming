```javascript run id=falsy-and-truthy
// These are all the falsy values; every other value is truthy.
const falsy = [false, 0, -0, 0n, "", null, undefined, NaN];
console.log(falsy.map(Boolean).join(" "));
console.log([Boolean("0"), Boolean("false"), Boolean([]), Boolean({})].join(" "));

// Boolean() and !! convert a value to a primitive Boolean.
console.log(Boolean("text"), !!"text", !!0);

// A Boolean object is an object, so it is truthy even when it holds false.
const wrapped = new Boolean(false);
console.log(typeof wrapped, wrapped ? "truthy" : "falsy");

// == converts both sides to numbers, so these surprise.
console.log([] == false, "0" == false, null == false);

// || replaces every falsy value; ?? replaces only null and undefined.
const count = 0;
console.log(count || 10, count ?? 10);

// && and || return an operand, not always a Boolean.
console.log("a" && "b", "" || "default", typeof (1 && "x"));
```

```output for=falsy-and-truthy
false false false false false false false false
true true true true
true true false
object truthy
true true false
10 0
b default string
```

- Verdict: native. The example lists all eight falsy values, and `"0"`, `"false"`, `[]` and `{}` are truthy. `Boolean()` and `!!` convert a value to a primitive Boolean. `new Boolean(false)` is an object, so it is truthy. `[] == false` and `"0" == false` are true, but `null == false` is false. `count || 10` replaces `0`, and `count ?? 10` keeps it.
