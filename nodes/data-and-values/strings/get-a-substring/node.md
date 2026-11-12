---
tag: P4KMR1
kind: task
slug: get-a-substring
name: Get a substring
aliases: [substring, slice a string, substr, string slice, cut a string, part of a string]
area: data-and-values/strings
level: intro
summary: Get the part of a string between two indexes, know which unit each language counts, and know what happens when an index is out of range or falls inside a character.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 4; Text Sequence Type
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String
  title: MDN Web Docs, String
  locator: UTF-16 characters, Unicode code points, and grapheme clusters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: substring(int, int); offsetByCodePoints
- url: https://learn.microsoft.com/en-us/dotnet/api/system.char?view=net-10.0
  title: .NET API reference, Char struct
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.substring?view=net-10.0
  title: .NET API reference, String.Substring method
  locator: Substring(Int32, Int32)
- url: https://go.dev/ref/spec#Slice_expressions
  title: The Go Programming Language Specification
  locator: Slice expressions; For statements with range clause
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: The Rust Standard Library, primitive type str
  locator: get; is_char_boundary; char_indices
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: RFC 3629, UTF-8, a transformation format of ISO 10646
  locator: Section 3
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Shell Parameter Expansion
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/slice
  title: MDN Web Docs, String.prototype.slice()
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/substring
  title: MDN Web Docs, String.prototype.substring()
  locator: Description
- url: https://en.cppreference.com/w/c/string/byte/strncpy
  title: cppreference.com, strncpy, strncpy_s
- url: https://en.cppreference.com/w/cpp/string/basic_string/substr
  title: cppreference.com, std::basic_string substr
---

## Task

Get the part of a string from one index up to a second index. The function `substring(s, start, end)` returns the code points of `s` at the indexes `start` to `end - 1`. The range is half-open, so `end - start` is the length of the result, and `start = end` gives the empty string. For example, `substring("Hello, world", 7, 12)` is `"world"`.

The indexes count code points, so the result is the same in every language. The built-in operations of most languages count other units. Python counts code points[^1], and Go, Rust, C and C++ count bytes of UTF-8. In JavaScript, "Strings are represented fundamentally as sequences of UTF-16 code units"[^2]. A Java `String` is "a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs"[^3], and a C# `char` "Represents a character as a UTF-16 code unit"[^4]. For ASCII text, all of these units are the same. For other text, an index in bytes or code units can fall inside a character.

## Solutions

| Language | Built-in operation | Unit of the built-in | The code |
|---|---|---|---|
| Python | `s[start:end]` | code point | the slice |
| JavaScript, TypeScript | `s.slice(start, end)` | UTF-16 code unit | `Array.from(s)`, then `slice` and `join` |
| Java | `s.substring(begin, end)` | UTF-16 code unit | `offsetByCodePoints`, then `substring` |
| C# | `s.Substring(start, length)`, `s[start..end]` | UTF-16 code unit | an offset loop, then `s[from..to]` |
| Go | `s[low:high]` | byte | an offset loop with `range`, then `s[i:j]` |
| Rust | `&s[a..b]`, `s.get(a..b)` | byte | `char_indices().nth(k)`, then `s[i..j]` |
| C | `memcpy` | byte | an offset loop, then `malloc` and `memcpy` |
| C++ | `s.substr(pos, count)` | byte | an offset loop, then `substr` |
| Bash | `${s:offset:length}` | character of the locale | the expansion with `LC_ALL=C.UTF-8` |

