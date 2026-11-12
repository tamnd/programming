---
tag: HYHEK1
kind: task
slug: trim-whitespace
name: Trim white space
aliases: [trim, strip, trim a string, remove leading and trailing spaces, lstrip, rstrip, trimStart, trimEnd, TrimSpace]
area: data-and-values/strings
level: intro
summary: Remove the white space at the start and at the end of a string, and know which characters each language counts as white space.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.strip
  title: The Python Standard Library, Built-in Types
  locator: str.strip, str.lstrip
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/trim
  title: MDN Web Docs, String.prototype.trim()
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.trim?view=net-10.0
  title: .NET API reference, String.Trim method
  locator: Overloads
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: trim(), strip()
- url: https://pkg.go.dev/strings#TrimSpace
  title: Go standard library, package strings
  locator: TrimSpace, TrimLeft
- url: https://doc.rust-lang.org/std/primitive.char.html#method.is_ascii_whitespace
  title: Rust standard library, primitive type char
  locator: is_ascii_whitespace
- url: https://doc.rust-lang.org/std/primitive.str.html#method.trim
  title: Rust standard library, primitive type str
  locator: trim
- url: https://en.cppreference.com/w/c/string/byte/isspace
  title: cppreference.com, isspace
  locator: Description
- url: https://en.cppreference.com/w/cpp/string/basic_string/find_first_not_of
  title: cppreference.com, std::basic_string::find_first_not_of
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion, 3.5.8.1 Pattern Matching
---

## Task

Remove the white space at the start and at the end of a string. The function `trim(s)` removes it from both ends, and the function `trim_start(s)` removes it only from the start. The white space between other characters stays. For example, `trim("  hello world\n")` is `hello world`.

The white space is the 6 ASCII white space characters: space, tab (U+0009), line feed (U+000A), vertical tab (U+000B), form feed (U+000C) and carriage return (U+000D). This is the set of the C locale, and the same set as `is_blank` in [[check-whether-a-string-is-empty]]. The trim of the library in most languages also removes the white space of Unicode, such as the no-break space U+00A0, so the solutions give the set.

## Solutions

| Language | Trim of the library | Trim of the 6 ASCII characters |
|---|---|---|
| Python | `s.strip()` | `s.strip(" \t\n\v\f\r")` |
| JavaScript, TypeScript | `s.trim()` | `s.replace` with a regular expression |
| Java | `s.strip()` | a loop from each end |
| C# | `s.Trim()` | `s.Trim(space)` with an array of 6 chars |
| Go | `strings.TrimSpace(s)` | `strings.Trim(s, space)` |
| Rust | `s.trim()` | `s.trim_matches(is_space)` |
| C | none | a loop from each end, then a copy |
| C++ | none | `find_first_not_of` and `find_last_not_of` |
| Bash | none | two pattern removals with `[![:space:]]` |

- **Python.** `str.strip` returns "a copy of the string with the leading and trailing characters removed"[^1]. With an argument, it removes each character of the argument.
- **JavaScript and TypeScript.** `trim` "removes whitespace from both ends of this string and returns a new string, without modifying the original string"[^2]. The regular expression has the 6 characters.
- **Java.** The loop finds the first and the last char that is not white space, and `substring` returns the text between them.
- **C#.** `Trim(Char[])` removes "all leading and trailing occurrences of a set of characters specified in an array from the current string"[^3].
- **Go.** `strings.Trim` takes a cutset, and it removes each code point of the cutset from both ends.
- **Rust.** `trim_matches` takes a function. `trim` returns a slice of the same string, so it does not copy.
- **C.** The code moves a pointer over the white space at the start, and it makes the length smaller for the white space at the end. Then it copies the text to a new string.
- **C++.** `find_first_not_of` and `find_last_not_of` find the first and the last char that is not white space.
- **Bash.** `${s%%[![:space:]]*}` is the white space at the start, and `${s#...}` removes it. In the C locale, the class `space` is the 6 characters.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `trim_start` are in `cases/start.jsonl`.

- `empty` and `all-space`: the result is the empty string.
- `vt-ff`: the vertical tab and the form feed are white space, so they are removed.
- `other-control`: U+0001 is a control character, but it is not white space, so it stays.
- `no-break-space`, `ideographic-space`, `next-line` and `bom`: U+00A0, U+3000, U+0085 and U+FEFF stay. The trim of the library removes some of them, and it is different in each language.
- `astral`: an emoji between two spaces stays.
- `combining`: U+0301 after a space stays. The trim removes the space, and the combining mark is then at the start of the string.
- `start-end-only`: `trim_start` keeps the white space at the end.

## Pitfalls

- **Python strip takes a set of characters.** The argument is not a prefix. "See str.removeprefix() for a method that will remove a single prefix string rather than all of a set of characters"[^1]. With no argument, `strip` removes U+00A0 and U+3000:

```python run id=python-strip-set
print("Arthur: three!".lstrip("Arthur: "))
print("Arthur: three!".removeprefix("Arthur: "))
print(len("\u00a0x\u3000".strip()))
```

