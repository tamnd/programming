```javascript run id=typeof-and-primitives
// typeof gives the type of a value. JavaScript has 7 primitive types and Object.
const values = [
  ["42", 42], ["4.2", 4.2], ["10n", 10n], ['"s"', "s"], ["true", true], ["undefined", undefined],
  ["null", null], ['Symbol("k")', Symbol("k")], ["{}", {}], ["[]", []], ["() => 0", () => 0],
];
for (const [text, v] of values) {
  console.log(text.padStart(11), typeof v);
}

// typeof null is "object", and an array is an object too.
console.log(null === null, Array.isArray([]), Object.prototype.toString.call([]));

// A variable has no type: the same variable holds values of many types.
let v = 1;
v = "one";
v = [1];
console.log(typeof v, v.constructor.name);

// Operators convert values when the types do not match.
console.log(1 + "2", "3" * "4", [] + {}, true + 1);

// A class makes objects with a shared prototype, and instanceof checks it.
class Celsius {
  constructor(degrees) { this.degrees = degrees; }
}
const t = new Celsius(21);
console.log(typeof t, t instanceof Celsius, t.constructor.name, typeof Celsius);
```

```output for=typeof-and-primitives
         42 number
        4.2 number
        10n bigint
        "s" string
       true boolean
  undefined undefined
       null object
Symbol("k") symbol
         {} object
         [] object
    () => 0 function
true true [object Array]
object Array
12 12 [object Object] 2
object true Celsius function
```

- Verdict: native. `typeof` gives one of the primitive types, `"object"` or `"function"`. `typeof null` is `"object"`, so the test for null is `=== null`, and `Array.isArray` tells an array from another object. A variable holds a number, then a string, then an array. Operators convert values of mixed types, so `1 + "2"` is `"12"` and `"3" * "4"` is `12`. `instanceof` checks the class of an object.
