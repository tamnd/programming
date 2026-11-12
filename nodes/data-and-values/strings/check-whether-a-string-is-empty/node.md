---
tag: 0WBE1M
kind: task
slug: check-whether-a-string-is-empty
name: Check whether a string is empty
aliases: [empty string, is empty, isEmpty, blank string, isBlank, IsNullOrEmpty, IsNullOrWhiteSpace, string has only white space]
area: data-and-values/strings
level: intro
summary: Check whether a string has no characters, check whether it has only white space, and know which values each language also treats as empty and which characters it calls white space.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#truth
  title: The Python Standard Library, Built-in Types
  locator: Truth Value Testing; str.isspace
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: isEmpty(); isBlank()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.isnullorwhitespace?view=net-10.0
  title: .NET API reference, String.IsNullOrWhiteSpace method
  locator: Remarks
- url: https://doc.rust-lang.org/std/primitive.str.html#method.is_empty
  title: The Rust Standard Library, primitive type str
  locator: is_empty, trim
- url: https://en.cppreference.com/w/cpp/string/basic_string/empty
  title: cppreference.com, std::basic_string empty
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Bash-Conditional-Expressions
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Bash Conditional Expressions; Bourne Shell Builtins, test
- url: https://developer.mozilla.org/en-US/docs/Glossary/Falsy
  title: MDN Web Docs Glossary, Falsy
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/trim
  title: MDN Web Docs, String.prototype.trim()
  locator: Return value
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Character.html
  title: Java SE 25 API, java.lang.Character
  locator: isWhitespace(char)
- url: https://pkg.go.dev/strings#TrimSpace
  title: Go standard library, package strings
  locator: TrimSpace
- url: https://pkg.go.dev/unicode#IsSpace
  title: Go standard library, package unicode
  locator: IsSpace
- url: https://doc.rust-lang.org/std/primitive.char.html#method.is_ascii_whitespace
  title: The Rust Standard Library, primitive type char
  locator: is_ascii_whitespace
- url: https://en.cppreference.com/w/c/string/byte/isspace
  title: cppreference.com, isspace
---

## Task

Check whether a string is empty. The function `is_empty(s)` returns true if `s` has no code points. The function `is_blank(s)` returns true if each code point of `s` is ASCII white space: space, tab, line feed, vertical tab, form feed or carriage return. Both functions return true for the empty string. For example, `is_empty(" ")` is false, and `is_blank(" ")` is true.

A string of invisible characters, such as U+200B ZERO WIDTH SPACE, is not empty, and it is not blank. The languages do not agree on which code points are white space, so the contract uses only the six ASCII code points.

## Solutions

| Language | Empty | Blank in the contract | Blank in the library |
|---|---|---|---|
| Python | `len(s) == 0` | `s.strip(" \t\n\v\f\r") == ""` | `not s or s.isspace()` |
| JavaScript, TypeScript | `s.length === 0` | `/^[ \t\n\v\f\r]*$/.test(s)` | `s.trim() === ""` |
| Java | `s.isEmpty()` | `allMatch` on `s.chars()` | `s.isBlank()` |
| C# | `s.Length == 0` | `s.AsSpan().Trim(...).IsEmpty` | `string.IsNullOrWhiteSpace(s)` |
| Go | `s == ""` | `strings.Trim(s, ...) == ""` | `strings.TrimSpace(s) == ""` |
| Rust | `s.is_empty()` | `all` on `s.chars()` | `s.trim().is_empty()` |
| C | `s[0] == '\0'` | `s[strspn(s, ...)] == '\0'` | a loop with `isspace` |
| C++ | `s.empty()` | `s.find_first_not_of(...) == npos` | none |
| Bash | `[[ -z $s ]]` | `[[ -z ${s//[$' \t\n\v\f\r']/} ]]` | `[[ $s != *[![:space:]]* ]]` |