- **Python.** "Strings are immutable sequences of Unicode code points"[^1], so `s[start:end]` is the answer. The slice is "the sequence of items with index k such that i <= k < j"[^1].
- **JavaScript and TypeScript.** `slice` and `substring` count UTF-16 code units, so they can cut an emoji in two. The code makes an array of the code points with `Array.from(s)`, slices the array and joins it.
- **Java.** `substring(begin, end)` "begins at the specified beginIndex and extends to the character at index endIndex - 1"[^3], and it counts `char` values. `offsetByCodePoints` "Returns the index within this String that is offset from the given index by codePointOffset code points"[^3], so the code converts each index with it first.
- **C#.** `Substring` "starts at a specified character position and has a specified length"[^5]. A `char` is one UTF-16 code unit[^4], so the code walks the string, skips one more unit at each surrogate pair, and then uses the range `s[from..to]`.
- **Go.** "For arrays or strings, the indices are in range if 0 <= low <= high <= len(a)"[^6], and `len` counts bytes. For a string, a `range` loop "iterates over the Unicode code points in the string starting at byte index 0"[^6], so the code uses a `range` loop to find the byte index of each code point index.
- **Rust.** `char_indices` "Returns an iterator over the chars of a string slice, and their positions"[^7]. The code takes the byte position of the code point `start` and of the code point `end`, and slices between them. The end of the string is a valid position.
- **C.** A `char *` holds bytes of UTF-8. In UTF-8, each byte after the first byte of a code point has the form `10xxxxxx`[^8], so "Character boundaries are easily found from anywhere in an octet stream"[^8]. The code counts the bytes that do not have this form to find each offset, copies the bytes with `memcpy` into a new allocation and adds the null byte. The caller frees the result.
- **C++.** The code finds the two byte offsets in the same way and calls `substr` with a position and a count.
- **Bash.** `${parameter:offset:length}` "expands to up to length characters of the value of parameter starting at the character specified by offset"[^9]. The function runs the expansion in a subshell with `LC_ALL=C.UTF-8`, where each code point is one character.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the empty string with `start = end = 0` gives the empty string.
- `whole` and `at-end`: `end` can be the length of the string. An index equal to the length is valid, and an index larger than it is outside the contract.
- `empty-range`: `start = end` in the middle of the string gives the empty string.
- `non-ascii`: in `naïve café`, the letter `ï` is 2 bytes in UTF-8, so an index in bytes and an index in code points are not the same after it. The CJK case has 3 bytes for each character.
- `astral` and `astral-only`: an emoji such as 🙂 is one code point, 4 bytes of UTF-8 and 2 UTF-16 code units. A solution that counts code units fails these cases.
- `combining` and `combining-mark`: `e\u0301` is the letter `e` and a combining accent, which are 2 code points. A cut between them is valid in this contract, and the result starts with the mark. To keep each character that a reader sees whole, count graphemes, as in [[count-the-graphemes-of-a-string]].

## Pitfalls

- **Python slices do not check the range.** "If i or j is greater than len(s), use len(s)"[^1], so a slice past the end is shorter than you asked, or empty. An index of one item raises `IndexError`:

```python run id=python-slice-clamps
s = "abc"
print(repr(s[1:10]), repr(s[5:9]), repr(s[-2:]))
try:
    s[5]
except IndexError as e:
    print("IndexError:", e)
```

```output for=python-slice-clamps
'bc' '' 'bc'
IndexError: string index out of range
```

- **JavaScript slice and substring are not the same.** For `slice`, "If indexStart < 0, the index is counted from the end of the string"[^10], and an end before the start gives the empty string. For `substring`, "If indexStart is greater than indexEnd, then the effect of substring() is as if the two arguments were swapped"[^11], and a negative index is 0. Both count code units, so a cut after 2 units of a string with an emoji keeps half of the emoji, and the result is not well formed:

```javascript run id=javascript-slice-and-substring
const s = "abc";
console.log(s.slice(2, 0), "|", s.substring(2, 0));
console.log(s.slice(-2), "|", s.substring(-2));
const t = "a\u{1F642}";
const cut = t.slice(0, 2);
console.log(cut.length, cut.isWellFormed());
```

```output for=javascript-slice-and-substring
 | ab
bc | abc
2 false
```

- **Java substring throws when the end is past the length.** It throws an exception "if the beginIndex is negative, or endIndex is larger than the length of this String object, or beginIndex is larger than endIndex"[^3]. The cut can also fall between the two halves of a surrogate pair:

```java run id=java-substring-bounds
public class Main {
  public static void main(String[] args) {
    String s = "Hello, world";
    System.out.println(s.substring(7, 12));
    try {
      s.substring(7, 20);
    } catch (StringIndexOutOfBoundsException e) {
      System.out.println("StringIndexOutOfBoundsException");
    }
    String t = "a🙂";
    String cut = t.substring(0, 2);
    System.out.println(cut.length() + " " + Character.isHighSurrogate(cut.charAt(1)));
  }
}
```

```output for=java-substring-bounds
world
StringIndexOutOfBoundsException
2 true
```

