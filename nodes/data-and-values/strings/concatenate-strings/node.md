---
tag: V1CQDW
kind: task
slug: concatenate-strings
name: Concatenate strings
aliases: [string concatenation, join two strings, append a string, add strings, string plus operator, concat]
area: data-and-values/strings
level: intro
summary: Join two strings into one with the operator or function of each language, and know the rules when one operand is a number or null and the cost of repeated concatenation.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 6
- url: https://docs.python.org/3/reference/lexical_analysis.html#f-strings
  title: The Python Language Reference, Lexical analysis
  locator: f-strings
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Addition
  title: MDN Web Docs, Addition (+)
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/concat
  title: MDN Web Docs, String.prototype.concat()
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
  title: MDN Web Docs, Template literals (Template strings)
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.18.1
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15, Expressions
  locator: 15.18, Additive Operators; 15.18.1, String Concatenation Operator +
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/addition-operator
  title: C# language reference, Addition operators + and +=
  locator: String concatenation
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: String concatenation; Conversions to and from a string type
- url: https://pkg.go.dev/strconv#Itoa
  title: Go standard library, package strconv
  locator: Itoa
- url: https://doc.rust-lang.org/std/string/struct.String.html
  title: The Rust Standard Library, struct String
  locator: impl Add<&str> for String
- url: https://doc.rust-lang.org/std/macro.format.html
  title: The Rust Standard Library, macro format
- url: https://en.cppreference.com/w/c/string/byte/strcat
  title: cppreference.com, strcat, strcat_s
- url: https://en.cppreference.com/w/cpp/string/basic_string/operator_plus
  title: cppreference.com, std::operator+(std::basic_string)
- url: https://en.cppreference.com/w/cpp/utility/format/format
  title: cppreference.com, std::format
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: Class description; concat(String)
- url: https://en.cppreference.com/w/c/io/fprintf
  title: cppreference.com, printf, fprintf, sprintf, snprintf
  locator: Return value
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameters
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Shell Parameters
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/strings/common-tasks/concatenate
  title: C# fundamentals, How to concatenate multiple strings
---

## Task

Concatenate two strings: make one string that holds the characters of `a` and then the characters of `b`, with nothing between them. The function `concat(a, b)` returns the result. A second function, `concat_number(s, n)`, puts the decimal form of a 32-bit integer after a string, because each language has its own rule for a string and a number.

Concatenation does not change the two strings. In most languages a string is immutable, so the result is a new string. The cost of one concatenation is the sum of the two lengths, and a loop of concatenations can cost much more. The task "Build a string efficiently" covers that loop.

## Solutions

| Language | Two strings | A string and a number |
|---|---|---|
| Python | `a + b` | `f"{s}{n}"` or `s + str(n)` |
| JavaScript, TypeScript | `a + b` | `` `${s}${n}` `` |
| Java | `a + b` | `s + n` |
| C# | `a + b` | `s + n.ToString(CultureInfo.InvariantCulture)` |
| Go | `a + b` | `s + strconv.Itoa(n)` |
| Rust | `a.to_owned() + b` | `format!("{s}{n}")` |
| C | `malloc` and `memcpy` | `snprintf` |
| C++ | `std::string` and `+=` | `std::format("{}{}", s, n)` |
| Bash | `"$a$b"` | `"$s$n"` |