- **Python.** The length of an empty string is 0. In a condition, "empty sequences and collections"[^1] are false, so `not s` is also a test. `str.isspace` returns true "if there are only whitespace characters in the string and there is at least one character"[^1].
- **JavaScript and TypeScript.** `length` counts UTF-16 code units, and it is 0 only for the empty string. The code tests the blank string with a character class, because `trim` removes more than the ASCII white space.
- **Java.** `isEmpty` "Returns true if, and only if, length() is 0"[^2]. `isBlank` "Returns true if the string is empty or contains only white space codepoints"[^2], with the Java rules for white space.
- **C#.** `Length` is 0 only for the empty string. `IsNullOrWhiteSpace` "Indicates whether a specified string is null, empty, or consists only of white-space characters"[^3].
- **Go.** A Go string cannot be `nil`, and the zero value of a string is `""`. So `s == ""` and `len(s) == 0` are the same test.
- **Rust.** `is_empty` "Returns true if self has a length of zero bytes"[^4]. The code lists the six characters for `is_blank`.
- **C.** A C string ends at its first null character, so the string is empty when its first byte is `'\0'`. `strspn` gives the length of the first part of `s` that has only white space, and the string is blank when that part ends at the null character.
- **C++.** `empty` "Checks if the string has no characters"[^5]. `find_first_not_of` returns `npos` when no character is outside the set.
- **Bash.** In a conditional expression, `-z string` is "True if the length of string is zero"[^6]. The expansion deletes each ASCII white space character, and the test checks that nothing is left.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `is_blank` are in `cases/blank.jsonl`.

- `empty` and `blank-empty`: the empty string is empty, and it is also blank.
- `space`, `newline` and `nbsp`: white space is not empty.
- `blank-mixed` and `blank-vt-ff`: each of the six ASCII white space characters is blank, also the vertical tab U+000B and the form feed U+000C.
- `blank-nbsp` and `blank-ideographic`: U+00A0 NO-BREAK SPACE and U+3000 IDEOGRAPHIC SPACE are not ASCII, so they are not blank in the contract. Most library functions call them white space.
- `zero-width` and `blank-zero-width`: U+200B is not empty, and it is not white space in any of the libraries.
- `combining`, `astral` and `cjk`: a single combining mark, an emoji or a CJK character is not empty.

## Pitfalls

- **Python not s is also true for None.** `not s` is true for `None`, for `0` and for each empty collection, not only for the empty string. `''.isspace()` is false, so a blank test with `isspace` must also check for the empty string. `isspace` accepts U+00A0, but not U+200B:

```python run id=python-not-s-none
for s in ["", None, " "]:
    print(repr(s), not s)
print("".isspace(), "\u00a0".isspace(), "\u200b".isspace())
```

```output for=python-not-s-none
'' True
None True
' ' False
False True False
```

- **JavaScript !s is true for more than the empty string.** A falsy value "is considered false when encountered in a Boolean context"[^7], and the falsy values include `0`, `null` and `undefined`. `trim` removes Unicode spaces, because "Whitespace is defined as white space characters plus line terminators"[^8], but it does not remove U+200B:

```javascript run id=javascript-falsy-values
for (const v of ["", 0, null, " "]) {
  console.log(JSON.stringify(v), !v);
}
console.log("\u00a0\u3000".trim() === "");
console.log("\u200b".trim().length);
```

```output for=javascript-falsy-values
"" true
0 true
null true
" " false
true
1
```

- **Java isBlank does not accept the no-break space.** A Java white space character is "a Unicode space character (SPACE_SEPARATOR, LINE_SEPARATOR, or PARAGRAPH_SEPARATOR) but is not also a non-breaking space"[^9]. So U+2003 EM SPACE is blank, and U+00A0 is not. A call on `null` throws a `NullPointerException`:

```java run id=java-isblank-nbsp
public class Main {
  public static void main(String[] args) {
    System.out.println("\u2003".isBlank());
    System.out.println("\u00a0".isBlank());
    String s = null;
    try {
      System.out.println(s.isEmpty());
    } catch (NullPointerException e) {
      System.out.println("NullPointerException");
    }
  }
}
```

```output for=java-isblank-nbsp
true
false
NullPointerException
```

- **C# IsNullOrWhiteSpace uses Unicode.** "White-space characters are defined by the Unicode standard"[^3], so U+00A0 and U+3000 are white space, and U+200B is not. `s.Length` on a `null` string throws, so test for `null` first, or use `s?.Length == 0`, which is false for `null`:

