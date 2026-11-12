---
tag: APAQPG
kind: task
slug: search-for-a-substring
name: Search for a substring
aliases: [find a substring, indexOf, contains, string search, strstr, find the position of a substring, rfind, lastIndexOf]
area: data-and-values/strings
level: intro
summary: Find the index of the first and the last match of a substring in a string, test whether a string contains a substring, and know the value that each language gives when there is no match.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.find
  title: The Python Standard Library, Built-in Types
  locator: str.find, str.index, str.rfind
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: MDN Web Docs, String
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: indexOf(String); contains; codePointCount
- url: https://learn.microsoft.com/en-us/dotnet/api/system.char?view=net-10.0
  title: .NET API reference, Char struct
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/indexOf
  title: MDN Web Docs, String.prototype.indexOf()
  locator: Return value; Checking occurrences
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.indexof?view=net-10.0
  title: .NET API reference, String.IndexOf method
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
  title: .NET fundamentals, Best practices for comparing strings in .NET
- url: https://pkg.go.dev/strings#Index
  title: Go standard library, package strings
  locator: Index, LastIndex, Contains
- url: https://doc.rust-lang.org/std/primitive.str.html#method.find
  title: The Rust Standard Library, primitive type str
  locator: find, rfind, contains
- url: https://en.cppreference.com/w/c/string/byte/strstr
  title: cppreference.com, strstr
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: RFC 3629, UTF-8, a transformation format of ISO 10646
  locator: Section 3
- url: https://en.cppreference.com/w/cpp/string/basic_string/find
  title: cppreference.com, std::basic_string find
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Conditional-Constructs
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Conditional Constructs; Shell Parameter Expansion
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/includes
  title: MDN Web Docs, String.prototype.includes()
- url: https://en.cppreference.com/w/cpp/string/basic_string/contains
  title: cppreference.com, std::basic_string contains
---

## Task

Find where a substring occurs in a string. The function `find(s, sub)` returns the index of the first match of `sub` in `s`, and -1 if there is no match. The second function, `find_last(s, sub)`, returns the index of the last match. For example, `find("abcabc", "bc")` is 1 and `find_last("abcabc", "bc")` is 4. The empty string matches at each position, so `find(s, "")` is 0 and `find_last(s, "")` is the length of `s`.

The indexes count code points, as in [[get-a-substring]], so that the result is the same in each language. The search functions of most languages return an index in another unit. Python counts code points[^1], and Go, Rust, C and C++ count bytes of UTF-8. In JavaScript, "Strings are represented fundamentally as sequences of UTF-16 code units"[^2]. A Java `String` is "a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs"[^3], and a C# `char` "Represents a character as a UTF-16 code unit"[^4]. So each solution converts the index of the built-in search to code points.

## Solutions

| Language | First match | Last match | No match | Test only |
|---|---|---|---|---|
| Python | `s.find(sub)` | `s.rfind(sub)` | -1 | `sub in s` |
| JavaScript, TypeScript | `s.indexOf(sub)` | `s.lastIndexOf(sub)` | -1 | `s.includes(sub)` |
| Java | `s.indexOf(sub)` | `s.lastIndexOf(sub)` | -1 | `s.contains(sub)` |
| C# | `s.IndexOf(sub, StringComparison.Ordinal)` | `s.LastIndexOf(sub, StringComparison.Ordinal)` | -1 | `s.Contains(sub)` |
| Go | `strings.Index(s, sub)` | `strings.LastIndex(s, sub)` | -1 | `strings.Contains(s, sub)` |
| Rust | `s.find(sub)` | `s.rfind(sub)` | `None` | `s.contains(sub)` |
| C | `strstr(s, sub)` | a loop of `strstr` | `NULL` | `strstr(s, sub) != NULL` |
| C++ | `s.find(sub)` | `s.rfind(sub)` | `npos` | `s.contains(sub)` |
| Bash | `${s%%"$sub"*}` | `${s%"$sub"*}` | no built-in value | `[[ $s == *"$sub"* ]]` |

