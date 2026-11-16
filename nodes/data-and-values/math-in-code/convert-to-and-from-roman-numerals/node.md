---
tag: ATFXNX
kind: task
slug: convert-to-and-from-roman-numerals
name: Convert to and from Roman numerals
aliases: [roman numerals, to roman, from roman, roman numeral encode, roman numeral decode, parse roman numerals, subtractive notation]
area: data-and-values/math-in-code
level: intro
summary: >-
  Convert an integer from 1 to 3999 to a Roman numeral with a table of values, and parse a Roman numeral back. A strict parser rejects forms such as IIII and IC.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Roman-numerals-Encode/00-TASK.txt
  title: Rosetta Code, Roman numerals/Encode (task text, from acmeism/RosettaCodeData)
  locator: task text
- url: https://en.wikipedia.org/wiki/Roman_numerals
  title: Wikipedia, Roman numerals
  locator: Description; Other additive forms; Zero
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Roman-numerals-Decode/00-TASK.txt
  title: Rosetta Code, Roman numerals/Decode (task text, from acmeism/RosettaCodeData)
  locator: task text
---

## Task

Convert a positive integer to a Roman numeral, and convert a Roman numeral back to an integer. The Rosetta Code task says: "Modern Roman numerals are written by expressing each digit separately, starting with the left most digit and skipping any digit with a value of zero"[^1]. For example, 1990 is M (1000), CM (900) and XC (90), so it is MCMXC.

The seven symbols are I (1), V (5), X (10), L (50), C (100), D (500) and M (1000). Six pairs use subtractive notation: "The numerals for 4 (IV) and 9 (IX) are written using subtractive notation"[^2], and "Subtractive notation is also used for 40 (XL), 90 (XC), 400 (CD) and 900 (CM)"[^2].

The function `to_roman(n)` returns the Roman numeral of `n` as a string. For example, `to_roman(1994)` is `"MCMXCIV"`. The function `from_roman(s)` returns the integer, or an error if `s` is not a valid numeral in the standard form.

The range is 1 to 3999. There is no symbol for 0: "the system as used by the Romans lacked a numeral for the number zero itself"[^2]. And "The largest number that can be represented in this manner is 3,999 (MMMCMXCIX)"[^2], because the standard form uses no more than three M symbols.

The conversion to a numeral uses a table of the 13 values and their symbols, from the largest to the smallest: 1000 M, 900 CM, 500 D, 400 CD, 100 C, 90 XC, 50 L, 40 XL, 10 X, 9 IX, 5 V, 4 IV and 1 I. For each row, while `n` is at least the value, append the symbol and subtract the value. The table has the six subtractive pairs, so the greedy loop never writes IIII.

The conversion back reads the symbols from left to right. If a symbol is smaller than the next symbol, subtract its value. Otherwise, add its value. So in XC, the X is subtracted and the C is added: 100 minus 10 is 90.

## Solutions

The standard libraries of the tier 1 languages have no Roman numeral functions, so each solution uses the table.

| Language | Table | Errors |
|---|---|---|
| Python | a tuple of pairs | `ValueError` |
| JavaScript, TypeScript | an array of pairs | `RangeError` and `SyntaxError` |
| Java | two arrays | `IllegalArgumentException` |
| C# | an array of tuples | `ArgumentOutOfRangeException` and `FormatException` |
| Go | a slice of structs | `error` values |
| Rust | an array of tuples | `Option` |
| C | an array of structs | a return value of -1 |
| C++ | a `constexpr` array of pairs | `std::out_of_range` and `std::optional` |
| Bash | two arrays | the exit status 1 |

- **Python.** The table is a tuple of `(value, symbol)` pairs. `from_roman` raises `ValueError` for an invalid numeral.
- **JavaScript.** The table is an array of pairs. `toRoman` throws `RangeError` for a number out of the range, and `fromRoman` throws `SyntaxError` for an invalid numeral.
- **TypeScript.** The same code as JavaScript, with types.
- **Java.** The table is two arrays, one of values and one of symbols. The functions throw `IllegalArgumentException` for bad input.
- **C#.** The table is an array of tuples. `ToRoman` throws `ArgumentOutOfRangeException`, and `FromRoman` throws `FormatException`.
- **Go.** The table is a slice of structs. Both functions return an `error` value as the second result.
- **Rust.** The table is an array of tuples. Both functions return `None` for bad input.
- **C.** `to_roman` writes the numeral into a buffer of 16 bytes, which holds the longest numeral, MMMDCCCLXXXVIII, and the null byte. Both functions return -1 for bad input.
- **C++.** The table is a `constexpr` array. `to_roman` throws `std::out_of_range`, and `from_roman` returns an empty `std::optional` for an invalid numeral.
- **Bash.** The table is two arrays. Both functions print the result, and return the exit status 1 for bad input.

The conversion to a numeral in Python:

```python run id=python-to-roman
TABLE = (
    (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
    (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
    (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I"),
)


def to_roman(n):
    if not 1 <= n <= 3999:
        raise ValueError("n is out of range")
    parts = []
    for value, symbol in TABLE:
        while n >= value:
            parts.append(symbol)
            n -= value
    return "".join(parts)


print(to_roman(1990), to_roman(2008), to_roman(1666), to_roman(3999))
```

