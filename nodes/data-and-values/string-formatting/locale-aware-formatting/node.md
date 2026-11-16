---
tag: 843W4A
kind: concept
slug: locale-aware-formatting
name: Locale-aware formatting
aliases: [locale-aware formatting, localized formatting, locale formatting, number localization, l10n of numbers, culture-specific formatting, CultureInfo, Intl.NumberFormat, setlocale, LC_NUMERIC, CLDR, ICU, decimal separator, digit grouping, thousands separator]
area: data-and-values/string-formatting
level: core
summary: >-
  Locale-aware formatting shows a number, a price or a date with the symbols and the digit groups of a language and region, so 1234.5 is 1,234.5 in English and 1.234,5 in German.
applies_to: core-languages
viz: optional
formal: none
see_also: [format-specifiers, string-interpolation, print-formatted-output]
same_as:
  wikidata: Q1202471
  wikipedia: Locale_(computer_software)
sources:
- url: https://en.wikipedia.org/wiki/Decimal_separator
  title: Decimal separator, Wikipedia
  locator: Introduction; Digit grouping; Current international standards
- url: https://cldr.unicode.org/
  title: Unicode CLDR Project
  locator: What is CLDR
- url: https://www.unicode.org/reports/tr35/tr35-numbers.html
  title: Unicode Technical Standard #35, Part 3, Numbers
  locator: Number Patterns; Grouping
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/NumberFormat
  title: Intl.NumberFormat, MDN Web Docs
  locator: Description; Examples
- url: https://en.cppreference.com/w/c/locale/setlocale.html
  title: setlocale, C reference, cppreference.com
  locator: Notes
- url: https://en.cppreference.com/w/c/locale/lconv.html
  title: lconv, C reference, cppreference.com
  locator: Member objects
- url: https://docs.python.org/3/library/locale.html
  title: locale, Internationalization services, Python documentation
  locator: Background, details, hints, tips and caveats; LC_NUMERIC
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Format Specification Mini-Language
- url: https://en.cppreference.com/w/cpp/utility/format/spec.html
  title: Standard format specification, C++ reference, cppreference.com
  locator: L (locale-specific formatting)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Class Formatter, Java SE 25 API
  locator: Localization
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
  title: Standard numeric format strings, .NET documentation
  locator: Number format specifier (N); NumberFormatInfo properties
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Localization
---

## Idea

People write numbers in different ways. "A decimal separator is a symbol that separates the integer part from the fractional part of a number written in decimal form. Different countries officially designate different symbols for use as the separator"[^1]. The number 1234567.891 is `1,234,567.891` in the United States, `1.234.567,891` in Germany and `12,34,567.891` in India.

A **locale** is a set of rules for a language and a region, such as `en-US`, `de-DE` or `hi-IN`. A locale-aware format function takes a value and a locale, and it uses the symbols and the groups of that locale:

```javascript run id=idea
const n = 1234567.891;
for (const loc of ["en-US", "de-DE", "en-IN", "ja-JP"]) {
  console.log(loc.padEnd(6), new Intl.NumberFormat(loc).format(n));
}
```

```output for=idea
en-US  1,234,567.891
de-DE  1.234.567,891
en-IN  12,34,567.891
ja-JP  1,234,567.891
```

The value is the same in each line. Only the text is different.

## How it works

**The locale data.** A program does not know the rules of each country. It reads them from a database. The main database today is CLDR: "CLDR (Common Locale Data & Rules) supplies key information and structures critical for programs and operating systems around the world to ensure that they feel natural, no matter which language users speak or where they live"[^2]. It has data for "Over 100 languages"[^2]: the decimal and group symbols, the digit groups, the currency symbols, the names of months and days, and the date patterns. The ICU libraries, the JavaScript `Intl` API, Java and .NET use the CLDR data. Unix systems have a second database, the POSIX locales, which C and Python use.

**What a locale changes in a number.**

- **The decimal separator**: `.` in English, `,` in German and French.
- **The group separator**: `,` in English, `.` in German, a space in French and `'` in Swiss German.
- **The group sizes.** Most locales use groups of 3. "There may be two different grouping sizes: The primary grouping size used for the least significant integer group, and the secondary grouping size used for more significant groups; these are not the same in all locales"[^3]. India uses 3 for the first group and 2 for the others.
- **The digits.** Some locales use other digits, for example the Arabic-Indic digits in Egypt.
- **The currency.** The symbol, its place before or after the number, and the number of decimals: the euro has 2, and the Japanese yen has 0[^4].

**Two ways to give the locale.**

- **A global locale.** C has one locale for the program. At startup, a C program runs the equivalent of `setlocale(LC_ALL, "C")` before any user code[^5], so it uses the neutral rules until it calls `setlocale`. The C function `localeconv` returns a struct with `decimal_point`, `thousands_sep` and `grouping`[^6]. Python's module `locale` uses the same model.
- **A locale argument.** Java, .NET and JavaScript take the locale as an argument of each call, or from an object: `String.format(Locale.GERMANY, ...)`, `x.ToString("N2", culture)` and `new Intl.NumberFormat("de-DE")`. This is clearer, because the result of a call depends only on its arguments.

