---
tag: NXYQT2
kind: feature
owner: javascript
slug: tagged-templates
name: Tagged templates
aliases: [tag functions, template tags, String.raw, raw strings in JavaScript, strings.raw, cooked and raw template strings]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  A tagged template calls a function with the text parts of a template literal and the values of its placeholders. The function can return anything. String.raw is the built-in tag.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-tagged-templates", locator: "13.3.11 Tagged Templates; 13.2.8.4 GetTemplateObject"}
see_also: [javascript/template-literals, javascript/string-escape-sequences, javascript/strings, python/template-string-literals]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: MDN Web Docs, JavaScript Reference, Template literals
  locator: Tagged templates; Raw strings; Tagged templates and escape sequences
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.2.8.4 GetTemplateObject ( templateLiteral )
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/raw
  title: MDN Web Docs, JavaScript Reference, String.raw()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/string.templatelib.html
  title: The Python Standard Library, Python 3.14, string.templatelib
  locator: Template strings
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
function show(strings, ...values) {
  return JSON.stringify({ strings: [...strings], values });
}
const a = 2;
const b = 3;
console.log(show`sum ${a} + ${b} = ${a + b}!`);
console.log(show`${a}`);
console.log(String.raw`C:\new\x${a}`);
```

```output for=basics
{"strings":["sum "," + "," = ","!"],"values":[2,3,5]}
{"strings":["",""],"values":[2]}
C:\new\x2
```

| Part | What the tag gets |
|---|---|
| `strings` | a frozen array of the text parts, one more than the placeholders |
| `strings.raw` | the same parts as written, with escapes not processed |
| `...values` | the placeholder values, not converted to strings |
| result | whatever the tag returns |

## Semantics

**r1.** "Tags allow you to parse template literals with a function. The first argument of a tag function contains an array of string values. The remaining arguments are related to the expressions."[^1] The tag can "return the manipulated string. (Alternatively, it can return something completely different, as described in one of the following examples.)"[^1] Example: [`basics`](#basics).

**r2.** The array of strings always has one more element than there are placeholders: "its length is equal to the number of substitutions (occurrences of ${…} ) plus one, and is therefore always non-empty."[^1] So a template that starts or ends with a placeholder has an empty string at that end.

**r3.** "For any particular tagged template literal expression, the tag function will always be called with the exact same literal array, no matter how many times the literal is evaluated."[^1] "This allows the tag to cache the result based on the identity of its first argument. To further ensure the array value's stability, the first argument and its raw property are both frozen, so you can't mutate them in any way."[^1] The specification builds the array once per site in GetTemplateObject and ends with "Perform ! SetIntegrityLevel(template, frozen)."[^2] Example: [`same-array`](#same-array).

**r4.** "The special raw property, available on the first argument to the tag function, allows you to access the raw strings as they were entered, without processing escape sequences."[^1] `String.raw` uses it: it "is the only built-in template literal tag. It has close semantics to an untagged literal since it concatenates all arguments and returns a string."[^3]

**r5.** A tagged template may hold escapes that are errors elsewhere: "the syntax restriction of well-formed escape sequences is removed from tagged templates."[^1] "However, illegal escape sequences must still be represented in the "cooked" representation. They will show up as an undefined element in the "cooked" array".[^1] Example: [`cooked`](#cooked). See [[javascript/string-escape-sequences]].

## Examples

A tag that escapes HTML in each value and leaves the text parts alone (rule r1).

```javascript run id=html
const codes = { "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" };
const escape = (s) => s.replace(/[&<>"]/g, (c) => codes[c]);
function html(strings, ...values) {
  return strings.reduce((out, str, i) => out + escape(String(values[i - 1])) + str);
}
const user = '<b>"Bob"</b>';
console.log(html`<p>Hello, ${user}</p>`);
```

```output for=html
<p>Hello, &lt;b&gt;&quot;Bob&quot;&lt;/b&gt;</p>
```

The same array on every evaluation of one site (rule r3).

```javascript run id=same-array
const seen = [];
const tag = (strings) => seen.push(strings);
for (let i = 0; i < 2; i++) tag`x${i}y`;
tag`x${0}y`;
console.log(seen[0] === seen[1], seen[0] === seen[2]);
console.log(Object.isFrozen(seen[0]), Object.isFrozen(seen[0].raw));
```

```output for=same-array
true false
true true
```

The two calls in the loop share one array; the third call is a different site with its own.

Cooked and raw text for a bad escape (rule r5).

```javascript run id=cooked
const parts = (s) => [s[0], s.raw[0]];
console.log(parts`\unicode`, parts`a\tb`[0].length, parts`a\tb`[1].length);
```

```output for=cooked
[ undefined, '\\unicode' ] 3 4
```

## Pitfalls

### Joining only the strings

A tag that uses `strings.join` drops the values. `String.raw({ raw: strings }, ...values)` puts them back between the parts, a trick MDN shows for a tag that passes the cooked strings to `String.raw`.[^1]

```javascript wrong id=join
const upper = (strings, ...values) => strings.join("").toUpperCase();
console.log(upper`hi ${"bob"}!`);

const shout = (strings, ...values) =>
  String.raw({ raw: strings }, ...values.map((v) => String(v).toUpperCase()));
console.log(shout`hi ${"bob"}!`);
```

```output for=join
HI !
hi BOB!
```

### A template on the next line without a semicolon

A template after an expression is a tagged template, even across a line break, so the line above is called as a tag. MDN: these expressions "would not be subject to automatic semicolon insertion".[^1]

```javascript wrong id=asi
try {
  const label = "total"
  `${1 + 2}`;
  console.log(label);
} catch (e) {
  console.log(e.name);
}
const label = "total";
console.log(label, `${1 + 2}`);
```

```output for=asi
TypeError
total 3
```

## Comparison

Python 3.14 has t-strings for the same job: "Template strings are a mechanism for custom string processing. They have the full flexibility of Python’s f-strings, but return a Template instance that gives access to the static and interpolated (in curly brackets) parts of a string before they are combined."[^4] A Python t-string evaluates to a `Template` object, and the code that processes it decides what to do with the parts. A JavaScript tag is called at once with the parts (rule r1). For raw text, MDN compares `String.raw` with other languages: "This is similar to the r prefix in Python, or the @ prefix in C# for string literals."[^3] See [[javascript/template-literals]] and [[python/template-string-literals]].

## References
