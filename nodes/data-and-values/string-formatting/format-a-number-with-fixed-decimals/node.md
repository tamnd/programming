---
tag: QDVN9N
kind: task
slug: format-a-number-with-fixed-decimals
name: Format a number with fixed decimals
aliases: [fixed decimals, fixed-point notation, round to 2 decimals, two decimal places, "%.2f", "{:.2f}", toFixed, F2 format, decimal places, format a float, round for display]
area: data-and-values/string-formatting
level: intro
summary: >-
  Write a floating-point number with exactly n digits after the decimal point, with %.2f, toFixed, F2 or {:.2}. The function rounds the stored binary value, so 2.675 gives 2.67.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Formatted-numeric-output/00-TASK.txt
  title: Rosetta Code, Formatted numeric output (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://docs.python.org/3/tutorial/floatingpoint.html
  title: Floating-Point Arithmetic, Issues and Limitations, The Python Tutorial
  locator: 15.1 Representation Error
- url: https://docs.python.org/3/library/string.html
  title: string, Common string operations, Python documentation
  locator: Format Specification Mini-Language
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/toFixed
  title: Number.prototype.toFixed(), MDN Web Docs
  locator: Description; Examples
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Formatter.html
  title: Class Formatter, Java SE 25 API
  locator: Float and Double
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/math/BigDecimal.html
  title: Class BigDecimal, Java SE 25 API
  locator: BigDecimal(double val)
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/standard-numeric-format-strings
  title: Standard numeric format strings, .NET documentation
  locator: Precision specifier; Fixed-point format specifier (F)
- url: https://pkg.go.dev/fmt
  title: Package fmt, The Go Standard Library
  locator: Printing; Width and precision
- url: https://doc.rust-lang.org/std/fmt/index.html
  title: Module std::fmt, The Rust Standard Library
  locator: Precision
- url: https://en.cppreference.com/w/c/io/fprintf.html
  title: printf, fprintf, sprintf, snprintf, C reference, cppreference.com
  locator: Parameters; conversion specification
- url: https://en.cppreference.com/w/cpp/utility/format/spec.html
  title: Standard format specification, C++ reference, cppreference.com
  locator: Type
---

## Task

Write a number with a fixed count of digits after the decimal point, for example a price with 2 decimals or a measure with 3. The Rosetta Code task Formatted numeric output asks a similar question: "Express a number in decimal as a fixed-length string with leading zeros"[^1].

The function `fixed(x, digits)` returns `x` in decimal notation, without an exponent, with exactly `digits` digits after the point. The point is a dot on each computer.

| `x` | `digits` | Result |
|---|---|---|
| `3.14159` | 2 | `3.14` |
| `1.0` | 3 | `1.000` |
| `1234.5678` | 2 | `1234.57` |
| `123.456` | 0 | `123` |

A floating-point number is a binary fraction, so most decimal numbers are stored as a value that is a little more or a little less. For `0.1`, the Python tutorial shows that "the exact number stored in the computer is equal to the decimal value 0.1000000000000000055511151231257827021181583404541015625"[^2]. The function rounds this stored value, not the text that was written in the program. So `fixed(2.675, 2)` is `2.67`: the stored value is a little less than 2.675.

## Solutions

| Language | Expression |
|---|---|
| Python | `f"{x:.{digits}f}"` |
| JavaScript, TypeScript | `x.toFixed(digits)` |
| Java | `new BigDecimal(x).setScale(digits, RoundingMode.HALF_EVEN).toPlainString()` |
| C# | `x.ToString("F" + digits, CultureInfo.InvariantCulture)` |
| Go | `strconv.FormatFloat(x, 'f', digits, 64)` |
| Rust | `format!("{x:.digits$}")` |
| C | `snprintf(out, size, "%.*f", digits, x)` |
| C++ | `std::format("{:.{}f}", x, digits)` |
| Bash | `awk` with `printf "%.*f"` |

- **Python.** The type `f` is "Fixed-point notation. For a given precision p, formats the number as a decimal number with exactly p digits following the decimal point"[^3]. Python does not use the locale here.
- **JavaScript.** `toFixed` "returns a string representation of a number without using exponential notation and with exactly digits digits after the decimal point"[^4]. It rounds the stored value: MDN shows that `(2.55).toFixed(1)` gives `'2.5'`, because the "closest representable float is lower"[^4]. For 1e21 and more, it uses exponential notation.
- **TypeScript.** The same as JavaScript.
- **Java.** `String.format("%.2f", x)` does not round the stored value. Its documentation says that the value "will be rounded using the round half up algorithm"[^5], and it rounds the short decimal text of `Double.toString`. The solution uses `BigDecimal`: the constructor gives "the exact decimal representation of the double's binary floating-point value"[^6], and `setScale` rounds it.
- **C#.** The format `F` gives "Integral and decimal digits with optional negative sign"[^7]. The result "reflects a number that is rounded to a representable result nearest to the infinitely precise result"[^7]. The culture gives the decimal separator, so the solution passes the invariant culture.
- **Go.** With `%f`, "precision sets the number of places after the decimal"[^8]. `strconv.FormatFloat` with the format `'f'` takes the count of digits as an argument.
- **Rust.** For a float, the precision "indicates how many digits after the decimal point should be printed"[^9]. `.digits$` takes the count from a variable.
- **C.** `%.*f` takes the precision from an argument. "The precision specifies the number of decimal places, trailing zeros will be written if necessary"[^10]. The decimal point comes from the locale, and the default C locale gives a dot.
- **C++.** The type `f` "Produces the output as if by calling std::to_chars(first, last, value, std::chars_format::fixed, precision)"[^11]. `{:.{}f}` takes the precision from the next argument.
- **Bash.** The shell `printf` reads the number as a `long double`, which is not the same value as a `double` (see Pitfalls). The solution calls `awk`, which uses a `double`.

The Python solution, with the stored value of `2.675`:

```python run id=python-fixed
from decimal import Decimal


def fixed(x, digits):
    return f"{x:.{digits}f}"


print(fixed(3.14159, 2), fixed(1.0, 3), fixed(123.456, 0))
print(fixed(0.1, 20))
print(Decimal(2.675))
print(fixed(2.675, 2), round(2.675, 2))
print(fixed(0.125, 2), fixed(2.5, 0))
```

```output for=python-fixed
3.14 1.000 123
0.10000000000000000555
2.67499999999999982236431605997495353221893310546875
2.67 2.67
0.12 2
```

The last line shows a value that is exactly halfway: `0.125` and `2.5` are stored exactly. Python rounds a tie to the even digit.

## Edge cases

- **No decimals.** With `digits` 0, there is no decimal point: `fixed(123.456, 0)` is `123`.
- **A stored value under the half.** `2.675` and `1.005` give `2.67` and `1.00`, because the stored values are a little less than the written values.
- **Many digits.** `fixed(0.1, 20)` is `0.10000000000000000555`. The digits after the 17th show the binary value.
- **Large numbers.** `1e20` with 2 decimals is `100000000000000000000.00`, with no exponent. In JavaScript, `toFixed` uses an exponent from `1e21`.
- **Negative numbers.** The minus sign is first: `fixed(-1.234, 1)` is `-1.2`.
- **An integer value.** `42.0` with 2 decimals is `42.00`.

## Pitfalls

- **Java rounds the decimal text.** `String.format` rounds the short text of the number, so it does not give the same result as the other languages:

```java wrong id=java-format-half-up
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        System.out.println(String.format(Locale.ROOT, "%.2f", 2.675));
        System.out.println(String.format(Locale.ROOT, "%.2f", 1.005));
        System.out.println(String.format(Locale.ROOT, "%.20f", 0.1));
        System.out.println(new BigDecimal(2.675).setScale(2, RoundingMode.HALF_EVEN).toPlainString());
    }
}
```

```output for=java-format-half-up
2.68
1.01
0.10000000000000000000
2.67
```

Both results can be correct for a program, but a program that compares its output with a program in another language must use one rule.

- **The shell printf.** Bash reads the number as a `long double`, so the extra digits are not those of a `double`:

```bash wrong id=bash-printf-long-double
LC_NUMERIC=C printf '%.20f\n' 0.1
LC_ALL=C awk 'BEGIN { printf "%.20f\n", 0.1 }'
```

```output for=bash-printf-long-double
0.10000000000000000000
0.10000000000000000555
```

- **Ties are not the same in all languages.** When the stored value is exactly halfway, Python rounds to the even digit (see the Python solution), and JavaScript `toFixed` takes the larger value:

```javascript run id=javascript-tofixed-ties
console.log((0.125).toFixed(2), (2.5).toFixed(0), (-2.5).toFixed(0));
```

```output for=javascript-tofixed-ties
0.13 3 -3
```

- **Formatting is not rounding the value.** `fixed` returns text. To compute with a rounded value, round the number, or use a decimal type for money.
- **The locale.** C# `ToString("F2")`, Java `String.format` without a locale and C `printf` after `setlocale` use the decimal separator of the locale. In Germany, the result is `3,14`.

## Related tasks

- [[format-a-number-with-separators]]: the digits before the decimal point.
- [[print-formatted-output]]: a number in a column of fixed width.
- [[format-specifiers]]: the precision in a format string.

## References
