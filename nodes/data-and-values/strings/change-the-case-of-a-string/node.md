---
tag: G0M5JN
kind: task
slug: change-the-case-of-a-string
name: Change the case of a string
aliases: [uppercase, lowercase, to upper case, to lower case, toUpperCase, toLowerCase, upper, lower, capitalize, case conversion]
area: data-and-values/strings
level: intro
summary: Change a string to upper case or lower case, and know when the Unicode case change of the library changes the length or depends on the locale.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.upper
  title: The Python Standard Library, Built-in Types
  locator: str.upper, str.casefold
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/toUpperCase
  title: MDN Web Docs, String.prototype.toUpperCase()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.toupperinvariant?view=net-11.0
  title: .NET API reference, String.ToUpperInvariant method
  locator: Summary
- url: https://pkg.go.dev/strings#ToUpper
  title: Go standard library, package strings
  locator: ToUpper, ToUpperSpecial, EqualFold
- url: https://doc.rust-lang.org/std/primitive.str.html#method.to_uppercase
  title: Rust standard library, primitive type str
  locator: to_uppercase, to_ascii_uppercase
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/toLocaleUpperCase
  title: MDN Web Docs, String.prototype.toLocaleUpperCase()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: toLowerCase(), toUpperCase(Locale)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.toupper?view=net-11.0
  title: .NET API reference, String.ToUpper method
  locator: Remarks
- url: https://doc.rust-lang.org/std/primitive.char.html#method.to_uppercase
  title: Rust standard library, primitive type char
  locator: to_uppercase
- url: https://en.cppreference.com/w/c/string/byte/toupper
  title: cppreference.com, toupper (C)
  locator: Description
- url: https://en.cppreference.com/w/cpp/string/byte/toupper
  title: cppreference.com, std::toupper
  locator: Notes
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
---

## Task

Change the case of a string. The function `to_upper(s)` changes each ASCII letter from `a` to `z` to the letter from `A` to `Z`, and the function `to_lower(s)` does the opposite. Each other code point stays the same. For example, `to_upper("Hello, World!")` is `HELLO, WORLD!`, and `to_upper("café")` is `CAFé`.

This is the ASCII case change. A program uses it for keywords, identifiers, hex digits, protocol names and file extensions, where the result must be the same on each computer. For text that people read, use the Unicode case change of the library, and read the pitfalls. The concept page [[case-mapping-and-folding]] tells how the Unicode case change works.

## Solutions

| Language | Case change of the library | ASCII case change |
|---|---|---|
| Python | `s.upper()` | `s.translate(table)` with the 26 letters |
| JavaScript, TypeScript | `s.toUpperCase()` | `s.replace(/[a-z]+/g, ...)` |
| Java | `s.toUpperCase(Locale.ROOT)` | a loop over the `char` values |
| C# | `s.ToUpperInvariant()` | a loop in `string.Create` |
| Go | `strings.ToUpper(s)` | `strings.Map` with the 26 letters |
| Rust | `s.to_uppercase()` | `s.to_ascii_uppercase()` |
| C | `toupper` on each byte, in the current locale | a compare with `'a'` and `'z'` |
| C++ | `std::toupper` on each byte | a compare with `'a'` and `'z'` |
| Bash | `${s^^}` | `tr` with `LC_ALL=C` |

- **Python.** `str.upper` returns "a copy of the string with all the cased characters [4] converted to uppercase"[^1]. A table from `str.maketrans` changes only the 26 letters.
- **JavaScript and TypeScript.** `toUpperCase` "returns this string converted to uppercase"[^2]. The code uses it only on a run of ASCII letters.
- **Java.** The loop changes each `char` from `a` to `z`. A UTF-16 code unit of a code point above U+FFFF is never in this range, so the loop is correct for each string.
- **C#.** `ToUpperInvariant` returns "a copy of this String object converted to uppercase using the casing rules of the invariant culture"[^3]. The invariant culture changes the letters of Unicode too, so the code uses a loop.
- **Go.** "ToUpper returns s with all Unicode letters mapped to their upper case"[^4]. `strings.Map` calls a function for each rune.
- **Rust.** For `to_ascii_uppercase`, "ASCII letters ‘a’ to ‘z’ are mapped to ‘A’ to ‘Z’, but non-ASCII letters are unchanged"[^5].
- **C and C++.** The bytes of a UTF-8 sequence are 0x80 or more, so a compare of each byte with `'a'` and `'z'` changes only the ASCII letters. It does not use the locale.
- **Bash.** `tr` with `LC_ALL=C` changes the bytes, and it works in each version of Bash.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `to_lower` are in `cases/lower.jsonl`.

- `empty`: the empty string stays empty.
- `digits-punct`: digits, `-` and `_` do not change.
- `non-ascii`, `sharp-s`, `greek` and `fullwidth`: `é`, `ß`, `σ` and the fullwidth `ａ` (U+FF41) do not change. The Unicode case change of the library changes each of them.
- `dotless-i` and `lower-dotted-i`: the dotless `ı` (U+0131) and the capital `İ` (U+0130) do not change.
- `lower-kelvin`: the KELVIN SIGN (U+212A) looks like `K`, but it is not an ASCII letter.
- `astral`: an emoji between two letters stays the same.
- `combining`: `cafe` followed by U+0301 COMBINING ACUTE ACCENT changes to `CAFE` followed by U+0301.

## Pitfalls

- **The length can change.** Python uses "section 3.13.2 ‘Default Case Conversion’ of the Unicode Standard"[^1], and `ß` changes to `SS`. The lower case of `İ` is 2 code points. To compare strings with no case, use `casefold`, not `lower`:

```python run id=python-upper-length
s = "straße"
print(s.upper(), len(s), len(s.upper()))
print(len("İ".lower()))
print("Straße".casefold() == "STRASSE".casefold())
```

