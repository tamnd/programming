---
tag: 0ZSVJ2
kind: feature
owner: javascript
slug: formatting-data
name: Formatting data
aliases: [Intl.NumberFormat, Intl.DateTimeFormat, Intl.ListFormat, Intl.RelativeTimeFormat, Intl.DurationFormat, formatToParts, locale-aware formatting in JavaScript, toLocaleString]
area: languages/javascript/strings-and-text
level: core
summary: >-
  Intl.NumberFormat, DateTimeFormat, ListFormat, RelativeTimeFormat and DurationFormat turn numbers, dates, lists, time differences and durations into text by the rules of a locale.
grammar: null
spec: {standard: "ECMA-402, 13th edition (ECMAScript 2026 Internationalization API)", url: "https://tc39.es/ecma402/2026/#numberformat-objects", locator: "11 DateTimeFormat Objects; 13 DurationFormat Objects; 14 ListFormat Objects; 16 NumberFormat Objects; 18 RelativeTimeFormat Objects"}
see_also: [javascript/internationalization, javascript/bigints, locale-aware-formatting, format-a-number-with-separators, parse-and-format-a-date]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Internationalization
  title: MDN Web Docs, JavaScript Guide, Internationalization
  locator: Formatting data; Date and time formatting; Number formatting; List formatting; Relative time formatting; Duration formatting; Understanding the return value
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/NumberFormat
  title: MDN Web Docs, JavaScript Reference, Intl.NumberFormat
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/DateTimeFormat
  title: MDN Web Docs, JavaScript Reference, Intl.DateTimeFormat
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/ListFormat
  title: MDN Web Docs, JavaScript Reference, Intl.ListFormat
  locator: the introduction
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/RelativeTimeFormat
  title: MDN Web Docs, JavaScript Reference, Intl.RelativeTimeFormat
  locator: the introduction
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/library/locale.html
  title: The Python Standard Library, Python 3.14, locale
  locator: locale.format_string; locale.currency
  accessed: 2026-11-18
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/NumberFormat.html
  title: Java SE 25 API, Class NumberFormat
  locator: the class description
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
const when = new Date(Date.UTC(2026, 10, 18));
const utc = { timeZone: "UTC" };
console.log(new Intl.DateTimeFormat("en-US", { ...utc, dateStyle: "full" }).format(when));
console.log(new Intl.DateTimeFormat("en-GB", utc).format(when));
console.log(new Intl.DateTimeFormat("ja-JP", utc).format(when));
console.log(new Intl.NumberFormat("en-US", { style: "percent" }).format(0.256));
console.log(new Intl.ListFormat("en-US", { type: "disjunction" }).format(["tea", "coffee", "milk"]));
console.log(new Intl.RelativeTimeFormat("en-US", { numeric: "auto" }).format(-1, "day"));
```

```output for=basics
Wednesday, November 18, 2026
18/11/2026
2026/11/18
26%
tea, coffee, or milk
yesterday
```

| Object | Input | Example output |
|---|---|---|
| `Intl.NumberFormat` | a number, a BigInt or a string | `1,234,567.89`, `$5.26`, `12K` |
| `Intl.DateTimeFormat` | a `Date` | `18/11/2026` |
| `Intl.ListFormat` | an array of strings | `tea, coffee, or milk` |
| `Intl.RelativeTimeFormat` | a number and a unit | `yesterday`, `in 2 days` |
| `Intl.DurationFormat` | an object of duration fields | `3 hours, 4 minutes, 5 seconds` |

## Semantics

**r1.** "A major use case of Intl is to output locale-specific texts representing structured data. This is similar to translation software, but instead of letting you translate arbitrary text, it takes data such as dates, numbers, and lists, and formats them according to locale-specific rules."[^1] Each formatter is built with a locale and options, as in "The Intl.NumberFormat object enables language-sensitive number formatting."[^2] "The Intl.DateTimeFormat object enables language-sensitive date and time formatting."[^3] Example: [`basics`](#basics). See [[javascript/internationalization]].

**r2.** Each formatter has `format()` and `formatToParts()`. The second "Takes the same data and returns the same string, but broken down into parts, each part being an object with a type and a value ."[^1] Example: [`parts`](#parts).

**r3.** A `Date` is one instant, shown in a calendar and a time zone: "Date objects represent a unique instant in the user's time zone and in the ISO 8601 calendar".[^1] "They are converted to the given calendar and timeZone by preserving the instant, so the date and time components may change."[^1] The options name the components to show, or use `dateStyle` and `timeStyle`; "You should pick one of these two methods, because they are mutually exclusive. Using both methods simultaneously will result in an error."[^1] Example: [`zones`](#zones).

**r4.** "The NumberFormat object accepts inputs in the form of numbers, strings, or BigInt values. Passing a string or BigInt instead of a number allows you to format numbers that are too large or too small to be represented precisely as a JavaScript number."[^1] The `style` option is one of `decimal`, `percent`, `currency` and `unit`.[^1] A currency is shown "applying specific currency symbols and rounding rules."[^1] The `notation` option can be `scientific`, `engineering` or `compact`.[^1] Example: [`numbers`](#numbers). See [[javascript/bigints]].

**r5.** "The Intl.ListFormat object enables language-sensitive list formatting."[^4] It "takes an array of strings and joins them in a locale-specific way such that the result represents a conjunction (and), disjunction (or), or a list of units."[^1] "The Intl.RelativeTimeFormat object enables language-sensitive relative time formatting."[^5] It "takes relative times in the form of two arguments: a number (with any sign) and a time unit".[^1] `Intl.DurationFormat` "uses Intl.NumberFormat and Intl.ListFormat internally to format each duration component, then joins them with a list separator."[^1] Example: [`lists`](#lists).

**r6.** "For casual use, you can also directly call the toLocaleString() method on the data, passing the locale and options as arguments."[^1] But "When the method is called many times with the same arguments, it is better to create a formatter object and use its format() method".[^1] Example: [`reuse`](#reuse).

**r7.** The exact text is not fixed: "the output may vary between implementations, even within the same locale"[^1] and "the string returned by format() may use non-breaking spaces or be surrounded by bidirectional control characters. You should not compare the results of any Intl method to hardcoded constants; they should only be displayed to users."[^1] Example: [`constant`](#constant).

## Examples

The parts of a formatted price, and currency rounding (rules r2 and r4).

```javascript run id=parts
const price = new Intl.NumberFormat("en-US", { style: "currency", currency: "USD" });
console.log(price.formatToParts(5.259).map((p) => `${p.type}:${p.value}`).join(" "));
console.log(new Intl.NumberFormat("en-US", { style: "currency", currency: "JPY" }).format(1234.5));
```

```output for=parts
currency:$ integer:5 decimal:. fraction:26
¥1,235
```

The yen has no minor unit, so the amount is rounded to a whole number.

Notation, digits and a BigInt (rule r4).

```javascript run id=numbers
for (const notation of ["scientific", "engineering", "compact"]) {
  console.log(notation, new Intl.NumberFormat("en-US", { notation }).format(12000));
}
console.log(new Intl.NumberFormat("en-US", { maximumSignificantDigits: 4 }).format(1234.56789));
console.log(new Intl.NumberFormat("en-US").format(12345678901234567890n));
```

```output for=numbers
scientific 1.2E4
engineering 12E3
compact 12K
1,235
12,345,678,901,234,567,890
```

One instant in two time zones and in the Hebrew calendar (rule r3).

```javascript run id=zones
const instant = new Date(Date.UTC(2022, 0, 1));
for (const timeZone of ["America/New_York", "Asia/Tokyo"]) {
  console.log(new Intl.DateTimeFormat("en-US", { dateStyle: "long", timeZone }).format(instant));
}
const hebrew = { dateStyle: "long", calendar: "hebrew", timeZone: "UTC" };
console.log(new Intl.DateTimeFormat("en-US", hebrew).format(instant));
try {
  new Intl.DateTimeFormat("en-US", { dateStyle: "long", year: "numeric" });
} catch (e) {
  console.log(e.name);
}
```

```output for=zones
December 31, 2021
January 1, 2022
28 Tevet 5782
TypeError
```

Lists, relative times and durations (rule r5).

```javascript run id=lists
const fruits = ["apple", "banana", "cherry"];
console.log(new Intl.ListFormat("en-US").format(fruits));
console.log(new Intl.ListFormat("de-DE").format(fruits));
const rtf = new Intl.RelativeTimeFormat("en-US", { numeric: "auto" });
console.log(rtf.format(1, "day"), "/", rtf.format(2, "day"), "/", rtf.format(-1, "hour"));
const df = new Intl.DurationFormat("en-US", { style: "long" });
console.log(df.format({ hours: 3, minutes: 4, seconds: 5 }));
```

```output for=lists
apple, banana, and cherry
apple, banana und cherry
tomorrow / in 2 days / 1 hour ago
3 hours, 4 minutes, 5 seconds
```

## Pitfalls

### Comparing formatted text with a string constant

```javascript wrong id=constant
const euro = new Intl.NumberFormat("de-DE", { style: "currency", currency: "EUR" });
const shown = euro.format(1234.5);
console.log(shown === "1.234,50 €", shown.length);
const wide = [...shown].filter((c) => c.codePointAt(0) > 0x7e);
console.log(wide.map((c) => "U+" + c.codePointAt(0).toString(16).toUpperCase().padStart(4, "0")));
```

```output for=constant
false 10
[ 'U+00A0', 'U+20AC' ]
```

The space before the euro sign is U+00A0 NO-BREAK SPACE in this run, so the string does not equal the one typed in the source with an ordinary space (rule r7). Test formatted text by its parts, or not at all.

### Calling `toLocaleString` with the same options in a loop

```javascript wrong id=reuse
const prices = [5.259, 10, 0.5];
const opts = { style: "currency", currency: "USD" };
console.log(prices.map((p) => p.toLocaleString("en-US", opts)).join(" "));

const fmt = new Intl.NumberFormat("en-US", opts);
console.log(prices.map((p) => fmt.format(p)).join(" "));
```

```output for=reuse
$5.26 $10.00 $0.50
$5.26 $10.00 $0.50
```

The output is the same, but each `toLocaleString` call "has to perform a search in a big database of localization strings."[^1] Build the formatter once (rule r6).

## Comparison

Python formats with the process locale: `locale.format_string` formats "a number val according to the current LC_NUMERIC setting",[^6] and `locale.currency` formats "a number val according to the current LC_MONETARY settings."[^6] Java's `NumberFormat` "provides the interface for formatting and parsing numbers in a localized manner."[^7] In JavaScript each formatter carries its own locale and options (rule r1). See [[locale-aware-formatting]] and [[format-a-number-with-separators]].

## References
