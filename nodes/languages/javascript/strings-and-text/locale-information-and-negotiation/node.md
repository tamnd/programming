---
tag: 0K2HVY
kind: feature
owner: javascript
slug: locale-information-and-negotiation
name: Locale information and negotiation
aliases: [Intl.Locale, BCP 47 language tags in JavaScript, Unicode extension keys, supportedLocalesOf, Intl.getCanonicalLocales, localeMatcher, locale fallback]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  A locale is a BCP 47 tag of language, script, region and variants, plus -u- extension keys. Intl.Locale parses one, and each Intl constructor matches a wanted list against the locales it has.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#locale-and-parameter-negotiation", locator: "9 Locale and Parameter Negotiation; 15 Locale Objects"}
see_also: [javascript/internationalization, javascript/formatting-data, locale-aware-formatting]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Locale information; Figuring out the locale
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Locale
  title: MDN Web Docs, JavaScript Reference, Intl.Locale
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/Locale/maximize
  title: MDN Web Docs, JavaScript Reference, Intl.Locale.prototype.maximize()
  locator: the introduction; Description
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/getCanonicalLocales
  title: MDN Web Docs, JavaScript Reference, Intl.getCanonicalLocales()
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl
  title: MDN Web Docs, JavaScript Reference, Intl
  locator: locales argument; Locale identification and negotiation
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: 9 Locale and Parameter Negotiation
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/locale.html
  title: The Python Standard Library, Python 3.14, locale
  locator: locale.normalize
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Locale.html
  title: Java SE 25 API, Class Locale
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const loc = new Intl.Locale("en-Latn-US-u-ca-gregory-hc-h23");
console.log(loc.language, loc.script, loc.region, loc.calendar, loc.hourCycle);
console.log(loc.baseName, String(loc));
console.log(new Intl.Locale("en").maximize().baseName, new Intl.Locale("ko").maximize().baseName);
console.log(Intl.getCanonicalLocales(["EN-us", "Fr", "en-US"]));
```

```output for=basics
en Latn US gregory h23
en-Latn-US en-Latn-US-u-ca-gregory-hc-h23
en-Latn-US ko-Kore-KR
[ 'en-US', 'fr' ]
```

| Part | Example | `Intl.Locale` property |
|---|---|---|
| language | `en`, `sr`, `zh` | `language` |
| script | `Latn`, `Cyrl`, `Hant` | `script` |
| region | `US`, `GB`, `419` | `region` |
| `-u-ca-` | calendar | `calendar` |
| `-u-hc-` | hour cycle | `hourCycle` |
| `-u-nu-` | numbering system | `numberingSystem` |
| `-u-co-`, `-u-kf-`, `-u-kn-` | collation, case first, numeric | `collation`, `caseFirst`, `numeric` |

## Semantics

**r1.** "Locales underlie all behaviors of Intl . A locale is a set of conventions, represented in the Intl API by the Intl.Locale object."[^1] "Each locale is primarily defined by four things: a language , a script , a region , and sometimes some variants . When connected together by - in that order, they form a BCP 47 language tag."[^1] "The language is the most important part of the locale and is mandatory."[^1]

**r2.** "The Intl.Locale object is a standard built-in property of the Intl object that represents a Unicode locale identifier."[^2] "Additional information about the locale is stored in the optional extension tags. Extension tags hold information about locale aspects such as calendar type, clock type, and numbering system type."[^2] Example: [`basics`](#basics).

**r3.** A convention can go in the tag or in the options: "append them to the language tag or specify them as options."[^1] For the tag, "you first append the string -u (meaning "Unicode extension"), then the extension subtag as given above, then the value."[^1] "Unrecognized properties are ignored".[^1] Example: [`extensions`](#extensions).

**r4.** `Intl.Locale` does not look up defaults: "The information above is exactly provided as-is when the Locale object is constructed, without consulting any external database."[^2] To find the value a service will use, ask the service: "to get the default calendar of en-US as used by Intl.DateTimeFormat , you can use its resolvedOptions() method."[^1] Example: [`defaults`](#defaults).

**r5.** `maximize()` "gets the most likely values for the language, script, and region of this locale based on existing values."[^3] It "only affects the main subtags that comprise the language identifier: language, script, and region subtags."[^3] `Intl.getCanonicalLocales` "returns an array containing the canonical locale names. Duplicates will be omitted and elements will be validated as structurally valid language tags."[^4]

**r6.** Negotiation: the locales list, "after Unicode extensions have been removed from them, is interpreted as a prioritized request from the application. The runtime compares it against the locales it has available and picks the best one available."[^5] "Two matching algorithms exist: the "lookup" matcher follows the Lookup algorithm specified in BCP 47; the "best fit" matcher lets the runtime provide a locale that's at least, but possibly more, suited for the request than the result of the Lookup algorithm."[^5] The `localeMatcher` option picks one.[^5] ECMA-402 describes the request as "A Language Priority List", "a sequence of locale preferences by descending priority."[^6] Example: [`negotiate`](#negotiate).

**r7.** "There is no API for listing all supported locales".[^1] Each constructor has a `supportedLocalesOf()` method, which "takes the same arguments as the constructor (locales and options ) and returns a subset of the given locale tags that match the given data."[^1]

## Examples

The same calendar given in the tag and in the options (rule r3).

```javascript run id=extensions
const day = new Date(Date.UTC(2022, 0, 1));
const a = new Intl.DateTimeFormat("en-US-u-ca-hebrew", { timeZone: "UTC" });
const b = new Intl.DateTimeFormat("en-US", { calendar: "hebrew", timeZone: "UTC" });
console.log(a.format(day) === b.format(day), a.resolvedOptions().calendar);
console.log(new Intl.Locale("de", { collation: "phonebk" }).toString());
```

```output for=extensions
true hebrew
de-u-co-phonebk
```

Choosing from a list of wanted locales (rules r6 and r7).

```javascript run id=negotiate
const wanted = ["xx-YY", "de-AT", "de", "en"];
console.log(Intl.DateTimeFormat.supportedLocalesOf(wanted, { localeMatcher: "lookup" }));
console.log(new Intl.NumberFormat(wanted).resolvedOptions().locale);
console.log(new Intl.NumberFormat("de-AT-u-nu-arab").resolvedOptions().locale);
```

```output for=negotiate
[ 'de-AT', 'de', 'en' ]
de-AT
de-AT-u-nu-arab
```

## Pitfalls

### Reading defaults from an `Intl.Locale`

```javascript wrong id=defaults
const locale = new Intl.Locale("en-US");
console.log(locale.calendar, locale.hourCycle);
const used = new Intl.DateTimeFormat(locale, { timeStyle: "short" }).resolvedOptions();
console.log(used.calendar, used.hourCycle);
```

```output for=defaults
undefined undefined
gregory h12
```

The `Locale` holds only what was written in the tag or options (rule r4). The formatter's `resolvedOptions()` shows what it chose.

### Expecting an error for a locale with no data

```javascript wrong id=nodata
const fmt = new Intl.DateTimeFormat(["tlh"], { timeZone: "UTC" });
console.log(fmt.resolvedOptions().locale === "tlh");
console.log(Intl.DateTimeFormat.supportedLocalesOf(["tlh"]).length);
```

```output for=nodata
false
0
```

A well-formed tag with no data is skipped without an error, and the default locale is used (rule r6). Check with `supportedLocalesOf` when it matters.

## Comparison

Python's `locale.normalize` "Returns a normalized locale code for the given locale name. The returned locale code is formatted for use with setlocale() ."[^7] Java has `java.util.Locale`: "A Locale represents a specific geographical, political, or cultural region."[^8] JavaScript has both the tag as a string and `Intl.Locale` (rule r2), and each Intl object picks its own locale from a list (rule r6). See [[javascript/internationalization]].

## References
