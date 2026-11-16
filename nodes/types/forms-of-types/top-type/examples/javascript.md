```javascript run id=object-root
const values = [42, "hi", null, undefined, [1], { a: 1 }, () => 1];
console.log(values.map((v) => typeof v).join(" "));
console.log(values.map((v) => v instanceof Object).join(" "));

// Object(v) wraps a primitive in an object, and it makes an empty object for null and undefined.
console.log(values.map((v) => Object(v) instanceof Object).join(" "));

// Most objects inherit from Object.prototype, so they share its methods.
console.log(Object.getPrototypeOf([]) === Array.prototype, Object.getPrototypeOf(Array.prototype) === Object.prototype);
```

```output for=object-root
number string object undefined object object function
false false false false true true true
true true true true true true true
true true
```

- Verdict: idiom. JavaScript has no static types, so any variable can hold any value. Arrays, plain objects and functions inherit from `Object.prototype`, but numbers, strings, `null` and `undefined` are primitives, so `instanceof Object` is false for them. `Object(v)` turns each value into an object.