```csharp run id=csharp-isnullorwhitespace
using System;

string? s = null;
Console.WriteLine(string.IsNullOrEmpty(s));
Console.WriteLine(string.IsNullOrWhiteSpace("\u00a0\u3000"));
Console.WriteLine(string.IsNullOrWhiteSpace("\u200b"));
Console.WriteLine(s?.Length == 0);
```

```output for=csharp-isnullorwhitespace
True
True
False
False
```

- **Go TrimSpace uses Unicode.** `TrimSpace` removes white space "as defined by Unicode"[^10]. `unicode.IsSpace` follows "Unicode's White Space property"[^11], so U+00A0 and U+3000 are white space, and U+200B is not:

```go run id=go-trimspace-unicode
package main

import (
	"fmt"
	"strings"
	"unicode"
)

func main() {
	var s string
	fmt.Println(s == "", len(s))
	fmt.Println(strings.TrimSpace("\u00a0\u3000") == "")
	fmt.Println(unicode.IsSpace('\u200b'))
}
```

```output for=go-trimspace-unicode
true 0
true
false
```

- **Rust is_ascii_whitespace does not accept the vertical tab.** "Because the list above excludes U+000B VERTICAL TAB, c.is_ascii_whitespace() is not equivalent to c.is_ascii() && c.is_whitespace()"[^12]. A value that can be missing is an `Option`, so test it with `is_none_or`:

```rust run id=rust-ascii-whitespace-vt
fn main() {
    println!("{}", "\u{a0}\u{3000}".trim().is_empty());
    println!("{}", '\x0B'.is_ascii_whitespace());
    println!("{}", '\x0B'.is_whitespace());
    let name: Option<&str> = None;
    println!("{}", name.is_none_or(str::is_empty));
}
```

```output for=rust-ascii-whitespace-vt
true
false
true
true
```

- **C isspace needs an unsigned char.** "The behavior is undefined if the value of ch is not representable as unsigned char and is not equal to EOF"[^13]. A byte such as 0xC2 is negative in a signed `char`, so cast it first. A `NULL` pointer is not an empty string, and the code must test the pointer before it reads the first byte:

```c run id=c-isspace-unsigned
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

// The cast to unsigned char keeps a byte such as 0xC2 from being negative.
static bool is_blank_locale(const char *s) {
    for (; *s != '\0'; s++) {
        if (!isspace((unsigned char)*s)) {
            return false;
        }
    }
    return true;
}

int main(void) {
    const char *p = NULL;
    // Test the pointer before the first byte.
    printf("%d\n", p == NULL || p[0] == '\0');
    printf("%d %d\n", is_blank_locale(" \t"), is_blank_locale("\xc2\xa0"));
    return 0;
}
```

```output for=c-isspace-unsigned
1
1 0
```

- **C++ empty does not clear the string.** `empty()` only returns a result. `clear()` removes the characters:

```cpp run id=cpp-empty-not-clear
#include <iostream>
#include <string>

int main() {
    std::string s = "text";
    // empty() only reads the string. clear() is the call that removes the text.
    std::cout << s.empty() << " " << s.size() << "\n";
    s.clear();
    std::cout << s.empty() << " " << s.size() << "\n";
}
```

```output for=cpp-empty-not-clear
0 4
1 0
```

- **Bash test with an unquoted variable.** In `[ -n $s ]`, an empty `s` disappears, and `[` sees one argument. With one argument, "The expression is true if, and only if, the argument is not null"[^6], so the test is true for the empty string. A value with a space gives two arguments, and `[` reports an error. Quote the variable, or use `[[`:

```bash run id=bash-test-unquoted
s=''
[ -n $s ] && echo "unquoted -n: true"
[ -n "$s" ] || echo "quoted -n: false"
s='a b'
[ -z "$s" ] || echo "quoted -z: false"
[ -z $s ] 2>/dev/null || echo "unquoted -z: error or false"
```

```output for=bash-test-unquoted
unquoted -n: true
quoted -n: false
quoted -z: false
unquoted -z: error or false
```

## Related tasks

- [[trim-whitespace]]: remove the white space at the ends of a string.
- [[find-the-length-of-a-string]]: count the code units, code points or bytes of a string.
- [[compare-strings]]: compare a string with the empty string or with another string.

## References
