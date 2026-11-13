---
tag: ZHK7ZJ
kind: task
slug: iterate-over-the-characters-of-a-string
name: Iterate over the characters of a string
aliases: [loop over characters, for each character, iterate over code points, chars, runes, string iterator]
area: data-and-values/strings
level: intro
summary: Visit each code point of a string in order, not each byte or each UTF-16 code unit.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#text-sequence-type-str
  title: The Python Standard Library, Built-in Types
  locator: Text Sequence Type, str; Bytes Objects
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/Symbol.iterator
  title: MDN Web Docs, String.prototype[Symbol.iterator]()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: class description, codePoints()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.enumeraterunes?view=net-10.0
  title: .NET API reference, String.EnumerateRunes method
  locator: Definition
- url: https://www.rfc-editor.org/rfc/rfc3629
  title: RFC 3629, UTF-8, a transformation format of ISO 10646
  locator: 3. UTF-8 definition
- url: https://go.dev/ref/spec#For_range
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://doc.rust-lang.org/std/primitive.str.html
  title: Rust standard library, primitive type str
  locator: len, chars, char_indices
- url: https://en.cppreference.com/w/cpp/string/basic_string/size
  title: cppreference.com, std::basic_string::size, length
  locator: Return value
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Word-Splitting
  title: Bash Reference Manual
  locator: 3.5.7 Word Splitting
---

## Task

Visit each character of a string, from the start to the end. The function `characters(s)` returns a list with one string for each code point of `s`. For example, `characters("abc")` is `["a", "b", "c"]`, and `characters("")` is an empty list.

A character is a code point. An emoji above U+FFFF is one character, and a combining mark such as U+0301 is a character of its own. The loop must not give the bytes of a UTF-8 string or the UTF-16 code units of a string. To visit what a reader sees as one character, see [[count-the-graphemes-of-a-string]].

## Solutions

| Language | Loop over the code points |
|---|---|
| Python | `for c in s` |
| JavaScript, TypeScript | `for (const c of s)` or `Array.from(s)` |
| Java | `s.codePoints()` |
| C# | `foreach (Rune r in s.EnumerateRunes())` |
| Go | `for i, r := range s` |
| Rust | `for c in s.chars()` |
| C | a loop that finds the length of each UTF-8 sequence from its first byte |
| C++ | the same loop, with `std::string_view::substr` |
| Bash | the same loop in the C locale, with `${s:i:n}` |

- **Python.** "Strings are immutable sequences of Unicode code points"[^1]. A loop over a `str` gives one code point at each step.
- **JavaScript and TypeScript.** "Strings are iterated by Unicode code points"[^2]. `for...of`, `Array.from` and the spread syntax use this iterator.
- **Java.** `codePoints()` returns "a stream of code point values from this sequence"[^3].
- **C#.** `EnumerateRunes` returns "an enumeration of Rune from this string"[^4]. A `Rune` is one code point.
- **Go.** A range loop over a string gives a rune at each step.
- **Rust.** `chars()` gives each `char`, which is a code point.
- **C, C++ and Bash.** A string is a sequence of bytes. In UTF-8, characters "are encoded using sequences of 1 to 4 octets"[^5]. The first byte gives the length: 1 byte below 0x80, 2 bytes from 0xC0, 3 bytes from 0xE0 and 4 bytes from 0xF0.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `single`: an empty string gives an empty list, and a string of one code point gives a list of one string.
- `non-ascii` and `cjk`: `é` (U+00E9) has 2 bytes in UTF-8, and each CJK character in `cjk` has 3 bytes. Each one is one character.
- `astral`: U+1F642 is one character, but it is 2 UTF-16 code units and 4 UTF-8 bytes.
- `combining`: `e` and U+0301 are 2 characters, but a reader sees one `é`.
- `regional-indicator`: a flag is 2 regional indicators, U+1F1FA and U+1F1F8, so it is 2 characters.
- `zwj`: U+1F469, U+200D and U+1F4BB are 3 characters. U+200D is the zero width joiner.
- `control`: a tab and a line feed are characters of their own.

## Pitfalls

- **Python bytes give integers.** For a `str`, the loop gives code points, so `e` and U+0301 are 2 steps. For `bytes`, "b[0] will be an integer"[^1], so the loop gives the byte values:

```python run id=python-bytes-integers
s = "e\u0301"
print([hex(ord(c)) for c in s])
print(list("h\u00e9".encode()))
```

```output for=python-bytes-integers
['0x65', '0x301']
[104, 195, 169]
```

- **JavaScript s[i] gives UTF-16 code units.** An index loop with `s.length` gives each half of a surrogate pair. `for...of` gives whole code points:

```javascript run id=javascript-index-units
const s = "a\u{1f642}";
const units = [];
for (let i = 0; i < s.length; i++) {
  units.push(s.charCodeAt(i).toString(16));
}
console.log(units.join(" "));
const points = [];
for (const c of s) {
  points.push(c.codePointAt(0).toString(16));
}
console.log(points.join(" "));
```

