```javascript run id=utf16-string-primitive
"use strict";
// A string is a primitive value made of UTF-16 code units.
const s = "naïve 😀";
console.log(typeof s, s.length, [...s].length);

// Strings are immutable. In strict mode an assignment to an index throws.
try {
  s[0] = "N";
} catch (e) {
  console.log(e.constructor.name);
}

// Methods return new strings and leave the old one as it was.
const upper = s.toUpperCase();
console.log(s, "|", upper);

// A String object is not a primitive string.
const boxed = new String("hi");
console.log(typeof boxed, boxed == "hi", boxed === "hi");

// TextEncoder gives the UTF-8 bytes of a string.
const bytes = new TextEncoder().encode(s);
console.log(bytes.constructor.name, bytes.length);
console.log(new TextDecoder().decode(bytes) === s);
```

```output for=utf16-string-primitive
string 8 7
TypeError
naïve 😀 | NAÏVE 😀
object true false
Uint8Array 11
true
```

- Verdict: native. `length` counts UTF-16 code units, so the string has a length of 8 but only 7 code points. In strict mode an assignment to an index throws `TypeError`. `toUpperCase` returns a new string. `new String("hi")` is an object: `==` converts it, and `===` sees a different type. `TextEncoder` gives 11 UTF-8 bytes, and `TextDecoder` gives the same text back.
