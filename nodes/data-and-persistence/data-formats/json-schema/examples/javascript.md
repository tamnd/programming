```javascript run id=max-length
// JSON Schema counts the length of a string in characters (code points), as RFC 8259 does.
// A JavaScript string counts UTF-16 code units, so a character outside the BMP counts as 2.
const schema = JSON.parse('{"type": "string", "maxLength": 3}');

function maxLengthWrong(s, max) {
  return s.length <= max;
}

function maxLength(s, max) {
  return [...s].length <= max;
}

for (const s of JSON.parse('["abc", "h\\u00e9!", "\\ud83d\\ude00\\ud83d\\ude00", "abcd"]')) {
  console.log(JSON.stringify(s), "length", s.length, "code points", [...s].length,
    "wrong", maxLengthWrong(s, schema.maxLength), "right", maxLength(s, schema.maxLength));
}
```

```output for=max-length
"abc" length 3 code points 3 wrong true right true
"hé!" length 3 code points 3 wrong true right true
"😀😀" length 4 code points 2 wrong false right true
"abcd" length 4 code points 4 wrong false right false
```

- Verdict: library. JavaScript has no JSON Schema validator in the language, so applications use a library such as Ajv. The example shows one rule that a hand-written check often gets wrong. The validation specification defines the length of a string as "the number of its characters as defined by RFC 8259", and a JSON character is a code point. The property `length` of a JavaScript string counts UTF-16 code units, so the string of two emoji has the length 4. The spread `[...s]` splits a string into code points, and the count 2 is the length that the schema uses.