- **Python.** `+` joins two `str` values. "Concatenating immutable sequences always results in a new object"[^1]. Python does not convert a number for `+`, so the code uses a "formatted string literal or f-string"[^2].
- **JavaScript and TypeScript.** "The addition (+) operator produces the sum of numeric operands or string concatenation"[^3]. `concat` "concatenates the string arguments to the calling string and returns a new string"[^4]. The code uses a template literal for the number, because a template literal makes the conversion clear and has "string interpolation with embedded expressions"[^5].
- **Java.** "If only one operand expression is of type String, then string conversion (§5.1.11) is performed on the other operand to produce a string at run time"[^6], so `s + n` gives the digits of `n`. The result "is newly created (§12.5) unless the expression is a constant expression (§15.29)"[^6].
- **C#.** "When one or both operands are of type string, the + operator concatenates the string representations of its operands"[^7]. The code calls `ToString` with `CultureInfo.InvariantCulture`, so the culture of the machine cannot change the digits.
- **Go.** "Strings can be concatenated using the + operator or the += assignment operator"[^8]. Go has no implicit conversion, so the code calls `strconv.Itoa`, which "is equivalent to FormatInt(int64(i), 10)"[^9].
- **Rust.** `String + &str` "consumes the String on the left-hand side and re-uses its buffer (growing it if necessary)"[^10], and "The string on the right-hand side is only borrowed"[^10]. The function receives two `&str` values, so it makes an owned copy of `a` first. `format!` "Creates a String using interpolation of runtime expressions"[^11].
- **C.** A C string is an array of bytes that ends with a null byte, and the language has no operator to join two of them. `strcat` "Appends a copy of the null-terminated byte string pointed to by src to the end of the null-terminated byte string pointed to by dest"[^12], but the caller must supply a destination that is large enough. The code allocates the sum of the two lengths plus one byte, and copies each part with `memcpy`. The caller frees the result. For the number, `snprintf` with a size of 0 returns the length of the text, and a second call writes it.
- **C++.** `operator+` "Returns a string containing characters from lhs followed by the characters from rhs"[^13]. The code takes two `std::string_view` values, reserves the sum of the two sizes and appends each part with `+=`. `std::format` writes the number[^14].
- **Bash.** Two expansions side by side in one word make one string, so `"$1$2"` is the concatenation. A Bash value is a string, so a number needs no conversion. `printf '%d'` checks that the argument is an integer.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `concat_number` are in `cases/number.jsonl`.

- `empty-both`, `empty-left` and `empty-right`: the empty string is the identity of concatenation. The result is the other operand.
- `non-ascii` and `astral`: the bytes of `ï` and `🙂` do not change. In every unit, the length of the result is the sum of the two lengths.
- `combining-mark`: `"e"` and the combining acute accent U+0301 give `"é"`, which a reader sees as one character. The result has the same code points as the operands, but the graphemes at the boundary can join, so the number of graphemes is not always the sum.
- `flag-halves`: the two regional indicators `🇫` and `🇷` give the flag `🇫🇷`. Two strings of one grapheme each make one grapheme.
- `digit-strings`: `"1"` and `"2"` give `"12"`, not `3`. The operands are strings, so no language adds them.
- `int32-min` and `negative`: the result has the minus sign U+002D and no space. `int32-min` is `-2147483648`, which has no positive 32-bit form, so code that writes the sign and then the digits of `-n` fails for it.

## Pitfalls

- **JavaScript `+` with a string and a number.** "If one side is a string, the other operand is also converted to a string"[^3], and `+` groups from the left. So `'1' + 2 + 3` is `'123'`, but `1 + 2 + '3'` is `'33'`. `-` has no string form, so `'5' - 2` is `3`:

```javascript run id=javascript-plus-order
console.log("1" + 2 + 3);
console.log(1 + 2 + "3");
console.log("5" - 2);
console.log(`${1 + 2}3`);
```

```output for=javascript-plus-order
123
33
3
33
```

- **Java `+` groups from the left, and `null` becomes text.** The "additive operators have the same precedence and are syntactically left-associative (they group left-to-right)"[^6], so Java gives the same results as JavaScript. String conversion turns a `null` reference into the text `null`. `concat` does not, because "passing a null argument to a constructor or method in this class will cause a NullPointerException to be thrown"[^15]:

```java run id=java-plus-and-null
public class Main {
  public static void main(String[] args) {
    System.out.println("1" + 2 + 3);
    System.out.println(1 + 2 + "3");
    String name = null;
    System.out.println("Hello, " + name);
    try {
      System.out.println("Hello, ".concat(name));
    } catch (NullPointerException e) {
      System.out.println("concat: " + e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-plus-and-null
123
33
Hello, null
concat: NullPointerException
```

- **C# `null` is an empty string.** In a concatenation, "the string representation of null is an empty string"[^7], so a missing name gives `"Hello, !"` and no error. `string.Concat` also skips a `null` argument:

