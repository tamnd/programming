---
tag: NEW-parse-an-integer
kind: task
slug: parse-an-integer
name: Parse an integer
aliases: [convert a string to an integer, string to int, parse a number from a string, read an integer in a given base]
area: data-and-values/primitive-types
level: core
summary: Parse a string as a signed 64-bit integer in a base from 2 to 36, with a strict syntax, and report bad text and a value out of range as two different errors.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/functions.html#int
    title: The Python Standard Library, Built-in Functions, int
    locator: "class int(x, base=10): white space, underscores, Unicode decimal digits, the prefixes 0b, 0o and 0x, the allowed bases 0 and 2 to 36"
  - url: https://docs.python.org/3/library/stdtypes.html#integer-string-conversion-length-limitation
    title: The Python Standard Library, Built-in Types, Integer string conversion length limitation
    locator: the default limit of 4300 digits; the ValueError
  - url: https://docs.python.org/3/library/exceptions.html#OverflowError
    title: The Python Standard Library, Built-in Exceptions
    locator: OverflowError
  - url: https://tc39.es/ecma262/#sec-parseint-string-radix
    title: ECMAScript Language Specification, parseInt (string, radix)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER
    title: MDN Web Docs, Number.MAX_SAFE_INTEGER
    locator: Description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Long.html#parseLong(java.lang.String,int)
    title: Java SE 25 API, java.lang.Long
    locator: parseLong(String, int)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Character.html#digit(char,int)
    title: Java SE 25 API, java.lang.Character
    locator: digit(char, int)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.int64.parse?view=net-10.0
    title: .NET API reference, Int64.Parse
    locator: Parse(String, NumberStyles, IFormatProvider), Exceptions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.globalization.numberstyles?view=net-10.0
    title: .NET API reference, NumberStyles
    locator: Fields (AllowLeadingSign, Integer)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.convert.toint64?view=net-10.0#system-convert-toint64(system-string-system-int32)
    title: .NET API reference, Convert.ToInt64(String, Int32)
    locator: Exceptions; Remarks
  - url: https://pkg.go.dev/strconv#ParseInt
    title: Go standard library, package strconv
    locator: ParseInt; NumError; ErrRange; ErrSyntax
  - url: https://doc.rust-lang.org/std/primitive.i64.html#method.from_str_radix
    title: The Rust Standard Library, primitive type i64
    locator: from_str_radix (Panics)
  - url: https://doc.rust-lang.org/std/num/enum.IntErrorKind.html
    title: The Rust Standard Library, std::num::IntErrorKind
    locator: Empty, InvalidDigit, PosOverflow, NegOverflow
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/strtol.html
    title: The Open Group, POSIX.1-2024, strtol, strtoll
    locator: Description; Return Value; Errors; Application Usage
  - url: https://en.cppreference.com/w/cpp/utility/from_chars
    title: cppreference.com, std::from_chars
    locator: Description; Return value
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Arithmetic
    title: GNU Bash Reference Manual, Shell Arithmetic
    locator: fixed-width integers with no check for overflow; base#n; a shell variable in an expression
  - url: https://www.unicode.org/Public/UCD/latest/ucd/CaseFolding.txt
    title: Unicode Character Database, CaseFolding.txt
    locator: "212A; C; 006B; # KELVIN SIGN"
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/isalnum.html
    title: The Open Group, POSIX.1-2024, isalnum, isalnum_l
    locator: Description
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap09.html#tag_09_03_05
    title: The Open Group, POSIX.1-2024, Base Definitions, Regular Expressions
    locator: 9.3.5 RE Bracket Expression, range expressions
---

## Task

Parse a string as a signed 64-bit integer. The function takes the string and a base from 2 to 36. It returns the value, or it reports an error.

The syntax is strict. The string is one optional `+` or `-` and then one or more digits of the base. The digits are the ASCII characters `0` to `9` for the values 0 to 9, and the letters `a` to `z` or `A` to `Z` for the values 10 to 35. Each digit value is below the base. Leading zeros are allowed, and they do not change the base. Nothing else is allowed: no white space, no underscore, no thousands separator, no prefix such as `0x`, no decimal point and no exponent.