- **Python.** `find` returns "the lowest index in the string where substring sub is found within the slice s[start:end]"[^1], and it returns -1 if sub is not found[^1]. A `str` counts code points, so the result needs no conversion. `rfind` gives the last match.
- **JavaScript and TypeScript.** `indexOf` returns "The index of the first occurrence of searchString found, or -1 if not found"[^5]. The index counts code units, so the code counts the code points in `s.slice(0, i)` with `Array.from`. A match of a well-formed string starts at a code point, so the slice does not cut a surrogate pair.
- **Java.** `indexOf` "Returns the index within this string of the first occurrence of the specified substring"[^3], in `char` values. `codePointCount(0, i)` converts the index to code points.
- **C#.** The overloads of `IndexOf` with no `StringComparison` argument do a culture-sensitive search: "This method performs a word (case-sensitive and culture-sensitive) search using the current culture"[^6]. The .NET guide says "Use overloads that explicitly specify the string comparison rules for string operations"[^7], so the code passes `StringComparison.Ordinal`. Then it counts the UTF-16 code units of the prefix that are not low surrogates. For an empty `sub`, `FindLast` returns the length in code points without a call.
- **Go.** "Index returns the index of the first instance of substr in s, or -1 if substr is not present in s"[^8]. The index counts bytes, so the code counts the code points of `s[:i]` with `utf8.RuneCountInString`.
- **Rust.** `find` "Returns the byte index of the first character of this string slice that matches the pattern. Returns None if the pattern doesn’t match"[^9]. The code counts the chars of `s[..i]` and maps `None` to -1.
- **C.** `strstr` returns a "Pointer to the first character of the found substring in str, or a null pointer if such substring is not found"[^10]. The code counts the code points in the bytes before the match. Each byte that does not have the form `10xxxxxx` starts a code point[^11]. C has no reverse `strstr`, so `find_last` searches again from the byte after each match.
- **C++.** `find` returns the "Position of the first character of the found substring or npos if no such substring is found"[^12]. `rfind` gives the last match. The code counts the code points before the position in the same way as the C code.
- **Bash.** The `%%` expansion deletes "the longest matching pattern"[^13] at the end of the value, and the `%` expansion deletes the shortest one. The pattern is the quoted `sub` and a `*` after it, so the rest is the text before the first or the last match, and its length is the index. The function tests for a match first with `[[ ... ]]`, and it counts the length with `LC_ALL=C.UTF-8`, where each code point is one character.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `find_last` are in `cases/last.jsonl`.

- `empty-sub`, `empty-s` and `both-empty`: the empty string matches at index 0 of each string, also of the empty string. A search for a non-empty string in the empty string gives -1.
- `longer-sub`: a substring that is longer than the string cannot match.
- `overlap` and `last-overlap`: in `aaaa`, the matches of `aa` overlap. The first match is at 0, and the last match is at 2, not at 1.
- `case-sensitive`: the search compares code points, so `hello` does not match `Hello`. Convert both strings with [[change-the-case-of-a-string]] for a search that ignores case.
- `non-ascii` and `cjk`: the index in code points is smaller than the index in bytes after the first character that is not ASCII.
- `astral` and `astral-sub`: an emoji is one code point and two UTF-16 code units, so a solution that returns the index of `indexOf` gives 3, not 2.
- `combining` and `precomposed`: `cafe\u0301` ends with `e` and a combining accent. A search for `e` finds the base letter, and a search for the precomposed `\u00e9` finds nothing, although a reader sees the same text. Use [[normalize-unicode-text]] before a search if the text can have both forms.

## Pitfalls

- **Python find returns -1, and -1 is a valid index.** `s[s.find(x)]` gives the last character when there is no match. Use `index`, which is "Like find(), but raise ValueError when the substring is not found"[^1]. To test only, "use the in operator"[^1]:

```python run id=python-find-minus-one
s = "Hello"
print("ell" in s, "" in s, s.find("x"), s.rfind("l"))
i = s.find("x")
print(repr(s[i]))
try:
    s.index("x")
except ValueError as e:
    print("ValueError:", e)
```

```output for=python-find-minus-one
True True -1 3
'o'
ValueError: substring not found
```

- **JavaScript indexOf returns 0 for a match at the start.** 0 is false in a condition, and -1 is true, so `if (t.indexOf(x))` is wrong in both directions. MDN says that "the correct way to check is test whether the return value is -1"[^5]. `includes` returns a boolean, and it "performs a case-sensitive search"[^14]:

```javascript run id=javascript-indexof-zero
const s = "\u{1F642}b";
console.log(s.indexOf("b"), Array.from(s).indexOf("b"));
const t = "apple";
if (t.indexOf("a")) {
  console.log("found a");
} else {
  console.log("no a? indexOf gave", t.indexOf("a"));
}
console.log(t.includes("a"), t.indexOf("x"));
```

```output for=javascript-indexof-zero
2 1
no a? indexOf gave 0
true -1
```

- **Java indexOf counts char values, not code points.** After an emoji, the index is one larger than the number of code points. `contains` "Returns true if and only if this string contains the specified sequence of char values"[^3]:

