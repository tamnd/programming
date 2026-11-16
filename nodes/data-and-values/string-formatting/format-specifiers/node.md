---
tag: 3SSY7M
kind: concept
slug: format-specifiers
name: Format specifiers
aliases: [format specifier, format string, conversion specification, printf format, format spec, format_spec, format mini-language, field width, precision, alignment, verbs, "%d", "%s", "%5.2f", "{:>8}", composite formatting]
area: data-and-values/string-formatting
level: core
summary: >-
  A format specifier is a short code in a format string, such as %5.2f or {:>8}, that tells a format function how to show one value: its width, alignment, precision, sign and base.
applies_to: core-languages
viz: optional
formal: none
see_also: [string-interpolation, print-formatted-output, locale-aware-formatting]
same_as:
  wikidata: Q838016
  wikipedia: Printf
sources:
- url: https://en.cppreference.com/w/c/io/fprintf.html
  title: printf, fprintf, sprintf, snprintf, C reference, cppreference.com
  locator: Parameters; conversion specification
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Class Formatter, Java SE 25 API
  locator: Format String Syntax
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Printing; Format errors
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Format Specification Mini-Language
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Positional parameters; Syntax
- url: https://en.cppreference.com/w/cpp/utility/format/spec.html
  title: Standard format specification, C++ reference, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/composite-formatting
  title: Composite formatting, .NET documentation
  locator: Format item syntax
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
  title: Standard numeric format strings, .NET documentation
  locator: Introduction
- url: https://en.wikipedia.org/wiki/Printf
  title: printf, Wikipedia
  locator: History; Uncontrolled format string exploit
---

## Idea

A format function takes a format string and a list of values. The format string is text with codes in it, and each code tells the function how to show one value. In C, "The format string consists of ordinary byte characters (except %), which are copied unchanged into the output stream, and conversion specifications"[^1]. A conversion specification starts with `%` and ends with a letter for the type:

```c run id=idea
#include <stdio.h>

int main(void) {
    printf("[%d] [%5d] [%-5d] [%05d]\n", 42, 42, 42, 42);
    printf("[%f] [%.2f] [%8.3f] [%+.1f]\n", 3.14159, 3.14159, 3.14159, 3.14159);
    printf("[%s] [%6s] [%-6s] [%.2s]\n", "abc", "abc", "abc", "abc");
    printf("[%x] [%#o] [%e] [%%]\n", 255, 8, 12345.678);
    return 0;
}
```

```output for=idea
[42] [   42] [42   ] [00042]
[3.141590] [3.14] [   3.142] [+3.1]
[abc] [   abc] [abc   ] [ab]
[ff] [010] [1.234568e+04] [%]
```

`%5d` is an integer in a field of 5 characters, `%-5d` is the same on the left, and `%05d` fills with zeros. `%.2f` is a float with 2 digits after the point, and `%8.3f` adds a width of 8. For a string, the precision `%.2s` is the largest number of characters. `%%` gives one `%`.

## How it works

**The parts of a specifier.** Most specifiers have the same parts, in this order: flags, a width, a precision and a type. In C, "(optional) integer value or *: specifies minimum field width. The result is padded with space characters (by default), if required, on the left when right-justified, or on the right if left-justified"[^1]. The precision is "(optional) . followed by non-negative integer number or *"[^1]: the number of digits after the point for a float, the smallest number of digits for an integer, and the largest number of characters for a string.

**Two families.** The format strings of today come in two families.

- **The printf family** uses `%` and a type letter: C, C++ `printf`, Java, Go, Bash `printf` and PHP. Java's syntax is `%[argument_index$][flags][width][.precision]conversion`[^2], and "Formatted printing for the Java language is heavily inspired by C's printf"[^2]. Go's package fmt says: "The format 'verbs' are derived from C's but are simpler"[^3].
- **The brace family** uses `{}` with an optional index or name and a specifier after `:`: Python, Rust and C++ `std::format`. Python's general form has the options fill, align, sign, `z`, `#` and `0`, then a width, a grouping, a precision and a type[^4]. Rust has `format_spec := [[fill]align][sign]['#']['0'][width]['.' precision][type]`[^5], and for C++ `std::format`, "the format specification is based on the format specification in Python"[^6].

