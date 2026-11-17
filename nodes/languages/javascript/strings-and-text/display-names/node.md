---
tag: RTJPF8
kind: feature
owner: javascript
slug: display-names
name: Display names
aliases: [Intl.DisplayNames, localized language names, localized region names, currency names in JavaScript, Intl.supportedValuesOf, languageDisplay]
area: languages/javascript/strings-and-text
level: advanced
summary: >-
  Intl.DisplayNames turns a code for a language, region, script, currency, calendar or date field into its name in a given locale, such as US to United States or EUR to Euro.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#intl-displaynames-objects", locator: "12 DisplayNames Objects; 12.1.1 Intl.DisplayNames; 12.3.3 Intl.DisplayNames.prototype.of"}
see_also: [javascript/internationalization, javascript/locale-information-and-negotiation, javascript/formatting-data]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DisplayNames
  title: MDN Web Docs, JavaScript Reference, Intl.DisplayNames
  locator: the introduction; Examples
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Display names
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DisplayNames/DisplayNames
  title: MDN Web Docs, JavaScript Reference, Intl.DisplayNames() constructor
  locator: Parameters; Exceptions
  accessed: 2026-11-18
- url: https://tc39.es/ecma402/2026/
  title: ECMAScript 2026 Internationalization API Specification (ECMA-402, 13th edition)
  locator: 12.1.1 Intl.DisplayNames
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DisplayNames/of
  title: MDN Web Docs, JavaScript Reference, Intl.DisplayNames.prototype.of()
  locator: Parameters; Return value; Exceptions
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/supportedValuesOf
  title: MDN Web Docs, JavaScript Reference, Intl.supportedValuesOf()
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Locale.html
  title: Java SE 25 API, Class Locale
  locator: getDisplayName
  accessed: 2026-11-18
