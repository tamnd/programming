```javascript run id=surrogate-pairs-and-lone-surrogates
// A JavaScript string is a sequence of UTF-16 code units.
for (const text of ["hello", "café", "日本", "😀"]) {
  const utf8 = new TextEncoder().encode(text).length;
  console.log(`${JSON.stringify(text)}: code units=${text.length} utf-8 bytes=${utf8}`);
}

// The emoji is stored as a surrogate pair: two code units.
const face = "😀";
console.log(face.charCodeAt(0).toString(16), face.charCodeAt(1).toString(16));

// A string can hold a lone surrogate, which is not valid UTF-16 text.
const lone = face.slice(0, 1);
console.log(lone.length, lone.isWellFormed(), face.isWellFormed());
try {
  encodeURIComponent(lone);
} catch (e) {
  console.log(e.name);
}
console.log(lone.toWellFormed().codePointAt(0).toString(16));
```

```output for=surrogate-pairs-and-lone-surrogates
"hello": code units=5 utf-8 bytes=5
"café": code units=4 utf-8 bytes=5
"日本": code units=2 utf-8 bytes=6
"😀": code units=2 utf-8 bytes=4
d83d de00
1 false true
URIError
fffd
```

- Verdict: native. `length` counts UTF-16 code units, so `日本` is 2 units but 6 UTF-8 bytes, and the emoji is 2 units. `charCodeAt` shows the surrogate pair `d83d de00`. A slice can cut the pair in half. The result is a string, but `isWellFormed` is false and `encodeURIComponent` throws `URIError`. `toWellFormed` replaces the lone surrogate with U+FFFD.