.NET is a third form: a format item is `{index[,width][:formatString]}`[^7], and the format string after the colon is a letter and a precision, such as `F2` or `N0`. "A standard numeric format string takes the form [format specifier][precision specifier]"[^8].

**The same field in each family.** The table shows one field, a float in 8 characters with 2 decimals:

| Meaning | C, Java, Go | Python, Rust, C++ `std::format` | C# |
|---|---|---|---|
| right-aligned, width 8, 2 decimals | `%8.2f` | `{:8.2f}` (Rust: `{:8.2}`) | `{0,8:F2}` |
| left-aligned | `%-8.2f` | `{:<8.2f}` | `{0,-8:F2}` |
| zero fill | `%08.2f` | `{:08.2f}` | `{0:00000.00}` |
| plus sign | `%+.2f` | `{:+.2f}` | `{0:+0.00;-0.00}` |
| hexadecimal | `%x` | `{:x}` | `{0:x}` |
| any fill, centered | none | `{:*^8}` | none |

In the brace family, a number is right-aligned and a string is left-aligned by default, and any character can be the fill. In Python, `,` and `_` group digits: "',' Inserts a comma every 3 digits for integer presentation type 'd' and floating-point presentation types"[^4].

**Default formats.** A placeholder with no specifier uses the default text of the value. "A general convention is that an empty format specification produces the same result as if you had called str() on the value"[^4]. Go's `%v` is "the value in a default format"[^3], and `{}` in Rust and C++ is the default form.

**Arguments by position.** A format string can pick its arguments by number, so one value can appear twice, or a translation can change the order. Rust says that if the index is omitted, "it is assumed to be “the next argument”"[^5]. Java uses `%1$s`, Go uses `%[1]s`, and Python, Rust and C++ use `{0}`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | the format mini-language in f-strings, `str.format` and `format`, and the older `%` operator |
| JavaScript | absent | no format strings; `toFixed`, `padStart`, `toString(16)` and `Intl.NumberFormat` |
| TypeScript | absent | as in JavaScript |
| Java | stdlib | `java.util.Formatter`, through `String.format`, `formatted` and `printf` |
| Kotlin | stdlib | `String.format` and `"...".format(...)` from Java |
| C# | stdlib | composite formatting `{0,8:F2}`, in `string.Format`, `Console.WriteLine` and `$"..."` |
| Go | stdlib | the package `fmt`, with verbs such as `%v`, `%d` and `%8.2f` |
| Rust | native | the macros `format!` and `println!`, checked at compile time |
| C | stdlib | `printf`, `snprintf` and the other functions of `<stdio.h>` |
| C++ | stdlib | `std::format` and `std::print` with the brace syntax, and `printf` from C |
| Bash | native | the builtin `printf`, with the specifiers of C |
| PowerShell | native | the operator `-f`, with the .NET format items: `"{0,8:F2}" -f $x` |
| SQL | idiom | no standard format strings; functions such as `to_char`, `FORMAT` or `printf` in some databases |
| Regex | n/a | patterns match text and do not format values |
| HTML | n/a | HTML shows text and does not format values |
| CSS | n/a | CSS does not format numbers; counters have styles such as `decimal-leading-zero` |

**C and Bash.** C trusts the format string: "If any argument after default argument promotions is not the type expected by the corresponding conversion specification"[^1], "the behavior is undefined"[^1]. Compilers warn when the format string is a literal. The Bash builtin `printf` uses the same specifiers and repeats the format if there are more values.