- url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.cultureinfo.displayname
  title: .NET API reference, CultureInfo.DisplayName Property
  locator: Property Value; Remarks
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const regions = new Intl.DisplayNames(["en"], { type: "region" });
const chineseRegions = new Intl.DisplayNames(["zh-Hant"], { type: "region" });
console.log(regions.of("US"), "/", regions.of("419"), "/", chineseRegions.of("US"));
const languages = new Intl.DisplayNames(["en"], { type: "language" });
console.log(languages.of("fr"), "/", languages.of("fr-CA"), "/", languages.of("zh-Hant"));
const currencies = new Intl.DisplayNames(["en"], { type: "currency" });
const scripts = new Intl.DisplayNames(["en"], { type: "script" });
console.log(currencies.of("EUR"), "/", scripts.of("Kana"));
```

```output for=basics
United States / Latin America / 美國
French / Canadian French / Traditional Chinese
Euro / Katakana
```

| `type` | Code passed to `of()` | Example |
|---|---|---|
| `language` | a language tag | `fr-CA` gives `Canadian French` |
| `region` | ISO 3166 or UN M49 | `419` gives `Latin America` |
| `script` | ISO 15924 | `Kana` gives `Katakana` |
| `currency` | ISO 4217 | `EUR` gives `Euro` |
| `calendar` | a calendar name | `gregory` gives `Gregorian Calendar` |
| `dateTimeField` | `year`, `month`, `day` and others | `month` in `pt` gives `mês` |

## Semantics

**r1.** "The Intl.DisplayNames object enables the consistent translation of language, region and script display names."[^1] "It's like a formatter, but it's not template-based. Instead, it is a direct mapping from language-agnostic identifiers to localized names. It supports formatting languages, regions, scripts (the three subfields of a BCP 47 language tag), currency, calendar, and date-time fields."[^2] Example: [`basics`](#basics).

**r2.** `type` is required. It is "The type of display names to return from of()", one of `language`, `region`, `script`, `currency`, `calendar` and `dateTimeField`.[^3] Without it the constructor throws: "If type is undefined, throw a TypeError exception."[^4] Example: [`notype`](#notype).

**r3.** `of(code)` "receives a code and returns a string based on the locale and options provided when instantiating this Intl.DisplayNames object."[^5] The code must fit the type: a region is "either a two-letter ISO 3166 region code, or a three-digit UN M49 geographic region", a script "a four-letter ISO 15924 script code", and a currency "a three-letter ISO 4217 currency code."[^5] A code that is not well formed for the type throws a RangeError.[^5] Example: [`fields`](#fields).

**r4.** `fallback` sets "What to return from of() if the input is structurally valid but there's no matching display name."[^3] With `code`, the default, `of()` returns the input; with `none` it returns `undefined`.[^3] Example: [`fallback`](#fallback).

**r5.** `languageDisplay`, which works only with the `language` type, is `dialect`, the default, which shows "special regional dialects using their own name", or `standard`, which shows "all languages using standard format."[^3] `style` is `narrow`, `short` or `long`, and the default is `long`.[^3] Example: [`dialect`](#dialect).

**r6.** `Intl.supportedValuesOf()` "returns an array containing the supported calendar, collation, currency, numbering systems, or unit values supported by the implementation."[^6] With `DisplayNames` it gives the list and the names for a picker: "you may want to show the calendars in the user's language, or unabbreviate them. The Intl.DisplayNames object is useful for this."[^2] Example: [`picker`](#picker).

## Examples

Calendar names and date field names (rule r3).

```javascript run id=fields
const calendars = new Intl.DisplayNames("en", { type: "calendar" });
console.log(calendars.of("gregory"), "/", calendars.of("chinese"));
const fields = new Intl.DisplayNames("pt", { type: "dateTimeField" });
console.log(["year", "month", "day"].map((f) => fields.of(f)));
```

```output for=fields
Gregorian Calendar / Chinese Calendar
[ 'ano', 'mês', 'dia' ]
```

Dialect names and standard names (rule r5).

```javascript run id=dialect
for (const languageDisplay of ["dialect", "standard"]) {
  const dn = new Intl.DisplayNames("en", { type: "language", languageDisplay });
  console.log(languageDisplay, dn.of("en-GB"), "/", dn.of("nl-BE"));
}
```

```output for=dialect
dialect British English / Flemish
standard English (United Kingdom) / Dutch (Belgium)
```

Names for a list of supported codes (rule r6).

```javascript run id=picker
const names = new Intl.DisplayNames("en", { type: "currency" });
const wanted = ["EUR", "JPY", "USD"];
const codes = Intl.supportedValuesOf("currency").filter((c) => wanted.includes(c));
console.log(codes.map((c) => `${c} ${names.of(c)}`));
```

```output for=picker
[ 'EUR Euro', 'JPY Japanese Yen', 'USD US Dollar' ]
```

## Pitfalls

### Taking the returned code for a name

```javascript wrong id=fallback
const regions = new Intl.DisplayNames("en", { type: "region" });
console.log(regions.of("XY"));
const strict = new Intl.DisplayNames("en", { type: "region", fallback: "none" });
console.log(strict.of("XY"));
try {
  regions.of("USA1");
} catch (e) {
  console.log(e.name);
}
```

```output for=fallback
XY
undefined
RangeError
```

`XY` is a well-formed region code with no name, so the default fallback hands the code back and it looks like a result. Use `fallback: "none"` to tell the two apart (rule r4). A code of the wrong shape throws (rule r3).

### Leaving out `type`

```javascript wrong id=notype
try {
  new Intl.DisplayNames("en");
} catch (e) {
  console.log(e.name);
}
console.log(new Intl.DisplayNames("en", { type: "language" }).of("de"));
```

```output for=notype
TypeError
German
```

The constructor has no default for `type`, so it must always be given (rule r2).

## Comparison

Java's `Locale` has `getDisplayName`, which "Returns a name for the locale that is appropriate for display to the user", and `getDisplayScript` and similar methods for the parts.[^7] .NET's `CultureInfo.DisplayName` "Gets the full localized culture name", and the docs say "You should use the returned name for display, and not attempt to parse it."[^8] JavaScript's `Intl.DisplayNames` takes the code and the type separately, and covers currencies, calendars and date fields as well (rule r1). See [[javascript/locale-information-and-negotiation]].

## References