The value is from -2^63 to 2^63 - 1. A solution reports two different errors, and a caller can tell them apart. An invalid string does not have the syntax above. An out-of-range string has the syntax, but its value is outside the range.

Text from a file, a command line, an environment variable or a network message comes in as a string. A strict parser rejects bad input at the edge of the program. A lax parser accepts `"42px"` as 42, or a value that does not fit as some other value.

## Solutions

- **Python.** `int(s, base)` also accepts white space, underscores, every Unicode decimal digit and a `0x` prefix in base 16, and it has no upper limit. So the code checks each character against the digits of the base first. Then it calls `int` and checks the range. It raises `ValueError` for an invalid string and `OverflowError` for a value out of range.
- **JavaScript and TypeScript.** `Number.parseInt` returns a double and stops at the first bad character (see Pitfalls). The code gets the value of each digit from its character code, adds it to a `BigInt` value, and returns a `bigint`. It throws `SyntaxError` or `RangeError`. The TypeScript file adds types only.
- **Java.** `Long.parseLong(s, radix)` accepts a sign and rejects white space. It also accepts digits outside ASCII, because it uses `Character.digit`, and it throws `NumberFormatException` for both errors. So the code checks the syntax first, and it changes a later `NumberFormatException` into an `ArithmeticException`.
- **C#.** In base 10, `long.Parse` with `NumberStyles.AllowLeadingSign` and the invariant culture matches the contract. It throws `FormatException` or `OverflowException`. `Convert.ToInt64(s, base)` takes only the bases 2, 8, 10 and 16, so the code has its own loop for the other bases. The loop uses `checked` arithmetic.
- **Go.** `strconv.ParseInt(s, base, 64)` matches the contract. The error is a `*strconv.NumError`. `errors.Is(err, strconv.ErrSyntax)` and `errors.Is(err, strconv.ErrRange)` tell the two errors apart. Base 0 reads a prefix, so the code rejects it.
- **Rust.** `i64::from_str_radix(s, radix)` matches the contract. `err.kind()` is `Empty` or `InvalidDigit` for an invalid string, and `PosOverflow` or `NegOverflow` for a value out of range. It panics when the radix is not from 2 to 36.
- **C.** The code checks each character with its own digit table, because `isalnum` depends on the locale. Then it sets `errno` to 0 and calls `strtoll`. `ERANGE` in `errno` means the value is out of range. The function returns a status and writes the value through a pointer.
- **C++.** `std::from_chars` takes no white space and no prefix, and it reports `std::errc::result_out_of_range`. It does not accept a `+` sign, so the code removes one. It then checks that `from_chars` read the whole string. The function returns `std::expected<std::int64_t, std::errc>`.
- **Bash.** `[[ $s =~ $re ]]` checks the syntax with a bracket expression that lists each digit of the base. A loop adds one digit at a time and checks for overflow before each step. The function prints the value and returns 1 for an invalid string and 2 for a value out of range.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. The inputs that must fail are in `cases/errors.jsonl`.

