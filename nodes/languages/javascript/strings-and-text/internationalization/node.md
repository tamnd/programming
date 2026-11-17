---
tag: 17RTFP
kind: feature
owner: javascript
slug: internationalization
name: Internationalization
aliases: [Intl, the Intl object, ECMA-402, ECMAScript Internationalization API, i18n in JavaScript, locales and options arguments, toLocaleString]
area: languages/javascript/strings-and-text
level: core
summary: >-
  Intl holds the ECMA-402 constructors for language-sensitive work. Each takes locales and options, picks the best supported locale, and gives an object whose method formats, compares or segments.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#intl-object", locator: "4.2 API Overview; 8 The Intl Object; 9 Locale and Parameter Negotiation"}
see_also: [javascript/formatting-data, javascript/string-normalization-and-comparison, javascript/string-methods, locale-aware-formatting, format-a-number-with-separators]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: the introduction; Overview
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: Introduction; 1 Scope; 4.2 API Overview; 8 The Intl Object
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl
  title: MDN Web Docs, JavaScript Reference, Intl
  locator: the introduction; Description; locales argument
  accessed: 2026-11-18
- url: https://nodejs.org/docs/latest-v24.x/api/intl.html
  title: Node.js v24 Documentation, Internationalization support
  locator: Internationalization support; Detecting internationalization support
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/locale.html
  title: The Python Standard Library, Python 3.14, locale
  locator: the introduction; locale.setlocale
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/NumberFormat.html
  title: Java SE 25 API, Class NumberFormat
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const n = 1234567.891;
console.log(new Intl.NumberFormat("en-US").format(n));
console.log(new Intl.NumberFormat("de-DE").format(n));
console.log(n.toLocaleString("en-IN"));
const price = new Intl.NumberFormat("en-US", { style: "currency", currency: "USD" });
console.log(price.format(5.259));
console.log(typeof Intl, typeof process.versions.icu);
```

```output for=basics
1,234,567.891
1.234.567,891
12,34,567.891
$5.26
object string
```

| Constructor | Job |
|---|---|
| `Intl.Locale` | a locale and its parts |
| `Intl.NumberFormat`, `Intl.DateTimeFormat`, `Intl.ListFormat`, `Intl.RelativeTimeFormat`, `Intl.DurationFormat` | formatting |
| `Intl.Collator` | comparing strings for sorting or searching |
| `Intl.PluralRules` | choosing a plural form |
| `Intl.Segmenter` | splitting text into graphemes, words or sentences |
| `Intl.DisplayNames` | names of languages, regions, scripts and currencies |

## Semantics

**r1.** "The Intl object is the namespace for the ECMAScript Internationalization API, which provides a wide range of locale- and culture-sensitive data and operations."[^1] The API is defined in its own standard, ECMA-402: "This Standard defines the application programming interface for ECMAScript objects that support programs that need to adapt to the linguistic and cultural conventions used by different human languages and countries."[^2]

**r2.** `Intl` is a plain object: "Unlike most global objects, Intl is not a constructor. You cannot use it with the new operator or invoke the Intl object as a function. All properties and methods of Intl are static (just like the Math object)."[^3] Example: [`errors`](#errors).

**r3.** "Most Intl APIs share a similar design (Intl.Locale is the only exception). You start by constructing an instance with the desired locale and options. This defines a set of rules for the desired operation (formatting, collation, segmentation, etc.). Then, when you call the method on the instance, such as format() , compare() , or segment() , the object applies the specified rule to the passed data."[^1] The object also "provides a resolvedOptions function, which the application can use to find out the exact configuration of the object."[^2] Example: [`basics`](#basics).

**r4.** The constructors "accept locales and options arguments, and negotiate the requested locale(s) against the locales they support".[^3] With `locales` left out, "The implementation's default locale will be used."[^3] "If a locale identifier is a string that's syntactically invalid, a RangeError is thrown. If a locale identifier is well-formed but the implementation doesn't recognize it, it is ignored and the next locale in the list is considered, eventually falling back to the system's locale."[^3] Example: [`negotiate`](#negotiate).

**r5.** The locale methods of ECMA-262, such as `toLocaleString` and `localeCompare`, are the indirect way in: they "are respecified in this specification to accept the same arguments as the Collator, NumberFormat, and DateTimeFormat constructors and produce the same results as their compare or format methods."[^2] See [[javascript/string-normalization-and-comparison]].

**r6.** The API formats but does not parse: "While the API includes a variety of formatters, it does not provide any parsing facilities."[^2] Example: [`parse`](#parse).

**r7.** In Node.js the work is done by ICU: "Node.js and the underlying V8 engine use International Components for Unicode (ICU) to implement these features in native C/C++ code. The full ICU data set is provided by Node.js by default."[^4] A build can leave it out, and "checking for process.versions.icu , a property defined only when ICU is enabled, works too".[^4]

## Examples

Choosing a locale from a list, and the result of the choice (rule r4).

```javascript run id=negotiate
const fmt = new Intl.DateTimeFormat(["xx-YY", "fr-CA", "en"], {
  timeZone: "UTC",
  dateStyle: "long",
});
console.log(fmt.resolvedOptions().locale);
console.log(fmt.format(new Date(Date.UTC(2026, 10, 18))));
console.log(Intl.getCanonicalLocales("EN-us"));
```

```output for=negotiate
fr-CA
18 novembre 2026
[ 'en-US' ]
```

`xx-YY` is well formed but unknown, so it is skipped and `fr-CA` is used.

Calling `Intl` and passing a malformed tag (rules r2 and r4).

```javascript run id=errors
try {
  new Intl();
} catch (e) {
  console.log(e.name);
}
try {
  new Intl.NumberFormat("en_US");
} catch (e) {
  console.log(e.name);
}
```

```output for=errors
TypeError
RangeError
```

A language tag uses `-`, not `_`.

## Pitfalls

### Reading a formatted number back with `Number`

```javascript wrong id=parse
const shown = new Intl.NumberFormat("de-DE").format(1234.5);
console.log(shown, Number(shown), parseFloat(shown));
```

```output for=parse
1.234,5 NaN 1.234
```

Formatted text is for people. Keep the number, and format it only for display (rule r6).

## Comparison

Python's `locale` module "opens access to the POSIX locale database and functionality."[^5] Its locale is set for the whole process with `setlocale`, and "setlocale() is not thread-safe on most systems."[^5] Java has classes such as `NumberFormat`, which "provides the interface for formatting and parsing numbers in a localized manner."[^6] In JavaScript each `Intl` object carries its own locale (rule r3), so two formatters for two languages can be used side by side, and there is no parsing (rule r6). See [[locale-aware-formatting]] and [[javascript/formatting-data]].

## References