```csharp run id=csharp-null-and-order
string? name = null;
Console.WriteLine("Hello, " + name + "!");
Console.WriteLine(1 + 2 + "3");
Console.WriteLine("1" + 2 + 3);
Console.WriteLine(string.Concat("a", null, "b"));
```

```output for=csharp-null-and-order
Hello, !
33
123
ab
```

- **Python does not convert a number for `+`.** `"items: " + 3` raises `TypeError`. Use `str(n)` or an f-string:

```python run id=python-plus-type-error
count = 3
try:
    print("items: " + count)
except TypeError as e:
    print(type(e).__name__)
print("items: " + str(count))
print(f"items: {count}")
```

```output for=python-plus-type-error
TypeError
items: 3
items: 3
```

- **Go `string(n)` makes a character, not digits.** The conversion of an integer to a string "yields a string containing the (possibly multi-byte) UTF-8 representation of the Unicode code point with the given integer value"[^8], so `string(rune(65))` is `"A"`. "The go vet tool flags certain integer-to-string conversions as potential errors"[^8]. Use `strconv.Itoa` or `fmt.Sprint`:

```go run id=go-string-of-int
package main

import (
	"fmt"
	"strconv"
)

func main() {
	n := 65
	fmt.Println("n=" + string(rune(n)))
	fmt.Println("n=" + strconv.Itoa(n))
	fmt.Println("n=" + fmt.Sprint(n))
}
```

```output for=go-string-of-int
n=A
n=65
n=65
```

- **C needs room for the result.** For `strcat`, "The behavior is undefined if the destination array is not large enough for the contents of both src and dest and the terminating null character"[^12]. `snprintf` never writes past its buffer. It returns the "number of characters (not including the terminating null character) which would have been written to buffer if bufsz was ignored"[^16], so a return value that is not less than the size tells you that the text was cut:

```c run id=c-snprintf-truncation
#include <stdio.h>

int main(void) {
    char buf[8];
    int n = snprintf(buf, sizeof buf, "%s%s", "Hello, ", "world");
    printf("%d [%s]\n", n, buf);
}
```

```output for=c-snprintf-truncation
12 [Hello, ]
```

- **C++ `+` on a literal and a number.** A string literal is an array of `char`, so `"abc" + 1` is a pointer to the second character, and it gives `"bc"`. `s += 65` appends the `char` with the value 65, so it appends `A`. Make the left operand a `std::string`, and convert a number with `std::format` or `std::to_string`:

```cpp run id=cpp-plus-on-pointers
#include <print>
#include <string>

int main() {
    std::string s = "n=";
    s += 65;
    std::println("{}", s);
    std::println("{}", "abc" + 1);
    std::println("{}", std::string("abc") + "1");
}
```

```output for=cpp-plus-on-pointers
n=A
bc
abc1
```

- **Bash `+=` depends on the integer attribute.** "When applied to a string-valued variable, value is expanded and appended to the variable’s value"[^17]. When "the integer attribute has been set", the two values are evaluated as arithmetic expressions and added[^17]. So `+=` gives `12` for a plain variable and `3` after `declare -i`:

```bash run id=bash-append-and-integer
total=1
total+=2
echo "$total"
declare -i sum=1
sum+=2
echo "$sum"
greeting="Hello, "
greeting+="world"
echo "$greeting"
```

```output for=bash-append-and-integer
12
3
Hello, world
```

- **Repeated concatenation in a loop.** Each concatenation of immutable strings copies both parts. In Python, "building up a sequence by repeated concatenation will have a quadratic runtime cost in the total sequence length"[^1], and the documentation says that "you can build a list and use str.join() at the end"[^1]. In C#, "Each + or += operation creates a new string, because strings are immutable"[^18]. Rust re-uses the buffer of the left `String` to avoid the copy, which otherwise "would lead to O(n^2) running time when building an n-byte string by repeated concatenation"[^10]. For a loop, use a builder or a join. The task Build a string efficiently shows them.

## Related tasks

- [[split-a-string]]: a split is the reverse of a join, which concatenates many strings with a separator between them.
- [[find-the-length-of-a-string]]: the length of a concatenation is the sum of the two lengths in bytes, code units and code points, but not always in graphemes.

## References