- `plus-sign` and `negative-zero`: `+5` is 5, and `-0` is 0. An integer has no negative zero.
- `leading-zeros` and `many-leading-zeros`: `007` is 7 in base 10, not an octal value. The string of 5,000 zeros and then `42` is valid. Python `int` rejects it because of its digit limit, so the Python code removes the leading zeros first.
- `max`, `min`, `max-hex` and `min-binary`: the limits of the range. The minimum has no positive counterpart. So the C# loop adds each digit toward the sign of the result, and the Bash loop adds each digit toward the negative side.
- `upper-case` and `mixed-case`: the letters of a digit can be in either case.
- `above-safe-integer`: 2^53 + 1 has no exact double value. A parser that goes through a double gives 2^53.
- `trailing-newline`: a line from a file or from standard input often ends in a newline. Remove it before you parse, and do not let the parser skip it.
- `hex-prefix`, `underscore` and `thousands-separator`: the base is an argument, and the digits have no separators. Accept other syntax only on purpose, in a separate step.
- `arabic-indic-digits`, `fullwidth-digits` and `kelvin-sign`: Python `int` and Java `parseLong` read the first two as 12. U+212A KELVIN SIGN is not the letter K, but Unicode case folding maps it to `k`. So a case-insensitive match with Unicode rules can accept it as a digit. The solutions accept only the ASCII letters.
- `hex-sign-bit` and `hex-all-ones`: these are the bit patterns of -2^63 and -1. Their values are 2^63 and 2^64 - 1, so they are out of range. `Convert.ToInt64` reads them as negative numbers.
- `min-times-ten` and `many-digits`: a loop that stops at the limit must still report an error.
- A string with too many digits and a bad character is in neither error class alone. Go and Rust stop at the first overflow and report a range error. The other solutions check the syntax first.

## Pitfalls

- **`Number.parseInt` in JavaScript is lax.** It skips leading white space, stops at the first bad character, reads `0x` in base 16, and returns a double. `Number("")` is 0:

```javascript wrong id=js-parseint
console.log(Number.parseInt(" 42px", 10));
console.log(Number.parseInt("1e3", 10));
console.log(Number.parseInt("0x1A", 16));
console.log(Number.parseInt("9007199254740993", 10));
console.log(Number(""));
```

```output for=js-parseint
42
1
26
9007199254740992
0
```

- **Python `int` accepts more than digits.** It accepts white space, underscores, Unicode digits and a `0x` prefix in base 16. Its result can be any size:

```python wrong id=python-int-lax
print(int(" 42 "))
print(int("1_000"))
print(int("١٢"))
print(int("0x1A", 16))
print(int("9223372036854775808"))
```

```output for=python-int-lax
42
1000
12
26
9223372036854775808
```

- **Python limits the length of the string.** A decimal string longer than 4,300 digits raises `ValueError`, even when its value is small. The limit protects against denial-of-service attacks:

```python run id=python-int-limit
s = "0" * 5000 + "42"
try:
    print(int(s))
except ValueError as e:
    print("ValueError:", e)
print(int(s.lstrip("0") or "0"))
```

```output for=python-int-limit
ValueError: Exceeds the limit (4300 digits) for integer string conversion: value has 5002 digits; use sys.set_int_max_str_digits() to increase the limit
42
```

- **Java `parseLong` has one exception for two errors.** The exception for a bad character and the exception for a value out of range have the same type. `parseLong` also accepts Arabic-Indic digits:

```java run id=java-parselong
public class Main {
  public static void main(String[] args) {
    System.out.println(Long.parseLong("١٢"));
    for (String s : new String[] {"12x", "9223372036854775808"}) {
      try {
        Long.parseLong(s);
      } catch (NumberFormatException e) {
        System.out.println(e.getClass().getSimpleName() + ": " + e.getMessage());
      }
    }
  }
}
```

```output for=java-parselong
12
NumberFormatException: For input string: "12x"
NumberFormatException: For input string: "9223372036854775808"
```

- **C# `Convert.ToInt64` with a base accepts `0x` and wraps.** In base 16 it reads a value above `long.MaxValue` as a negative number. `long.Parse` with the default style allows white space at both ends:

```csharp wrong id=csharp-convert
using System.Globalization;

Console.WriteLine(Convert.ToInt64("0x1A", 16));
Console.WriteLine(Convert.ToInt64("ffffffffffffffff", 16));
Console.WriteLine(long.Parse(" 42 ", CultureInfo.InvariantCulture));
```

```output for=csharp-convert
26
-1
42
```

- **Go `ParseInt` returns a value with a range error.** The value is the nearest limit, so check `err` before you use it. Base 0 reads a prefix, underscores and a leading `0` as octal:

