```javascript run id=escapes-reserved-words-and-private-names
// $ and _ can start a name, and a Unicode escape can spell a name.
const $ = 1, _x = 2;
const \u0061bc = 3;
console.log($ + _x, abc);

// A reserved word is not a valid variable name, but it is a valid property name.
const obj = { class: "c", if: "i" };
console.log(obj.class, obj.if);
for (const src of ["let class = 1", "var let = 1"]) {
  try {
    new Function(src);
    console.log(JSON.stringify(src), "ok");
  } catch (e) {
    console.log(JSON.stringify(src), e.name);
  }
}

// JavaScript does not normalize names: two spellings of "é" give two names.
const nfc = "caf\u00e9", nfd = "cafe\u0301";
const f = new Function(`const ${nfc} = 1, ${nfd} = 2; return [${nfc}, ${nfd}];`);
console.log(f(), nfc === nfd, nfc === nfd.normalize("NFC"));

// A name that starts with # is a private field of a class.
class Counter {
  #n = 0;
  inc() { return ++this.#n; }
}
const c = new Counter();
c.inc();
console.log(c.inc(), Object.keys(c).length);
```

```output for=escapes-reserved-words-and-private-names
3 3
c i
"let class = 1" SyntaxError
"var let = 1" ok
[ 1, 2 ] false true
2 0
```

- Verdict: native. `$` and `_` can start a name, and the Unicode escape for `a` spells the same name as the letter. A reserved word such as `class` is a valid property name but not a variable name, while `let` is a valid name in non-strict code. JavaScript does not normalize names, so the two spellings of `café` are two variables. A name that starts with `#` is a private field and is not an own key of the object.