- **C# Substring takes a length, not an end.** `s.Substring(7, 5)` and the range `s[7..12]` give the same result. A call with an end index in place of the length throws `ArgumentOutOfRangeException` when "startIndex plus length indicates a position not within this instance"[^5]:

```csharp run id=csharp-substring-length
string s = "Hello, world";
Console.WriteLine(s.Substring(7, 5));
Console.WriteLine(s[7..12]);
try
{
    Console.WriteLine(s.Substring(7, 12));
}
catch (ArgumentOutOfRangeException)
{
    Console.WriteLine("ArgumentOutOfRangeException");
}
```

```output for=csharp-substring-length
world
world
ArgumentOutOfRangeException
```

- **Go slices a string by bytes.** `s[0:3]` of `naïve` keeps the first byte of `ï` only, and the result is not valid UTF-8. Convert to `[]rune` to count code points. "If the indices are out of range at run time, a run-time panic occurs"[^6]:

```go run id=go-slice-bytes
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	s := "naïve"
	cut := s[0:3]
	fmt.Printf("%q %v\n", cut, utf8.ValidString(cut))
	r := []rune(s)
	fmt.Println(string(r[0:3]))
	defer func() { fmt.Println("panic:", recover()) }()
	i := 9
	fmt.Println(s[2:i])
}
```

```output for=go-slice-bytes
"na\xc3" false
naï
panic: runtime error: slice bounds out of range [:9] with length 6
```

- **Rust panics on a byte index inside a character.** `is_char_boundary` "Checks that index-th byte is the first byte in a UTF-8 code point sequence or the end of the string"[^7]. Index 3 of `naïve` is not a boundary. `get` "Returns None whenever equivalent indexing operation would panic"[^7]:

```rust run id=rust-char-boundary
fn main() {
    let s = "naïve";
    println!("{:?}", (0..=s.len()).filter(|&i| s.is_char_boundary(i)).collect::<Vec<_>>());
    println!("{:?} {:?}", s.get(0..3), s.get(0..4));
    let t: String = s.chars().skip(2).take(3).collect();
    println!("{t}");
}
```

```output for=rust-char-boundary
[0, 1, 2, 4, 5, 6]
None Some("naï")
ïve
```

- **C strncpy does not always add the null byte.** "If count is reached before the entire array src was copied, the resulting character array is not null-terminated"[^12]. Add the null byte yourself. To print part of a string with no copy, give `printf` a precision with `%.*s`:

```c run id=c-strncpy-no-null
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *s = "Hello, world";
    printf("%.*s\n", 5, s + 7);
    char d[6];
    memset(d, 'z', sizeof d);
    strncpy(d, s, 3);
    printf("d[3] is '%c', not a null byte\n", d[3]);
    d[3] = '\0';
    printf("%s\n", d);
}
```

```output for=c-strncpy-no-null
world
d[3] is 'z', not a null byte
Hel
```

- **C++ substr takes a count, not an end.** "If the requested substring extends past the end of the string"[^13], the result stops at the end, so `substr(7, 12)` works and hides the mistake. A position past the end throws `std::out_of_range`[^13]:

```cpp run id=cpp-substr-count
#include <print>
#include <stdexcept>
#include <string>

int main() {
    std::string s = "Hello, world";
    std::println("{}", s.substr(7, 12));
    std::println("{}", s.substr(7, 5));
    try {
        s.substr(13);
    } catch (const std::out_of_range &) {
        std::println("std::out_of_range");
    }
}
```

```output for=cpp-substr-count
world
world
std::out_of_range
```

- **Bash needs a space before a negative offset.** "a negative offset must be separated from the colon by at least one space to avoid being confused with the ‘:-’ expansion"[^9]. Without the space, `${s:-3}` is the default value expansion, and it gives all of `Hello`:

```bash run id=bash-negative-offset
s=Hello
echo "${s:1:3}"
echo "${s: -3}"
echo "${s:-3}"
echo "${s:(-3):2}"
```

```output for=bash-negative-offset
ell
llo
Hello
ll
```

## Related tasks

- [[find-the-length-of-a-string]]: the length in bytes, code units and code points, which are the units of the indexes.
- [[split-a-string]]: cut a string at each separator, not at an index.
- [[count-the-graphemes-of-a-string]]: count and cut by the characters that a reader sees.

## References