**Java and Go.** Both check the types at run time. "Java formatting is more strict than C's; for example, if a conversion is incompatible with a flag, an exception will be thrown"[^2]. Go puts the error into the output: "If an invalid argument is given for a verb, such as providing a string to %d, the generated string will contain a description of the problem"[^3], for example `%!d(string=hi)`.

**Rust and C++.** The format string must be a literal, so the compiler checks it against the types of the values. In Rust, "A format string is required to use all of its arguments, otherwise it is a compile-time error"[^5].

## Variations and related ideas

- **Interpolation with a specifier.** `f"{price:.2f}"`, `$"{price:F2}"` and `format!("{price:.2}")` put the specifier inside the interpolated string. See [[string-interpolation]].
- **Locale.** A specifier can use the decimal separator and the digit groups of a locale: `%'d` in POSIX `printf`, the type `n` in Python, `L` in C++ and `N` in .NET. See [[locale-aware-formatting]].
- **Custom formats.** A type can define its own specifiers: `__format__` in Python, `IFormattable` in .NET, `std::formatter` in C++ and the traits of `std::fmt` in Rust. Dates have their own codes, such as `%Y-%m-%d`.
- **Scanning.** `scanf` and `fmt.Sscanf` use the same specifiers to read values from text.

## Pitfalls

- **A format string from a user.** If a program passes outside text as the format string, the text can contain specifiers. "the format string can be the value of a variable, which allows for dynamic formatting but also a security vulnerability known as an uncontrolled format string exploit"[^9]. In C, `%n` writes to memory, so this is a serious weakness. Write `printf("%s", text)`, not `printf(text)`.
- **The wrong type.** In C, `%d` with a `long long` or `%f` with an `int` is undefined behavior. Use `%lld`, `%zu` for `size_t`, and the macros of `<inttypes.h>` for the fixed-width types. Java throws an exception, and Go prints a description of the error:

```go wrong id=go-wrong-verb
package main

import "fmt"

func main() {
	fmt.Printf("%d items\n", "three")
	fmt.Printf("%d and %d\n", 1)
	fmt.Printf("%.2f\n", 3)
}
```

```output for=go-wrong-verb
%!d(string=three) items
1 and %!d(MISSING)
%!f(int=03)
```

`go vet` reports these calls before the program runs.

- **Width is a minimum.** A width never cuts a value. `%3d` of 12345 gives `12345`, so a table column moves when a value is long. A precision cuts a string, but not a number.
- **Width counts characters, not columns.** A width counts bytes in C and Bash, UTF-16 units in Java and C#, and code points in Python and Rust. A wide character such as a Chinese ideograph takes two columns on a terminal, so a padded table can still look wrong. See [[pad-a-string]].
- **Rounding.** `%.2f` rounds the binary value of the float, not the decimal number in the source. `2.675` is a little less than 2.675 in binary, so C and Python show `2.67`. See [[format-a-number-with-fixed-decimals]].
- **A literal brace or percent sign.** In the printf family, write `%%`. In the brace family, write `{{` and `}}`.

## Cost

A format function parses the format string each time it runs, unless the compiler does the work: `format!` in Rust, `std::format` with a literal in C++, and interpolated strings in C# are parsed at compile time. The conversion of each value costs much more than the parse, in particular for floats, where the shortest correct digits need a special algorithm. For a hot loop, a format string that does not change is cheap, and a new string for each line is the main cost.

## History

The printf family is old. "In 1967, BCPL appeared"[^9], and its library had the routine `writef`, which acted as a function call. "In 1973, printf was included as a C standard library routine as part of Version 4 Unix"[^9]. "In 1990, the printf command-line program, modeled after the C standard library function, was included with 4.3BSD-Reno"[^9]. "In 2004, Java 5.0 (1.5) released, which extended the class java.io.PrintStream, adding a method named printf()"[^9]. Python added `str.format` with its brace syntax, and C++ took that syntax for `std::format`[^6].

## References