```output for=python-to-roman
MCMXC MMVIII MDCLXVI MMMCMXCIX
```

The conversion back, with a check. The loop accepts some strings that are not in the standard form, so the function converts the result to a numeral again and compares it with the input:

```python run id=python-from-roman
VALUES = {"I": 1, "V": 5, "X": 10, "L": 50, "C": 100, "D": 500, "M": 1000}
TABLE = (
    (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
    (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
    (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I"),
)


def to_roman(n):
    parts = []
    for value, symbol in TABLE:
        while n >= value:
            parts.append(symbol)
            n -= value
    return "".join(parts)


def from_roman(s):
    if not s or any(c not in VALUES for c in s):
        raise ValueError("not a Roman numeral")
    total = 0
    for i, c in enumerate(s):
        value = VALUES[c]
        if i + 1 < len(s) and value < VALUES[s[i + 1]]:
            total -= value
        else:
            total += value
    if total > 3999 or to_roman(total) != s:
        raise ValueError("not in the standard form")
    return total


print(from_roman("MCMXC"), from_roman("MMVIII"), from_roman("XCIX"))
for s in ["IC", "IIII", "MMMM", ""]:
    try:
        from_roman(s)
    except ValueError as e:
        print(repr(s), e)
```

```output for=python-from-roman
1990 2008 99
'IC' not in the standard form
'IIII' not in the standard form
'MMMM' not in the standard form
'' not a Roman numeral
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. They are for `to_roman`. The tests of each solution also convert each number from 1 to 3999 to a numeral and back, and check that `from_roman` rejects invalid strings.

- `year-1990`, `year-2008` and `each-symbol-once`: the examples of the Rosetta Code task. 1666 "uses each Roman symbol in descending order: MDCLXVI"[^1].
- `one`: 1 is I, the smallest numeral.
- `four` and `nine`: IV and IX, the subtractive pairs.
- `forty-nine` and `ninety-nine`: XLIX and XCIX. Each digit is written on its own, so 49 is not IL, and 99 is not IC.
- `four-hundred-forty-four` and `year-1994`: numbers with three subtractive pairs.
- `longest`: 3888 is MMMDCCCLXXXVIII, with 15 symbols, the longest numeral in the range.
- `largest`: 3999 is MMMCMXCIX.

## Pitfalls

- **A table without the subtractive pairs.** A greedy loop with only the seven symbols writes the additive forms, such as IIII for 4. These forms are in old texts, but "subtractive notation for 4, 40, and 400 (IV, XL, and CD) is the modern standard"[^2]:

```python wrong id=python-additive-table
TABLE = ((1000, "M"), (500, "D"), (100, "C"), (50, "L"), (10, "X"), (5, "V"), (1, "I"))


def to_roman(n):
    parts = []
    for value, symbol in TABLE:
        while n >= value:
            parts.append(symbol)
            n -= value
    return "".join(parts)


print(to_roman(1994))
```

```output for=python-additive-table
MDCCCCLXXXXIIII
```

- **The parser accepts invalid numerals.** The Rosetta Code task for the conversion back says: "You don't need to validate the form of the Roman numeral"[^3]. A parser with no check gives a value for strings that are not valid numerals:

```javascript wrong id=javascript-lenient-parse
const values = { I: 1, V: 5, X: 10, L: 50, C: 100, D: 500, M: 1000 };

function fromRoman(s) {
  let total = 0;
  for (let i = 0; i < s.length; i++) {
    const value = values[s[i]];
    if (i + 1 < s.length && value < values[s[i + 1]]) {
      total -= value;
    } else {
      total += value;
    }
  }
  return total;
}

console.log(fromRoman("IIII"), fromRoman("IC"), fromRoman("VX"), fromRoman("I2"));
```

```output for=javascript-lenient-parse
4 99 5 NaN
```

  To reject them, convert the result to a numeral again, and compare it with the input. Also reject the empty string and each character that is not a symbol.

- **Zero and negative numbers.** There is no Roman numeral for 0. The greedy loop returns an empty string for 0 and for each negative number, with no error. Check the range first:

```go wrong id=go-roman-zero
package main

import "fmt"

var values = []int{1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1}
var symbols = []string{"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}

func toRoman(n int) string {
	s := ""
	for i, v := range values {
		for n >= v {
			s += symbols[i]
			n -= v
		}
	}
	return s
}

func main() {
	fmt.Printf("%q %q %q\n", toRoman(7), toRoman(0), toRoman(-5))
}
```

```output for=go-roman-zero
"VII" "" ""
```

- **Lowercase letters.** Some texts write Roman numerals in lowercase, such as xiv. The solutions on this page accept only uppercase letters. To accept lowercase letters, convert the input to uppercase first.

## Related tasks

- [[compute-the-greatest-common-divisor]]: another small task on integers.
- [[check-whether-a-number-is-prime]]: another small task on integers.

## References
