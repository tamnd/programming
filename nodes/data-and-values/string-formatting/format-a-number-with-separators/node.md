---
tag: ZNQ468
kind: task
slug: format-a-number-with-separators
name: Format a number with separators
aliases: [thousands separator, digit grouping, group separator, comma separator, commatize, format with commas, number with commas, "%'d", "{:,}", toLocaleString, N0 format, grouping separator]
area: data-and-values/string-formatting
level: intro
summary: >-
  Write an integer with a comma between each group of 3 digits, as in 1,234,567, with a format option, a locale API or a short loop. The result must not change with the locale of the computer.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Commatizing-numbers/00-TASK.txt
  title: Rosetta Code, Commatizing numbers (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Decimal_separator
  title: Decimal separator, Wikipedia
  locator: Introduction; Digit grouping
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Format Specification Mini-Language
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Intl/NumberFormat
  title: Intl.NumberFormat, MDN Web Docs
  locator: Examples
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Class Formatter, Java SE 25 API
  locator: Flags; Number Localization Algorithm
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
  title: Standard numeric format strings, .NET documentation
  locator: Number format specifier (N); NumberFormatInfo properties
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Localization
- url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fprintf.html
  title: fprintf, The Open Group Base Specifications Issue 8 (POSIX.1-2024)
  locator: DESCRIPTION; flag characters
- url: https://en.cppreference.com/w/cpp/utility/format/spec.html
  title: Standard format specification, C++ reference, cppreference.com
  locator: L (locale-specific formatting)
---

## Task

Write an integer in decimal with a separator between each group of 3 digits, so that a person can read a large number quickly. The Rosetta Code task Commatizing numbers describes a function that "adds commas (or some other characters, including blanks or tabs) to the first numeric part of a string"[^1]. This task does the main part of that work for one integer.

The function `group_thousands(n)` returns the digits of `n` with a comma before each group of 3 digits, counted from the right. A negative number has a minus sign first.

| `n` | Result |
|---|---|
| `1234567` | `1,234,567` |
| `1000` | `1,000` |
| `42` | `42` |
| `-1234567` | `-1,234,567` |

The separator is not the same in all countries. Wikipedia lists "Three ways to group the number ten thousand with digit group separators"[^2]: a space, a full stop and a comma. The size of the groups also changes: India puts the first separator after 3 digits and the next ones after each 2 digits. This task uses one fixed format, a comma and groups of 3, so the result is the same on each computer. For a format that follows the user's locale, see [[locale-aware-formatting]].

## Solutions

| Language | Fixed format, comma and groups of 3 | Locale format |
|---|---|---|
| Python | `f"{n:,}"` | `locale.format_string("%d", n, grouping=True)` |
| JavaScript, TypeScript | a loop or a regular expression | `n.toLocaleString("en-US")` |
| Java | `String.format(Locale.ROOT, "%,d", n)` | `String.format("%,d", n)` |
| C# | `n.ToString("N0", CultureInfo.InvariantCulture)` | `n.ToString("N0")` |
| Go | a loop over the digits | `golang.org/x/text/message`, outside the standard library |
| Rust | a loop over the digits | a crate, outside the standard library |
| C | a loop over the digits | `printf("%'lld", n)` after `setlocale` |
| C++ | a loop over the digits | `std::format(loc, "{:L}", n)` |
| Bash | a loop with a regular expression | `printf "%'d"` in a locale with grouping |

- **Python.** The format option `,` "Inserts a comma every 3 digits for integer presentation type 'd' and floating-point presentation types, excluding 'n'"[^3]. The option `_` puts an underscore. The type `n` uses the separator of the locale.
- **JavaScript.** `toLocaleString` and `Intl.NumberFormat` use the rules of a locale. The locale `en-US` gives commas and groups of 3. MDN shows that "India uses thousands/lakh/crore separators"[^4], with the result `1,23,456.789`. Both work with `BigInt`, so a 64-bit value keeps all its digits.
- **TypeScript.** The same as JavaScript. The type of the parameter is `number | bigint`.
- **Java.** With the flag `,`, "The result will include locale-specific grouping separators"[^5]. `Locale.ROOT` gives a comma and groups of 3. Without a locale, the default locale of the computer is used, so the result can change.
- **C#.** The format `N` gives "Integral and decimal digits, group separators, and a decimal separator with optional negative sign"[^6], and `N0` gives no decimals. The culture gives the separator: `NumberGroupSeparator` "Defines the string that separates groups of integral numbers"[^6]. The invariant culture gives a comma.
- **Go.** The package `fmt` has no grouping. The solution writes the digits with `strconv.FormatInt` and adds a comma before each group of 3.
- **Rust.** The standard library has no grouping and no locale. Its format functions "do not have any concept of locale and will produce the same results on all systems regardless of user configuration"[^7]. The solution uses a loop.
- **C.** POSIX adds the flag `'` (an apostrophe) to `printf`: "The integer portion of the result of a decimal conversion (%i, %d, %u, %f, %F, %g, or %G) shall be formatted with thousands' grouping characters"[^8]. The character comes from the locale, and the default C locale has no separator. Standard C has no grouping, so the portable solution uses a loop.
- **C++.** `std::format` has the option `L`: "For integral types, the locale-specific form inserts the appropriate digit group separator characters"[^9]. The locale must be given or set, so the solution uses a loop.
- **Bash.** `printf "%'d"` uses the locale of the shell. The solution uses a loop that adds one comma at a time with `[[ =~ ]]`.

The Python solution, with the other options of the format:

```python run id=python-grouping
n = 1234567
print(f"{n:,}")
print(f"{n:_}")
print(f"{-9223372036854775808:,}")
print(f"{1234567.891:,.2f}")
```

```output for=python-grouping
1,234,567
1_234_567
-9,223,372,036,854,775,808
1,234,567.89
```

The loop is the same in all languages. Write the digits of the absolute value, then copy them and put a comma before each digit whose distance from the end is a multiple of 3. The Go solution:

```go run id=go-grouping
package main

import (
	"fmt"
	"strconv"
	"strings"
)

func groupThousands(n int64) string {
	s := strconv.FormatInt(n, 10)
	sign := ""
	if s[0] == '-' {
		sign, s = "-", s[1:]
	}
	var b strings.Builder
	b.WriteString(sign)
	for i, c := range s {
		if i > 0 && (len(s)-i)%3 == 0 {
			b.WriteByte(',')
		}
		b.WriteRune(c)
	}
	return b.String()
}

func main() {
	for _, n := range []int64{0, 999, 1000, -1234567, -9223372036854775808} {
		fmt.Println(groupThousands(n))
	}
}
```

```output for=go-grouping
0
999
1,000
-1,234,567
-9,223,372,036,854,775,808
```

## Edge cases

- **Zero and numbers under 1000.** No separator: `0`, `42` and `999`.
- **The boundary of a group.** `100000` is `100,000` and `1000000` is `1,000,000`. A loop that counts from the left gives `1,000,00` or puts a comma first.
- **Negative numbers.** The minus sign is not a digit. `-999` is `-999`, not `-,999`.
- **The smallest 64-bit value.** `-9223372036854775808` has no positive value in 64 bits, so `abs(n)` or `-n` overflows. Convert the number to text first, and then take off the sign, as the Go solution does.
- **Numbers with a fraction.** Group only the digits before the decimal point. The digits after the point are not grouped in most styles.

## Pitfalls

- **A regular expression that also groups the fraction.** A common one line solution puts a comma at each position that has a multiple of 3 digits after it. It is correct for an integer, but it also changes the digits after the decimal point:

```javascript wrong id=javascript-regex-fraction
const group = (s) => s.replace(/\B(?=(\d{3})+(?!\d))/g, ",");
console.log(group("1234567"));
console.log(group("1234.5678"));
```

```output for=javascript-regex-fraction
1,234,567
1,234.5,678
```

Split the text at the decimal point and group only the integer part, or use `Intl.NumberFormat`.

- **The locale of the computer.** Java `String.format("%,d", n)`, C# `n.ToString("N0")`, C `%'d` and Bash `%'d` all use the locale of the computer. The same program gives `1,234,567` in the United States and `1.234.567` in Germany. Data for a file or for another program must use a fixed format. Give the locale explicitly: `Locale.ROOT`, `CultureInfo.InvariantCulture` or `en-US`.
- **Separators in data.** A number with separators is text for people. Most parsers do not read it back: `int("1,234")` fails in Python, and `parseInt("1,234")` in JavaScript stops at the comma and gives `1`.
- **The size of the groups.** Not all locales use groups of 3. A loop with a fixed group size is correct only for the format that the program chose.

## Related tasks

- [[format-a-number-with-fixed-decimals]]: the digits after the decimal point.
- [[print-formatted-output]]: a number in a column of fixed width.
- [[locale-aware-formatting]]: the separators of each locale.

## References