**The neutral locale.** Each system has one locale that never changes: the `"C"` locale in C and Python, `Locale.ROOT` in Java and `CultureInfo.InvariantCulture` in .NET. A program uses it for text that another program reads.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | the module `locale` with `setlocale`, `format_string` and the format type `n`; the library Babel for CLDR data |
| JavaScript | stdlib | `Intl.NumberFormat`, `Intl.DateTimeFormat` and `toLocaleString`, with the CLDR data |
| TypeScript | stdlib | as in JavaScript |
| Java | stdlib | a `Locale` argument for `String.format`, and `NumberFormat`, `DecimalFormat` and `DateTimeFormatter` |
| Kotlin | stdlib | the Java classes, through `String.format(locale, ...)` |
| C# | stdlib | `CultureInfo` and `NumberFormatInfo`, through `ToString("N2", culture)` and `string.Format(culture, ...)` |
| Go | library | no locale in the package `fmt`; the module `golang.org/x/text/message` adds it |
| Rust | library | no locale in `std::fmt`; crates such as `num-format` and `icu` add it |
| C | stdlib | `setlocale`, `localeconv`, `strfmon` and the POSIX flag `'` in `printf` |
| C++ | stdlib | `std::locale`, the option `L` of `std::format` and the facets of `<locale>` |
| Bash | native | the builtin `printf` with the flag `'`, after `LC_NUMERIC` is set |
| PowerShell | stdlib | the .NET `CultureInfo`; `-f` and `ToString` use the current culture |
| SQL | idiom | `to_char` with the patterns `G` and `D` in PostgreSQL and Oracle, and `FORMAT` with a culture in SQL Server |
| Regex | n/a | patterns match text and do not format values |
| HTML | n/a | the attribute `lang` names the language of the text, but HTML does not format numbers |
| CSS | n/a | CSS does not format numbers |

**Python.** The module `locale` sets a global locale. "Initially, when a program is started, the locale is the C locale, no matter what the user’s preferred locale is"[^7]. Only some functions use it: "The functions format_string(), atoi(), atof() and str() of the locale module are affected by that category. All other numeric formatting operations are not affected"[^7]. The format type `n` is the exception: "This is the same as 'd', except that it uses the current locale setting to insert the appropriate digit group separators"[^8]. The option `,` always gives commas.

**C++.** `std::format` ignores the locale unless the specifier has `L`. "For integral types, the locale-specific form inserts the appropriate digit group separator characters according to the context's locale"[^9].

**Java and C#.** The methods take a locale. Java's documentation shows `formatter.format(Locale.FRANCE, "e = %+10.4f", Math.E)`, which gives `e = +2,7183`[^10]. In .NET, the format `N` gives "Integral and decimal digits, group separators, and a decimal separator with optional negative sign"[^11], and "The result string is affected by the formatting information of the current NumberFormatInfo object"[^11]. Without an argument, both use the locale of the user, so the same program can print different text on two computers.

**Go and Rust.** The standard formatting has no locale. "The format functions provided by Rust’s standard library do not have any concept of locale and will produce the same results on all systems regardless of user configuration"[^12]. A program that must show local numbers uses a library.

## Variations and related ideas

- **Dates and times.** A locale also gives the order of day, month and year, the names of the months, and the 12-hour or 24-hour clock. `Intl.DateTimeFormat`, `DateTimeFormatter.ofLocalizedDate` and the format `d` in .NET use it.
- **Plural forms and messages.** A full translation also needs plural rules and message formats, such as ICU MessageFormat. This is internationalization, and the numbers are one part of it.
- **Parsing.** The reverse operation reads a number from local text: `locale.atof`, `NumberFormat.parse` and `double.Parse(s, culture)`.
- **Grouping without a locale.** `{:,}` in Python and `N0` with the invariant culture give fixed commas. See [[format-a-number-with-separators]].

## Pitfalls

- **Machine text with a user locale.** A number in a log, a CSV file, JSON or an SQL query must use the neutral locale. If a program writes `3,5` on a German computer, a reader that expects `3.5` fails, or reads a wrong value. Pass the neutral locale explicitly: `Locale.ROOT`, `CultureInfo.InvariantCulture` or no `setlocale` call.
- **Parsing with the wrong locale.** In German, `.` is the group separator, so a parser can read `1.5` as `15`:

```csharp wrong id=parse-wrong-culture
using System;
using System.Globalization;

var de = CultureInfo.GetCultureInfo("de-DE");
Console.WriteLine(double.Parse("1.5", de));
Console.WriteLine(double.Parse("1.5", CultureInfo.InvariantCulture));
```

```output for=parse-wrong-culture
15
1.5
```

- **A global locale is shared.** `setlocale` changes the locale of the whole program. Python warns: "It is generally a bad idea to call setlocale() in some library routine, since as a side effect it affects the entire program"[^7]. Other threads see the change too.
- **The locale is not installed.** On Linux, `setlocale(LC_ALL, "de_DE.UTF-8")` fails if the system does not have that locale. Check the return value. CLDR-based APIs carry their own data, but a small build can omit it.
- **Non-ASCII separators.** In the current CLDR data, the French group separator is a narrow no-break space, U+202F, not a normal space. A test that compares with `"1 234,5"` fails, and the CLDR data changes between versions. Do not compare localized text in tests, or build the expected text from the same API.
- **Special values.** Java says of NaN and infinity: "These values are not localized"[^10]. Other APIs give `NaN`, `∞` or a local word.

## Cost

A locale-aware call is slower than a plain format, because it looks up the locale data and applies more rules. Creating a formatter, such as `Intl.NumberFormat` or `DecimalFormat`, parses the locale data, so a program creates it once and reuses it in a loop. The CLDR data is large: a full ICU data file has tens of megabytes, so some runtimes ship a smaller set.

## History

The locale model of C came with the first C standard, which defined `setlocale`, `localeconv` and the categories `LC_NUMERIC`, `LC_MONETARY` and `LC_TIME`[^5]. The decimal symbol was a question for standards bodies too: in 2003, the 22nd General Conference on Weights and Measures declared "The symbol for the decimal marker shall be either the point on the line or the comma on the line"[^1]. The Unicode Consortium started CLDR to give all systems the same locale data, and the JavaScript `Intl` object put it in the browser: `Intl.NumberFormat` has been "available across browsers since September 2017"[^4].

## References
