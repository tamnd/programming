```javascript wrong id=merge-proto
function merge(target, source) {
  for (const key of Object.keys(source)) {
    const value = source[key];
    if (value !== null && typeof value === "object") {
      if (typeof target[key] !== "object" || target[key] === null) {
        target[key] = {};
      }
      merge(target[key], value);
    } else {
      target[key] = value;
    }
  }
  return target;
}

const settings = merge({}, JSON.parse('{"theme": "dark"}'));
console.log("theme:", settings.theme);

// The classic demonstration input for prototype pollution.
merge({}, JSON.parse('{"__proto__": {"isAdmin": true}}'));
const user = {};
console.log("an empty object, isAdmin:", user.isAdmin);
```

```output for=merge-proto
theme: dark
an empty object, isAdmin: true
```

`JSON.parse` is safe on its own: it makes `__proto__` an ordinary key of the parsed object. The merge then reads `target["__proto__"]`, which is `Object.prototype`, and writes `isAdmin` into it. Every object in the program now inherits `isAdmin`. CWE-1321 describes the same pattern with the path `__proto__.isAdmin`.

```javascript run id=merge-safe
const FORBIDDEN = new Set(["__proto__", "constructor", "prototype"]);

function merge(target, source) {
  for (const key of Object.keys(source)) {
    if (FORBIDDEN.has(key)) {
      throw new Error(`refused key: ${key}`);
    }
    const value = source[key];
    if (value !== null && typeof value === "object") {
      if (!Object.hasOwn(target, key) || typeof target[key] !== "object" || target[key] === null) {
        target[key] = Object.create(null);
      }
      merge(target[key], value);
    } else {
      target[key] = value;
    }
  }
  return target;
}

const settings = merge(Object.create(null), JSON.parse('{"theme": "dark"}'));
console.log("theme:", settings.theme);

try {
  merge(Object.create(null), JSON.parse('{"__proto__": {"isAdmin": true}}'));
} catch (e) {
  console.log(e.message);
}
const user = {};
console.log("an empty object, isAdmin:", user.isAdmin);
```

```output for=merge-safe
theme: dark
refused key: __proto__
an empty object, isAdmin: undefined
```

The merge refuses the three keys that reach a prototype, as CWE-1321 recommends, and it reads only own properties of the target. Objects from `Object.create(null)` have no prototype at all.

- Verdict: native. `JSON.parse` builds only plain objects, arrays, strings, numbers, booleans and `null`, and it does not call code. Do not use `eval` or the `Function` constructor to parse data.
- Copy the expected fields into a fresh object instead of a deep merge of the whole input.
- Use a `Map` for keys that come from input.
- The `reviver` argument of `JSON.parse` runs for each value. Keep it free of lookups that create objects from names in the data.
