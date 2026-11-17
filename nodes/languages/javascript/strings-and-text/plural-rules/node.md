---
tag: 7C7DXB
kind: feature
owner: javascript
slug: plural-rules
name: Plural rules
aliases: [Intl.PluralRules, plural categories, cardinal and ordinal plurals, ordinal suffixes in JavaScript, selectRange, pluralization]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  Intl.PluralRules maps a number to the plural category a locale uses for it, one of zero, one, two, few, many and other, for counts or for ordinals. The program picks the words for each category.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#pluralrules-objects", locator: "17 PluralRules Objects; 17.1.1 Intl.PluralRules; 17.5.1 PluralRuleSelect"}
see_also: [javascript/formatting-data, javascript/internationalization, javascript/template-literals, locale-aware-formatting]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Plural rules
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/PluralRules/select
  title: MDN Web Docs, JavaScript Reference, Intl.PluralRules.prototype.select()
  locator: Return value; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/PluralRules
  title: MDN Web Docs, JavaScript Reference, Intl.PluralRules
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: 17.1.1 Intl.PluralRules; 17.3.2 Intl.PluralRules.prototype.resolvedOptions; 17.5.1 PluralRuleSelect
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/PluralRules/PluralRules
  title: MDN Web Docs, JavaScript Reference, Intl.PluralRules() constructor
  locator: Parameters; type
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/PluralRules/selectRange
  title: MDN Web Docs, JavaScript Reference, Intl.PluralRules.prototype.selectRange()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/gettext.html
  title: The Python Standard Library, Python 3.14, gettext
  locator: gettext.ngettext
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/ChoiceFormat.html
  title: Java SE 25 API, Class ChoiceFormat
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const en = new Intl.PluralRules("en-US");
console.log([0, 1, 2, 1.5].map((n) => en.select(n)));
const ar = new Intl.PluralRules("ar-EG");
console.log([0, 1, 2, 6, 18, 100].map((n) => ar.select(n)));
console.log(new Intl.PluralRules("zh").select(1), en.resolvedOptions().pluralCategories);
```

```output for=basics
[ 'other', 'one', 'other', 'other' ]
[ 'zero', 'one', 'two', 'few', 'many', 'other' ]
other [ 'one', 'other' ]
```

| Call | Result |
|---|---|
| `new Intl.PluralRules(locale)` | rules for counts (`type: "cardinal"`) |
| `new Intl.PluralRules(locale, { type: "ordinal" })` | rules for 1st, 2nd, 3rd |
| `rules.select(n)` | one of `zero`, `one`, `two`, `few`, `many`, `other` |
| `rules.selectRange(a, b)` | the category for a range |
| `rules.resolvedOptions().pluralCategories` | the categories the locale uses |

## Semantics

**r1.** "The Intl.PluralRules object is useful for selecting the correct plural form of a word. It doesn't automatically pluralize words for you (for example, you can't pass it "apple" and expect "apples" back), but it tells you which plural form to use based on a number."[^1] `select(number)` returns "A string representing the pluralization category of the number".[^2] The program keeps one word or message per category and looks it up with the result. Example: [`basics`](#basics).

**r2.** "The full set of tags that might be returned are: zero , one , two , few , many , and other (the "general" plural form, also used if the language only has one form)."[^3] Languages differ: "English has two forms for expressing cardinal numbers", "while Chinese has only one form, and Arabic has six!"[^3] `resolvedOptions().pluralCategories` lists "all possible results of PluralRuleSelect for the selected locale".[^4]

**r3.** The `type` option is `cardinal`, the default, "For cardinal numbers (referring to the quantity of things)", or `ordinal`, "For ordinal number (referring to the ordering or ranking of things".[^5] For English ordinals, `select()` "can return any of four tags", one for each of the endings st, nd, rd and th.[^3] Example: [`ordinal`](#ordinal).

**r4.** The category depends on how the number is written, not only on its value: "The plural form of the specified number may also depend on constructor options , such as how the number is rounded".[^3] The constructor takes the digit options of `Intl.NumberFormat`, such as `minimumFractionDigits`.[^5] In ECMA-402 the operation that picks the category takes the number as "s (a decimal String)".[^4] Example: [`digits`](#digits).

**r5.** `selectRange()` "receives two values and returns a string indicating which plural rule to use for locale-aware formatting of the indicated range."[^6] "In English there is only one plural form, such as "1–10 apples", and the method will return other . Other languages can have many forms."[^6] Example: [`range`](#range).

**r6.** `Intl.PluralRules` must be called with `new`. Its constructor begins: "If NewTarget is undefined, throw a TypeError exception."[^4] Example: [`nonew`](#nonew).

## Examples

Ordinal suffixes in English (rule r3).

```javascript run id=ordinal
const pr = new Intl.PluralRules("en-US", { type: "ordinal" });
const suffixes = new Map([["one", "st"], ["two", "nd"], ["few", "rd"], ["other", "th"]]);
const ordinal = (n) => `${n}${suffixes.get(pr.select(n))}`;
console.log([0, 1, 2, 3, 4, 11, 12, 13, 21, 42, 103].map(ordinal).join(" "));
```

```output for=ordinal
0th 1st 2nd 3rd 4th 11th 12th 13th 21st 42nd 103rd
```

The same number shown as `1` and as `1.0` (rule r4).

```javascript run id=digits
const plain = new Intl.PluralRules("en");
const oneDecimal = { minimumFractionDigits: 1 };
const fixed = new Intl.PluralRules("en", oneDecimal);
console.log(plain.select(1), fixed.select(1));
console.log(new Intl.NumberFormat("en", oneDecimal).format(1));
```

```output for=digits
one other
1.0
```

With one fraction digit the number is shown as `1.0`, and the English rules put that in `other`. Give the rules the same digit options as the formatter that shows the number.

Ranges (rule r5).

```javascript run id=range
console.log(new Intl.PluralRules("en").selectRange(1, 10));
console.log(new Intl.PluralRules("sl").selectRange(102, 201));
console.log(new Intl.PluralRules("pt").selectRange(102, 102));
```

```output for=range
other
few
other
```

## Pitfalls

### Choosing the form with `n === 1`

```javascript wrong id=ternary
const byHand = (n) => (n === 1 ? "one" : "other");
const pl = new Intl.PluralRules("pl");
const counts = [1, 2, 5, 22, 25];
console.log(counts.map(byHand));
console.log(counts.map((n) => pl.select(n)));
```

```output for=ternary
[ 'one', 'other', 'other', 'other', 'other' ]
[ 'one', 'few', 'many', 'few', 'many' ]
```

The test works for English and fails for languages with more forms. MDN: the hand-written test "is hard to generalize across languages, especially those with many plural forms."[^1] Ask the rules for the category (rule r2).

### Calling `Intl.PluralRules` without `new`

```javascript wrong id=nonew
console.log(typeof Intl.Collator("en").compare);
try {
  Intl.PluralRules("en");
} catch (e) {
  console.log(e.name);
}
```

```output for=nonew
function
TypeError
```

The `Intl.Collator` constructor goes on without `new`: "If NewTarget is undefined, let newTarget be the active function object".[^4] `Intl.PluralRules` throws (rule r6).

## Comparison

Python's `gettext.ngettext` takes a singular, a plural and a number, and when a translation is found it applies "the plural formula to n"; "If no translation is found, return singular if n is 1; return plural otherwise."[^7] Java's `ChoiceFormat` attaches "a format to a range of numbers" and "is generally used in a MessageFormat for handling plurals", but "ChoiceFormat doesn't implement any locale specific behavior."[^8] JavaScript gives the category from the locale data (rule r2) and leaves the words to the program (rule r1). See [[javascript/formatting-data]].

## References
