---
tag: 118D22
kind: task
slug: print-formatted-output
name: Print formatted output
aliases: [printf, formatted print, print a table, format a line, String.format, fmt.Printf, println!, Console.WriteLine, std::print, f-string, column alignment, fixed width output, print with padding]
area: data-and-values/string-formatting
level: intro
summary: >-
  Print a line of a table with a format: a name on the left in 10 columns, a count on the right in 5 and a price with 2 decimals in 8, with printf, format, println! and f-strings.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Formatted-numeric-output/00-TASK.txt
  title: Rosetta Code, Formatted numeric output (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.cppreference.com/w/c/io/fprintf.html
  title: printf, fprintf, sprintf, snprintf, C reference, cppreference.com
  locator: Parameters; Return value
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Format Specification Mini-Language
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/padStart
  title: String.prototype.padStart(), MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/toFixed
  title: Number.prototype.toFixed(), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Class Formatter, Java SE 25 API
  locator: Format String Syntax
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/composite-formatting
  title: Composite formatting, .NET documentation
  locator: Width component
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Printing; Sprintf
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Width; Precision; Related macros
- url: https://en.cppreference.com/w/cpp/utility/format/format.html
  title: std::format, C++ reference, cppreference.com
  locator: Description
---

## Task

Print values in fixed columns, so that the lines of a table line up. The Rosetta Code task asks for one case of this: "Express a number in decimal as a fixed-length string with leading zeros"[^1]. This task does more: it formats a line with three columns of three types.

The function `format_row(name, qty, price)` returns one line:

- `name`, a string, on the left of a column of 10 characters;
- a bar `|`;
- `qty`, an integer, on the right of a column of 5 characters;
- a bar `|`;
- `price`, a float, rounded to 2 decimals, on the right of a column of 8 characters.

In C notation, the format is `"%-10s|%5d|%8.2f"`. For example, `format_row("apple", 3, 1.5)` is `"apple     |    3|    1.50"`. A program prints the lines one after the other:

```text
apple     |    3|    1.50
widget    |   12|   19.99
laptop    |    1| 1234.50
```

A width is a minimum, not a maximum. A value that is wider than its column is not cut, so the line becomes longer. In C, the width "specifies minimum field width. The result is padded with space characters (by default), if required, on the left when right-justified, or on the right if left-justified"[^2].

The function returns the line, and the caller prints it. Then the tests can check the text, and the same function can write to a file or a socket.

## Solutions

Each tier 1 language has a format function in its standard library, except JavaScript.

| Language | Function | Format |
|---|---|---|
| Python | f-string | `f"{name:<10}\|{qty:5d}\|{price:8.2f}"` |
| JavaScript, TypeScript | `padEnd`, `padStart`, `toFixed` | no format string |
| Java | `String.format` | `"%-10s\|%5d\|%8.2f"` |
| C# | `string.Format` | `"{0,-10}\|{1,5}\|{2,8:F2}"` |
| Go | `fmt.Sprintf` | `"%-10s\|%5d\|%8.2f"` |
| Rust | `format!` | `"{name:<10}\|{qty:5}\|{price:8.2}"` |
| C | `snprintf` | `"%-10s\|%5lld\|%8.2f"` |
| C++ | `std::format` | `"{:<10}\|{:5}\|{:8.2f}"` |
| Bash | `printf -v` | `'%-10s\|%5d\|%8.2f'` |

- **Python.** An f-string with a format specifier after each colon. `<` forces the field to be left-aligned: "'<' Forces the field to be left-aligned within the available space (this is the default for most objects)"[^3]. To print, call `print(format_row(...))`.
- **JavaScript.** JavaScript has no format strings. `toFixed(2)` gives the 2 decimals, and `padStart` and `padEnd` give the width. `padStart` "pads this string with a given string (repeated and/or truncated, if needed) so that the resulting string has a given length"[^4]. The function `toFixed` returns the number "without using exponential notation and with exactly digits digits after the decimal point"[^5].
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** `String.format` with the specifiers of C. The width "is a positive decimal integer indicating the minimum number of characters to be written to the output"[^6]. The solution passes `Locale.ROOT`, so the decimal point is always `.`.
- **C#.** `string.Format` with format items. In `{0,-10}`, the number after the comma is the width, and a minus sign puts the value on the left. "If the value of width is less than the length of the formatted string, width is ignored, and the length of the formatted string is used as the field width"[^7]. The solution passes `CultureInfo.InvariantCulture`.
- **Go.** `fmt.Sprintf` "formats according to a format specifier and returns the resulting string"[^8]. `fmt.Printf` prints the same text.
- **Rust.** The macro `format!` with named arguments. `println!` prints the same text: it is the same as `print!`, "but appends a newline"[^9].
- **C.** `snprintf` writes into a buffer of a given size and returns the length of the full text. The solution returns that length, so the caller can see if the buffer was too small.
- **C++.** `std::format` returns a `std::string`. It "Format args according to the format string fmt, and return the result as a string"[^10]. `std::print` prints the same text.
- **Bash.** The builtin `printf -v line` writes the text into the variable `line`. Without `-v`, `printf` prints the text.

The Python solution and a loop that prints a table:

```python run id=python-table
def format_row(name, qty, price):
    return f"{name:<10}|{qty:5d}|{price:8.2f}"


rows = [("apple", 3, 1.5), ("widget", 12, 19.99), ("laptop", 1, 1234.5)]
print(f"{'item':<10}|{'qty':>5}|{'price':>8}")
print("-" * 10 + "+" + "-" * 5 + "+" + "-" * 8)
for row in rows:
    print(format_row(*row))
```

```output for=python-table
item      |  qty|   price
----------+-----+--------
apple     |    3|    1.50
widget    |   12|   19.99
laptop    |    1| 1234.50
```

The same line in Go, with `Printf`:

```go run id=go-printf
package main

import "fmt"

func main() {
	fmt.Printf("%-10s|%5d|%8.2f\n", "apple", 3, 1.5)
	fmt.Printf("%-10s|%5d|%8.2f\n", "watermelonade", 123456, 123456.75)
}
```

```output for=go-printf
apple     |    3|    1.50
watermelonade|123456|123456.75
```

The second line shows that a long value moves the columns after it.

## Edge cases

- **Empty name.** The name column is 10 spaces.
- **A value of the exact width.** `abcdefghij`, `99999` and `99999.99` fill their columns with no spaces.
- **A value wider than its column.** The value is not cut, and the line is longer. A table that must stay aligned needs the widest value first: see the task `align-columns-of-text`.
- **Negative numbers.** The minus sign counts in the width: `-3.50` takes 5 of the 8 characters.
- **Rounding.** `0.006` gives `0.01`, and `2.344` gives `2.34`. The contract has no value that is halfway between two results, because the languages do not round those values in the same way.
- **Spaces and format characters in the name.** A name such as `100% {x}` is a value, not a format string, so the `%` and the braces are copied as they are.

## Pitfalls

- **The locale of the computer.** In Java and C#, a format call with no locale uses the locale of the user. On a computer with a German locale, `String.format("%.2f", 1.5)` gives `1,50`. Pass `Locale.ROOT` or `CultureInfo.InvariantCulture` for text that a program reads. See [[locale-aware-formatting]].
- **The wrong type for a specifier.** In C, `%d` with a `long long` is undefined behavior. Use `%lld`. In Go, a wrong type gives a text such as `%!d(string=3)` in the output.
- **The value as the format string.** Never pass a value as the format: `printf(name)` treats each `%` in the name as a specifier. Write `printf("%s", name)`.
- **A width cuts nothing.** A long value moves the next columns:

```c wrong id=c-width-does-not-cut
#include <stdio.h>

int main(void) {
    printf("%-5s|%3d|\n", "pen", 7);
    printf("%-5s|%3d|\n", "notebook", 12345);
    return 0;
}
```

```output for=c-width-does-not-cut
pen  |  7|
notebook|12345|
```

To cut a string, give a precision: `%-5.5s`. Do not cut numbers, because a cut number is a wrong number.

- **Wide characters.** A width counts bytes in C and Bash, UTF-16 units in Java, C# and JavaScript, and code points in Python and Rust. A name with accents or Chinese characters can make a column too wide or too narrow. The contract uses ASCII names.
- **`toFixed` and large numbers.** In JavaScript, `toFixed` gives exponential notation for numbers of 1e21 or more[^5]. The contract keeps the price below 1e9.

## Related tasks

- [[interpolate-values-into-a-string]]: build a string with values and expressions inside.
- [[format-specifiers]]: the codes of the format strings, in each family.
- [[locale-aware-formatting]]: numbers with the symbols of a language and region.
- [[string-interpolation]]: values inside string literals.

## References