```go run id=go-parseint
package main

import (
	"fmt"
	"strconv"
)

func main() {
	fmt.Println(strconv.ParseInt("9223372036854775808", 10, 64))
	fmt.Println(strconv.ParseInt("017", 0, 64))
	fmt.Println(strconv.ParseInt("1_000", 0, 64))
}
```

```output for=go-parseint
9223372036854775807 strconv.ParseInt: parsing "9223372036854775808": value out of range
15 <nil>
1000 <nil>
```

- **A line from input still has its newline.** Rust `parse` does not skip white space, so a line that still ends in `\n` fails. Remove the line ending first:

```rust run id=rust-newline
fn main() {
    let line = "42\n";
    println!("{:?}", line.parse::<i64>());
    println!("{:?}", line.trim_end().parse::<i64>());
}
```

```output for=rust-newline
Err(ParseIntError { kind: InvalidDigit })
Ok(42)
```

- **C `strtoll` reads a prefix of the string.** It skips leading white space, stops at the first bad character, and reads `0x` in base 16. Pass an end pointer and check that it is at the end of the string. On overflow it returns `LLONG_MAX` or `LLONG_MIN` and sets `errno` to `ERANGE`. It does not set `errno` to 0 on success, so set it to 0 before the call:

```c wrong id=c-strtoll
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *inputs[] = {" 42", "42px", "0x1A", "9223372036854775808"};
    const int bases[] = {10, 10, 16, 10};
    for (int i = 0; i < 4; i++) {
        char *end = NULL;
        errno = 0;
        long long value = strtoll(inputs[i], &end, bases[i]);
        printf("\"%s\": %lld, rest \"%s\", %s\n", inputs[i], value, end,
               errno == ERANGE ? "ERANGE" : "no error");
    }
    return 0;
}
```

```output for=c-strtoll
" 42": 42, rest "", no error
"42px": 42, rest "px", no error
"0x1A": 26, rest "", no error
"9223372036854775808": 9223372036854775807, rest "", ERANGE
```

- **C++ `from_chars` stops early and rejects `+`.** It returns a pointer to the first character that it did not read. On a range error it does not change the value:

```cpp run id=cpp-from-chars
#include <charconv>
#include <cstdint>
#include <print>
#include <string_view>
#include <system_error>

int main() {
    for (std::string_view s : {"+5", "42px", "9223372036854775808"}) {
        std::int64_t value = -1;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
        const char *error = ec == std::errc{}                     ? "no error"
                            : ec == std::errc::invalid_argument ? "invalid_argument"
                                                                : "result_out_of_range";
        std::println("\"{}\": value {}, read {} of {} chars, {}", s, value, ptr - s.data(), s.size(),
                     error);
    }
}
```

```output for=cpp-from-chars
"+5": value -1, read 0 of 2 chars, invalid_argument
"42px": value 42, read 2 of 4 chars, no error
"9223372036854775808": value -1, read 19 of 19 chars, result_out_of_range
```

- **Bash arithmetic wraps and runs code.** Shell arithmetic uses fixed-width integers and does not check for overflow (see [[integer-overflow]]). A variable in an arithmetic expression is itself read as an expression, so a subscript with `$(...)` runs a command. Never put input text in `$((...))` before you check it (see [[os-command-injection]]):

```bash wrong id=bash-arithmetic
exec 2>&1
s=9223372036854775808
echo $((10#$s))
x='a[$(echo "this command ran" >&2)]'
echo $((x))
```

```output for=bash-arithmetic
-9223372036854775808
this command ran
0
```

## Related tasks

- [[read-from-standard-input]]: read a line, remove its newline, and then parse it.
- [[parse-command-line-arguments]]: an option such as `-n 10` needs an integer parser.
- [[read-an-environment-variable]]: a port number or a size comes in as a string.
- [[split-a-string]]: split a line into fields, and then parse each field.
- [[integer-overflow]]: what happens when arithmetic goes out of range.

## References
