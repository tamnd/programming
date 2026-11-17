---
tag: YEQZ7H
kind: feature
owner: javascript
slug: strings
name: Strings
aliases: [JavaScript string, String type, string primitive, String object, string literal in JavaScript, string coercion]
area: languages/javascript/strings-and-text
level: intro
summary: >-
  A JavaScript string is an immutable sequence of UTF-16 code units. Write it with single quotes, double quotes or backticks. Methods work on the primitive, and String(x) converts a value to a string.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html#sec-ecmascript-language-types-string-type", locator: "6.1.4 The String Type"}
see_also: [javascript/string-escape-sequences, javascript/template-literals, javascript/unicode-and-utf-16-strings, javascript/string-methods, string-concept, python/strings]
sources:
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-data-types-and-values.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 6
  locator: 6.1.4 The String Type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: MDN Web Docs, JavaScript Reference, String
  locator: Description; Creating strings; Character access; Comparing strings; String primitives and String objects; String coercion; UTF-16 characters, Unicode code points, and grapheme clusters
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Numbers_and_strings
  title: MDN Web Docs, JavaScript Guide, Numbers and strings
  locator: Strings; String literals; String object
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/stdtypes.html
  title: The Python Standard Library, Python 3.14, Built-in Types
  locator: Text Sequence Type, str
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: the class description
  accessed: 2026-11-18
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String types
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const name = "Ada";
const greeting = 'Hello, ' + name + "!";
console.log(greeting, greeting.length);
console.log(greeting[0], greeting.at(-1), greeting.slice(7, 10));
console.log(`${name} has ${name.length} letters`);
console.log(typeof greeting, typeof new String("x"));
console.log("😀".length, [..."😀"].length);
```

```output for=basics
Hello, Ada! 11
H ! Ada
Ada has 3 letters
string object
2 1
```

| Form | Example | Notes |
|---|---|---|
| single quotes | `'it\'s'` | the same as double quotes |
| double quotes | `"say \"hi\""` | the same as single quotes |
| template literal | `` `sum: ${a + b}` `` | placeholders, and the text may span lines |
| conversion | `String(value)` | gives a primitive string |
| wrapper object | `new String("x")` | rarely needed |

## Semantics

**r1.** "The String type is generally used to represent textual data in a running ECMAScript program, in which case each element in the String is treated as a UTF-16 code unit value."[^1] "The length of a String is the number of elements (i.e., 16-bit values) within it."[^1] So `length` counts code units, not characters: "Strings are represented fundamentally as sequences of UTF-16 code units."[^2] Example: [`basics`](#basics). See [[javascript/unicode-and-utf-16-strings]].

**r2.** "String literals can be specified using single or double quotes, which are treated identically, or using the backtick character `. This last form specifies a template literal: with this form you can interpolate expressions."[^2] In a quoted literal, "most characters can be entered literally. The only exceptions are the backslash (\ , which starts an escape sequence), the quote character being used to enclose the string, which terminates the string, and the newline character, which is a syntax error if not preceded by a backslash."[^3] See [[javascript/string-escape-sequences]] and [[javascript/template-literals]].

**r3.** "There are two ways to access an individual character in a string. The first is the charAt() method" and "The other way is to treat the string as an array-like object, where individual characters correspond to a numerical index".[^2] The index positions cannot be changed: "When using bracket notation for character access, attempting to delete or assign a value to these properties will not succeed. The properties involved are neither writable nor configurable."[^2] Every string method returns a new string. Example: [`immutable`](#immutable).

**r4.** Literals give primitive strings, and methods still work on them: "In contexts where a method is to be invoked on a primitive string or a property lookup occurs, JavaScript will automatically wrap the string primitive and call the method or perform the property lookup on the wrapper object instead."[^2] `new String(...)` makes an object. MDN warns: "You should rarely find yourself using String as a constructor."[^2]

**r5.** Many operations convert their argument to a string first. In that conversion, `undefined` becomes the text `undefined`, "Numbers are converted with the same algorithm as toString(10) ", "Symbols throw a TypeError ", and objects are first converted with `[Symbol.toPrimitive]()`, `toString()` and `valueOf()`.[^2] A template literal "does exactly the string coercion steps explained above for the embedded expression", while `String(x)` differs in that "Symbols don't throw a TypeError".[^2] The `+` operator "coerces its operand to a primitive instead of a string, and, for some objects, has entirely different behaviors from normal string coercion."[^2] Example: [`coerce`](#coerce).

**r6.** "Note that all comparison operators, including === and == , compare strings case-sensitively."[^2] For text in a language, "A locale-aware and robust solution for testing case-insensitive equality is to use the Intl.Collator API or the string's localeCompare() method".[^2] Example: [`case`](#case).

## Examples

A string does not change in place (rule r3).

```javascript run id=immutable
const word = "cat";
word[0] = "b";
console.log(word, word.toUpperCase(), word);

const fixed = "b" + word.slice(1);
console.log(fixed);
```

```output for=immutable
cat CAT cat
bat
```

The assignment to `word[0]` has no effect, and `toUpperCase` returns a new string.

Three ways to convert a value (rule r5).

```javascript run id=coerce
const sym = Symbol("id");
console.log(String(null), String(42n), String([1, [2, 3]]), String(sym));
try {
  console.log(`${sym}`);
} catch (e) {
  console.log(e.name);
}
const temp = { [Symbol.toPrimitive]: (hint) => (hint === "string" ? "as string" : 7) };
console.log(`${temp}`, "" + temp, String(temp));
```

```output for=coerce
null 42 1,2,3 Symbol(id)
TypeError
as string 7 as string
```

`"" + temp` asks for a primitive with the hint `"default"`, so it gets 7, not the string.

## Pitfalls

### `new String` in a comparison

A wrapper object is an object, so `===` compares identity and an empty one is truthy (rule r4).

```javascript wrong id=wrapper
const a = new String("hi");
const b = new String("hi");
console.log(a === "hi", a == "hi", a === b);
console.log(typeof a, Boolean(new String("")));
console.log(String("hi") === "hi", a.valueOf() === b.valueOf());
```

```output for=wrapper
false true false
object true
true true
```

### Case-insensitive comparison with `toUpperCase`

MDN: "the German lowercase letter ß and ss are both transformed to SS by toUpperCase() , while the Turkish letter ı would be falsely reported as unequal to I by toLowerCase() unless specifically using toLocaleLowerCase("tr") ."[^2] Compare with `localeCompare` and a sensitivity (rule r6).

```javascript wrong id=case
console.log("ß".toUpperCase() === "ss".toUpperCase());
console.log("ı".toLowerCase() === "I".toLowerCase());

const same = (x, y, locale) => x.localeCompare(y, locale, { sensitivity: "accent" }) === 0;
console.log(same("ß", "ss", "de"), same("ı", "I", "tr"));
```

```output for=case
true
false
false true
```

## Comparison

In Python, "Strings are immutable sequences of Unicode code points."[^4] A JavaScript string is a sequence of UTF-16 code units (rule r1), so its `length` can be larger than the number of characters. Java's `String` is close to JavaScript's: "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs", and "Strings are constant; their values cannot be changed after they are created."[^5] Go says "A string value is a (possibly empty) sequence of bytes", and "Strings are immutable: once created, it is impossible to change the contents of a string."[^6] See [[string-concept]] and [[python/strings]].

## References