```java run id=java-indexof-units
public class Main {
  public static void main(String[] args) {
    String s = "🙂b";
    int i = s.indexOf("b");
    System.out.println(i + " " + s.codePointCount(0, i));
    System.out.println("Hello".indexOf("L") + " " + "Hello".toLowerCase().indexOf("l"));
    System.out.println("Hello".contains("ell"));
  }
}
```

```output for=java-indexof-units
2 1
-1 2
true
```

- **C# IndexOf of a string uses the current culture.** A culture-sensitive search ignores some characters, such as the soft hyphen U+00AD, so it finds a match that an ordinal search does not find. `Contains` does an ordinal search. Pass `StringComparison.Ordinal` to `IndexOf`:

```csharp run id=csharp-indexof-culture
// U+00AD is SOFT HYPHEN, an ignorable character in a culture-sensitive search.
string s = "animal";
Console.WriteLine(s.IndexOf("\u00ADn"));
Console.WriteLine(s.IndexOf("\u00ADn", StringComparison.Ordinal));
Console.WriteLine(s.IndexOf("\u00AD"));
Console.WriteLine(s.Contains("\u00ADn"));
```

```output for=csharp-indexof-culture
1
-1
0
False
```

- **Go strings.Index returns a byte index.** For `naïve café`, the match of `café` is at byte 7 and at code point 6. Use the byte index to slice the string, and count the runes only to show a position to a reader:

```go run id=go-index-bytes
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	s := "naïve café"
	i := strings.Index(s, "café")
	fmt.Println(i, utf8.RuneCountInString(s[:i]))
	fmt.Println(strings.Contains(s, "ïv"), strings.Index(s, "x"))
	fmt.Println(strings.Index(strings.ToLower("Café"), "café"))
}
```

```output for=go-index-bytes
7 6
true -1
0
```

- **Rust find returns an Option of a byte index.** "The pattern can be a &str, char, a slice of chars, or a function or closure that determines if a character matches"[^9], so `find(char::is_whitespace)` works:

```rust run id=rust-find-option
fn main() {
    let s = "naïve café";
    println!("{:?} {:?}", s.find("café"), s.find("x"));
    println!("{:?} {:?}", s.find('é'), s.find(char::is_whitespace));
    let i = s.find("café").unwrap();
    println!("{}", s[..i].chars().count());
}
```

```output for=rust-find-option
Some(7) None
Some(10) Some(6)
6
```

- **C strstr returns a pointer.** Check it for a null pointer before you use it, and subtract the start of the string to get the byte index. "If substr points to an empty string, str is returned"[^10]:

```c run id=c-strstr-pointer
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *s = "Hello, world";
    const char *p = strstr(s, "world");
    if (p != NULL) {
        printf("found at byte %td: %s\n", p - s, p);
    }
    printf("%s\n", strstr(s, "x") == NULL ? "NULL" : "found");
    printf("%td\n", strstr(s, "") - s);
}
```

```output for=c-strstr-pointer
found at byte 7: world
NULL
0
```

- **C++ find returns npos, not -1.** `npos` is the largest value of `std::size_t`, and a match at the start is 0, so `if (s.find(x))` is wrong in both directions. Compare with `npos`, or use `contains`, which "Checks if the string contains the given substring"[^15] (C++23):

```cpp run id=cpp-find-npos
#include <print>
#include <string>

int main() {
    std::string s = "Hello";
    std::size_t i = s.find("x");
    std::println("{} {}", i == std::string::npos, i);
    if (s.find("H")) {
        std::println("H found");
    } else {
        std::println("no H? find gave {}", s.find("H"));
    }
    std::println("{} {}", s.contains("ell"), s.rfind('l'));
}
```

```output for=cpp-find-npos
true 18446744073709551615
no H? find gave 0
true 3
```

- **Bash treats an unquoted variable as a pattern.** In `[[ $s == *$sub* ]]`, a `*` in `sub` matches any text. "If you quote any part of the pattern, using any of the shell’s quoting mechanisms, the quoted portion is matched literally"[^13]:

```bash run id=bash-quote-the-pattern
s=abc
sub='a*'
[[ $s == *$sub* ]] && echo "unquoted: match"
[[ $s == *"$sub"* ]] || echo "quoted: no match"
s='a*c'
[[ $s == *"$sub"* ]] && echo "quoted: match in a*c"
```

```output for=bash-quote-the-pattern
unquoted: match
quoted: no match
quoted: match in a*c
```

## Related tasks

- [[get-a-substring]]: cut the text at the index that a search returns.
- [[split-a-string]]: cut a string at each match of a separator.
- [[replace-a-substring]]: change each match of a substring.

## References