```output for=python-upper-length
STRASSE 6 7
2
True
```

- **JavaScript toLocaleUpperCase uses a locale.** It changes the string "according to any locale-specific case mappings"[^6], and in Turkish the capital of `i` is `İ` (U+0130):

```javascript run id=javascript-turkish-i
console.log("straße".toUpperCase());
console.log("i".toUpperCase());
console.log("i".toLocaleUpperCase("tr").codePointAt(0).toString(16));
```

```output for=javascript-turkish-i
STRASSE
I
130
```

- **Java toLowerCase() uses the default locale.** "This method is locale sensitive, and may produce unexpected results if used for strings that are intended to be interpreted locale independently"[^7]. In the Turkish locale, `TITLE` changes to `tıtle` with U+0131. Use `toLowerCase(Locale.ROOT)`. Also, "the resulting String and this String may differ in length"[^7]:

```java run id=java-locale-root
import java.util.Locale;

public class Main {
  public static void main(String[] args) {
    Locale tr = Locale.forLanguageTag("tr");
    System.out.println("TITLE".toLowerCase(Locale.ROOT));
    System.out.println(Integer.toHexString("TITLE".toLowerCase(tr).charAt(1)));
    System.out.println("straße".toUpperCase(Locale.ROOT).length());
  }
}
```

```output for=java-locale-root
title
131
7
```

- **C# ToUpper uses the current culture.** For a name of the system, "such as a file name, named pipe, or registry key, use the ToLowerInvariant or ToUpperInvariant method. This produces the same result in every culture and performs more efficiently"[^8]. The output shows that `ToUpperInvariant` changes `é` to `É` (U+00C9), and that it keeps the length of `straße`:

```csharp run id=csharp-culture-upper
using System;
using System.Globalization;

var tr = new CultureInfo("tr-TR");
Console.WriteLine("i".ToUpperInvariant());
Console.WriteLine(((int)"i".ToUpper(tr)[0]).ToString("X4"));
Console.WriteLine(((int)"é".ToUpperInvariant()[0]).ToString("X4"));
Console.WriteLine("straße".ToUpperInvariant().Length);
```

```output for=csharp-culture-upper
I
0130
00C9
6
```

- **Go has a special case for Turkish.** `ToUpperSpecial` changes the letters "using the case mapping specified by c"[^4]. For a compare with no case, use `EqualFold`, which compares "under simple Unicode case-folding"[^4]:

```go run id=go-turkish-case
package main

import (
	"fmt"
	"strings"
	"unicode"
)

func main() {
	fmt.Println(strings.ToUpper("istanbul"))
	fmt.Printf("%U\n", []rune(strings.ToUpperSpecial(unicode.TurkishCase, "i")))
	fmt.Println(strings.EqualFold("Go", "GO"))
}
```

```output for=go-turkish-case
ISTANBUL
[U+0130]
true
```

- **Rust to_uppercase returns a new String.** "Since some characters can expand into multiple characters when changing the case, this function returns a String instead of modifying the parameter in-place"[^5]. `char::to_uppercase` returns "an iterator that yields the uppercase mapping of this char as one or more chars"[^9]:

```rust run id=rust-upper-expand
fn main() {
    let s = "stra\u{df}e";
    println!("{}", s.to_uppercase());
    println!("{}", s.to_ascii_uppercase().chars().count());
    println!("{}", '\u{df}'.to_uppercase().count());
}
```

```output for=rust-upper-expand
STRASSE
6
2
```

- **C toupper takes one byte.** In the C locale, "the following lowercase letters abcdefghijklmnopqrstuvwxyz are replaced with respective uppercase letters ABCDEFGHIJKLMNOPQRSTUVWXYZ"[^10]. It cannot change `é`, which is 2 bytes in UTF-8. Cast each `char` to `unsigned char` first:

```c run id=c-toupper-bytes
#include <ctype.h>
#include <stdio.h>

int main(void) {
    char s[] = "caf\xc3\xa9";
    for (char *p = s; *p != '\0'; p++) {
        *p = (char)toupper((unsigned char)*p);
    }
    for (char *p = s; *p != '\0'; p++) {
        printf(p == s ? "%02x" : " %02x", (unsigned char)*p);
    }
    printf("\n");
    return 0;
}
```

```output for=c-toupper-bytes
43 41 46 c3 a9
```

- **C++ std::toupper with a negative char.** cppreference says that "the behavior of std::toupper is undefined if the argument's value is neither representable as unsigned char nor equal to EOF"[^11]. With `std::transform`, use a lambda that takes an `unsigned char`:

```cpp run id=cpp-transform-toupper
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello, World";
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    std::cout << s << "\n";
}
```

```output for=cpp-transform-toupper
HELLO, WORLD
```

- **Bash has case expansions.** "This expansion modifies the case of alphabetic characters in parameter"[^12]. `${s^^}` changes each letter, `${s^}` changes the first letter and `${s,,}` changes each letter to lower case. A script for many shells can use `tr`:

```bash run id=bash-case-expansion
s='hello, world'
echo "${s^^}"
echo "${s^}"
t='HELLO'
echo "${t,,}"
printf '%s\n' "$s" | LC_ALL=C tr 'a-z' 'A-Z'
```

```output for=bash-case-expansion
HELLO, WORLD
Hello, world
hello
HELLO, WORLD
```

## Related tasks

- [[compare-strings]]: compare two strings after a change of case.
- [[check-whether-a-string-is-empty]]: another check that a program does on text from a user.
- [[normalize-unicode-text]]: change the two forms of `é` to the same code points before a case change.
- [[trim-whitespace]]: remove the white space before a compare with no case.

## References
