---
tag: TV11TH
kind: feature
owner: javascript
slug: template-literals
name: Template literals
aliases: [template strings, backtick strings, string interpolation in JavaScript, multi-line strings in JavaScript, placeholders, template substitution]
area: languages/javascript/strings-and-text
level: intro
summary: >-
  A template literal is a string in backticks. Each ${expression} placeholder is evaluated, converted to a string and joined with the text around it, and the text may span several lines.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html#sec-template-literals", locator: "13.2.8 Template Literals; 13.2.8.6 Runtime Semantics: Evaluation"}
see_also: [javascript/strings, javascript/tagged-templates, javascript/string-escape-sequences, string-interpolation, interpolate-values-into-a-string, python/formatted-string-literals]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: MDN Web Docs, JavaScript Reference, Template literals
  locator: the introduction; Description; Multi-line strings; String interpolation; Nesting templates
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-expressions.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 13
  locator: 13.2.8.6 Runtime Semantics, Evaluation
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 12
  locator: 12.9.6 Template Literal Lexical Components
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/lexical_analysis.html
  title: The Python Language Reference, Python 3.14, Lexical analysis
  locator: 2.5.7 f-strings
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/tokens/interpolated
  title: C# language reference, String interpolation using $
  locator: the introduction
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const item = "tea";
const price = 3.5;
console.log(`One ${item} costs $${price.toFixed(2)}.`);
console.log(`2 + 3 = ${2 + 3}, upper: ${item.toUpperCase()}`);
const lines = `first
second`;
console.log(lines.split("\n"), lines.length);
console.log(`a backtick: \`, a placeholder: \${x}`);
```

```output for=basics
One tea costs $3.50.
2 + 3 = 5, upper: TEA
[ 'first', 'second' ] 12
a backtick: `, a placeholder: ${x}
```

| Part | Example | Notes |
|---|---|---|
| text | `` `plain` `` | the same escapes as in quoted strings |
| placeholder | `` `${a + b}` `` | any expression; the value is converted to a string |
| line break | a new line in the source | is part of the string |
| escaped backtick | `` `\`` `` | a literal backtick |
| escaped dollar | `` `\${x}` `` | the text `${x}`, no placeholder |

## Semantics

**r1.** Template literals are delimited with backtick characters, "allowing for multi-line strings, string interpolation with embedded expressions, and special constructs called tagged templates."[^1] A template literal can "contain other parts called placeholders, which are embedded expressions delimited by a dollar sign and curly braces: ${expression} ."[^1] Example: [`basics`](#basics).

**r2.** Without a tag, "The default function (when you don't supply your own) just performs string interpolation to do substitution of the placeholders and then concatenate the parts into a single string."[^1] The specification evaluates each placeholder and takes `ToString` of its value: "Let middle be ? ToString(sub)."[^2] With a function name before the backtick, "the result is called a tagged template".[^1] See [[javascript/tagged-templates]].

**r3.** "Template literals coerce their expressions directly to strings, while addition coerces its operands to primitives first."[^1] The specification says the same: "The string conversion semantics applied to the Expression value are like String.prototype.concat rather than the + operator."[^2] So an object with both `valueOf` and `toString` can give different text in a template and with `+`. Example: [`coerce`](#coerce).

**r4.** "Any newline characters inserted in the source are part of the template literal."[^1] Line endings are made the same: in the value of a template, a carriage return and line feed pair, or a carriage return alone, becomes a line feed, and an escape such as `\r` is needed to keep a carriage return.[^3] Example: [`indent`](#indent).

**r5.** "To escape a backtick in a template literal, put a backslash (\ ) before the backtick." "Dollar signs can be escaped as well to prevent interpolation."[^1] An untagged template does not allow a bad escape: "invalid escape sequences will cause a syntax error, unless a tag function is used".[^1] See [[javascript/string-escape-sequences]].

**r6.** Templates nest: "Within a backtick-delimited template, it is simple to allow inner backticks by using them inside an ${expression} placeholder within the template."[^1] Example: [`nested`](#nested).

## Examples

A template inside a placeholder (rule r6).

```javascript run id=nested
const items = ["pen", "ink"];
const list = `<ul>${items.map((i) => `<li>${i}</li>`).join("")}</ul>`;
console.log(list);
const files = (n) => `${n} file${n === 1 ? "" : "s"}`;
console.log(files(1), files(2));
```

```output for=nested
<ul><li>pen</li><li>ink</li></ul>
1 file 2 files
```

A template and `+` convert an object in different ways (rule r3).

```javascript run id=coerce
const money = { valueOf: () => 42, toString: () => "$42" };
console.log(`${money}`, "" + money, String(money));
console.log(`${[1, 2]}`, `${null}`, `${undefined}`, `${{}}`);
```

```output for=coerce
$42 42 $42
1,2 null undefined [object Object]
```

## Pitfalls

### Indentation inside a multi-line template

The spaces at the start of each line are part of the string (rule r4).

```javascript wrong id=indent
function usage() {
  return `Usage:
    run <file>`;
}
console.log(JSON.stringify(usage()));

const fixed = ["Usage:", "  run <file>"].join("\n");
console.log(JSON.stringify(fixed));
```

```output for=indent
"Usage:\n    run <file>"
"Usage:\n  run <file>"
```

### A placeholder in quotes

`${...}` is only a placeholder between backticks. In single or double quotes it is plain text.

```javascript wrong id=quotes
const name = "Ada";
console.log('Hello, ${name}');
console.log(`Hello, ${name}`);
```

```output for=quotes
Hello, ${name}
Hello, Ada
```

## Comparison

Python's f-strings are the closest match: "A formatted string literal or f-string is a string literal that is prefixed with" `f` or `F`, and "They may contain replacement fields delimited by curly braces {} . Replacement fields contain expressions which are evaluated at run time."[^4] In JavaScript the backticks mark the template, with no prefix letter (rule r1). C# uses a `$` before the string: "An interpolated string is a string literal that might contain interpolation expressions."[^5] See [[string-interpolation]], [[interpolate-values-into-a-string]] and [[python/formatted-string-literals]].

## References
