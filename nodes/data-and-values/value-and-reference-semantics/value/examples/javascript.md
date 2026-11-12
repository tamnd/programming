```javascript run id=primitives-and-objects
// A primitive is not an object and has no own properties; typeof tells the kinds apart.
const vals = [42, "hi", true, null, undefined, 10n, Symbol("s"), {}, [], () => 1];
console.log(vals.map((v) => typeof v).join(" "));

// Primitives are immutable: a string method returns a new string.
const s = "value";
const t = s.toUpperCase();
console.log(s, t);

// Primitives compare by value, objects compare by identity.
console.log("ab" === "a" + "b", [1] === [1], { a: 1 } === { a: 1 });

// Object.is differs from === for NaN and -0.
console.log(NaN === NaN, Object.is(NaN, NaN), 0 === -0, Object.is(0, -0));

// An assignment of an object copies the reference, so both names see the change.
const p = { x: 1 };
const q = p;
q.x = 2;
console.log(p.x);

// A function is a value: it can be stored, passed and returned.
const twice = (f) => (v) => f(f(v));
console.log(twice((v) => v + 3)(1));
```

```output for=primitives-and-objects
number string boolean object undefined bigint symbol object object function
value VALUE
true false false
false true true false
2
7
```

- Verdict: native. `typeof` gives the kind of each value, and it says `"object"` for `null`, an object and an array. String methods return new strings, because primitives are immutable. Primitives compare by value, so `"ab" === "a" + "b"`, and objects compare by identity, so `[1] === [1]` is false. `Object.is` treats `NaN` as equal to itself and `0` as different from `-0`. An assignment of an object copies the reference.