```output for=python-strip-set
ee!
three!
1
```

- **JavaScript trim removes more than ASCII.** "Whitespace is defined as white space characters plus line terminators"[^2]. The output shows that `trim` removes U+00A0, U+FEFF and U+3000:

```javascript run id=javascript-trim-unicode
console.log("\u00a0\ufeffx\u3000".trim().length);
console.log(JSON.stringify("  x  ".trimStart()));
```

```output for=javascript-trim-unicode
1
"x  "
```

- **Java trim and strip use different sets.** `trim` removes each character "whose codepoint is less than or equal to 'U+0020' (the space character)"[^4], so it removes U+0001 and keeps U+3000. `strip` removes "all leading and trailing white space"[^4], and the output shows that it keeps U+00A0:

```java run id=java-trim-strip
public class Main {
  public static void main(String[] args) {
    System.out.println("\u0001x\u0001".trim().length());
    System.out.println("\u3000x\u3000".trim().length());
    System.out.println("\u3000x\u3000".strip().length());
    System.out.println("\u00a0x\u00a0".strip().length());
  }
}
```

```output for=java-trim-strip
1
3
1
3
```

- **C# Trim() uses the white space of Unicode.** `Trim()` removes "all leading and trailing white-space characters from the current string"[^3], and it removes U+00A0. `Trim('a', 'b')` removes a set, not a word:

```csharp run id=csharp-trim-set
using System;

Console.WriteLine("\u00a0x\u00a0".Trim().Length);
Console.WriteLine("abcab".Trim('a', 'b'));
```

```output for=csharp-trim-set
1
c
```

- **Go TrimLeft is not TrimPrefix.** `TrimLeft` removes each code point of the cutset. "To remove a prefix, use TrimPrefix instead"[^5]. `TrimSpace` removes the white space "as defined by Unicode"[^5]:

```go run id=go-trimleft-prefix
package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println(strings.TrimLeft("abacus", "ab"))
	fmt.Println(strings.TrimPrefix("abacus", "ab"))
	fmt.Println(len(strings.TrimSpace("\u00a0x\u0085")))
}
```

```output for=go-trimleft-prefix
cus
acus
1
```

- **Rust is_ascii_whitespace does not include the vertical tab.** "Because the list above excludes U+000B VERTICAL TAB, c.is_ascii_whitespace() is not equivalent to c.is_ascii() && c.is_whitespace()"[^6]. `trim` uses the property White_Space, "which includes newlines"[^7], and it removes U+000B:

```rust run id=rust-ascii-whitespace
fn main() {
    let s = "\x0bhi\x0b";
    println!("{}", s.trim().len());
    println!("{}", s.trim_matches(|c: char| c.is_ascii_whitespace()).len());
}
```

```output for=rust-ascii-whitespace
2
4
```

- **C isspace takes an unsigned char value.** "The behavior is undefined if the value of ch is not representable as unsigned char and is not equal to EOF"[^8]. To trim in place, keep the pointer to the start of the array, and move the text with `memmove`, because the two ranges overlap:

```c run id=c-trim-in-place
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "  hi  ";
    char *p = s;
    while (isspace((unsigned char)*p)) {
        p++;
    }
    size_t n = strlen(p);
    while (n > 0 && isspace((unsigned char)p[n - 1])) {
        n--;
    }
    memmove(s, p, n);
    s[n] = '\0';
    printf("[%s]\n", s);
    return 0;
}
```

```output for=c-trim-in-place
[hi]
```

- **C++ find_first_not_of can return npos.** "If all characters in the range can be found in the given character sequence, npos will be returned"[^9]. For a string of only white space, `substr(npos)` throws `std::out_of_range`, so check for `npos` first:

```cpp run id=cpp-npos-all-space
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    std::string s = "   ";
    std::size_t start = s.find_first_not_of(" \t\n\v\f\r");
    std::cout << (start == std::string::npos) << "\n";
    try {
        std::cout << s.substr(start) << "\n";
    } catch (const std::out_of_range &) {
        std::cout << "out_of_range\n";
    }
}
```

```output for=cpp-npos-all-space
1
out_of_range
```

- **Bash `${s## }` removes one space.** The pattern is one space, and the expansion deletes "the longest matching pattern (the ‘##’ case)"[^10], which is still one space. A pattern with `[![:space:]]` finds all of the white space:

```bash run id=bash-pattern-trim
s='   hi   '
echo "[${s## }]"
s=${s#"${s%%[![:space:]]*}"}
s=${s%"${s##*[![:space:]]}"}
echo "[$s]"
```

```output for=bash-pattern-trim
[  hi   ]
[hi]
```

## Related tasks

- [[check-whether-a-string-is-empty]]: a string that is empty after a trim is blank.
- [[split-a-string]]: trim each field after a split.
- [[remove-characters-from-a-string]]: remove a character from each position, not only from the ends.
- [[get-a-substring]]: the trim returns the substring between the first and the last other character.

## References