```output for=javascript-index-units
61 d83d de42
61 1f642
```

- **Java charAt gives UTF-16 code units.** "A String represents a string in the UTF-16 format in which supplementary characters are represented by surrogate pairs"[^3]. So a loop with `charAt` gives 0xD83D and 0xDE42 for U+1F642:

```java run id=java-charat-units
public class Main {
  public static void main(String[] args) {
    String s = "a\uD83D\uDE42";
    StringBuilder units = new StringBuilder();
    for (int i = 0; i < s.length(); i++) {
      units.append(Integer.toHexString(s.charAt(i))).append(' ');
    }
    System.out.println(units.toString().trim());
    StringBuilder points = new StringBuilder();
    s.codePoints().forEach(c -> points.append(Integer.toHexString(c)).append(' '));
    System.out.println(points.toString().trim());
  }
}
```

```output for=java-charat-units
61 d83d de42
61 1f642
```

- **C# foreach over a string gives chars.** A `char` is a UTF-16 code unit. U+1F642 is 2 chars, and the second char is a surrogate:

```csharp run id=csharp-foreach-chars
using System;
using System.Linq;

string s = "a\U0001F642";
Console.WriteLine($"{s.Length} {s.EnumerateRunes().Count()} {char.IsSurrogate(s[1])}");
```

```output for=csharp-foreach-chars
3 2 True
```

- **Go s[i] gives bytes, and the range index is a byte index.** In a range loop, "the index value will be the index of the first byte of successive UTF-8-encoded code points in the string"[^6]. So the index goes 0, 1, 3, not 0, 1, 2:

```go run id=go-range-byte-index
package main

import (
	"fmt"
	"strings"
)

func main() {
	s := "h\u00e9x"
	var bytes, runes []string
	for i := 0; i < len(s); i++ {
		bytes = append(bytes, fmt.Sprintf("%x", s[i]))
	}
	for i, r := range s {
		runes = append(runes, fmt.Sprintf("%d:%U", i, r))
	}
	fmt.Println(strings.Join(bytes, " "))
	fmt.Println(strings.Join(runes, " "))
}
```

```output for=go-range-byte-index
68 c3 a9 78
0:U+0068 1:U+00E9 3:U+0078
```

- **Rust len counts bytes.** For `len`, "This length is in bytes, not chars or graphemes"[^7]. `char_indices` gives each char with its byte position, so the positions skip a number after `é`:

```rust run id=rust-len-bytes
fn main() {
    let s = "h\u{e9}x";
    println!("{} {}", s.len(), s.chars().count());
    let offsets: Vec<usize> = s.char_indices().map(|(i, _)| i).collect();
    println!("{:?}", offsets);
}
```

```output for=rust-len-bytes
4 3
[0, 1, 3]
```

- **C s[i] gives bytes.** The loop over `h` and `é` takes 3 steps. To count code points, skip each continuation byte, which has the form `10xxxxxx`:

```c run id=c-index-bytes
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *s = "h\xc3\xa9";
    size_t points = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        printf(i == 0 ? "%02x" : " %02x", (unsigned char)s[i]);
        // A continuation byte has the form 10xxxxxx.
        if (((unsigned char)s[i] & 0xc0) != 0x80) {
            points++;
        }
    }
    printf("\n%zu bytes, %zu code points\n", strlen(s), points);
    return 0;
}
```

```output for=c-index-bytes
68 c3 a9
3 bytes, 2 code points
```

- **C++ range for gives chars, and size counts them.** `size()` returns "the number of CharT elements in the string"[^8]. For a `std::string`, a `CharT` is a byte, so U+1F642 takes 4 steps:

```cpp run id=cpp-range-for-bytes
#include <iostream>
#include <string>

int main() {
    std::string s = "a\xf0\x9f\x99\x82";
    int steps = 0;
    int points = 0;
    for (char c : s) {
        steps++;
        if ((static_cast<unsigned char>(c) & 0xc0) != 0x80) {
            points++;
        }
    }
    std::cout << s.size() << " " << steps << " " << points << "\n";
}
```

```output for=cpp-range-for-bytes
5 5 2
```

- **Bash `for` over `$s` gives words.** "The shell treats each character of `$IFS` as a delimiter, and splits the results of the other expansions into fields using these characters as field terminators"[^9]. So `for w in $s` gives words, and the space is lost. Use an index loop with `${s:i:1}`:

```bash run id=bash-for-words
s='a b'
for w in $s; do
  printf '[%s]' "$w"
done
echo
for ((i = 0; i < ${#s}; i++)); do
  printf '[%s]' "${s:i:1}"
done
echo
```

```output for=bash-for-words
[a][b]
[a][ ][b]
```

## Related tasks

- [[find-the-length-of-a-string]]: count the code points, the bytes or the code units.
- [[get-the-code-point-of-a-character]]: get the number of each character that the loop gives.
- [[count-the-graphemes-of-a-string]]: visit what a reader sees as one character.
- [[reverse-a-string]]: a loop over the characters in the other direction.

## References
