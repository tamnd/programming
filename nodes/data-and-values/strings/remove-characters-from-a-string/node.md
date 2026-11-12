---
tag: TVV18V
kind: task
slug: remove-characters-from-a-string
name: Remove characters from a string
aliases: [delete characters, strip characters, remove a character, filter characters, remove all occurrences, translate, retain]
area: data-and-values/strings
level: intro
summary: Remove each character of a set from a string, and remove whole code points, not bytes or UTF-16 code units.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.maketrans
  title: The Python Standard Library, Built-in Types
  locator: str.maketrans, str.translate
- url: https://pkg.go.dev/strings#Map
  title: Go standard library, package strings
  locator: Map
- url: https://doc.rust-lang.org/std/string/struct.String.html#method.retain
  title: Rust standard library, struct String
  locator: retain, remove
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
  title: MDN Web Docs, String.prototype.replace()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StringBuilder.html
  title: Java SE 25 API, Class StringBuilder
  locator: deleteCharAt(int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.remove?view=net-10.0
  title: .NET API reference, String.Remove method
  locator: Overloads
- url: https://en.cppreference.com/w/cpp/algorithm/remove
  title: cppreference.com, std::remove, std::remove_if
  locator: Notes
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Pattern-Matching
  title: Bash Reference Manual
  locator: 3.5.8.1 Pattern Matching
---

## Task

Remove each character of a set from a string. The function `remove_chars(s, chars)` returns `s` without each code point that is also in `chars`. The other code points stay in the same order. For example, `remove_chars("banana", "a")` is `bnn`, and `remove_chars("hello, world", "lo")` is `he, wrd`.

A character is a code point. An emoji above U+FFFF is one character, and a combining mark such as U+0301 is a character of its own. The code must not remove a part of a code point: one UTF-16 code unit of a surrogate pair, or one byte of a UTF-8 sequence.

## Solutions

| Language | Remove the code points of a set |
|---|---|
| Python | `s.translate(str.maketrans("", "", chars))` |
| JavaScript, TypeScript | `Array.from(s).filter` with a `Set` of `chars` |
| Java | `s.codePoints().filter` and `appendCodePoint` |
| C# | `EnumerateRunes` and a `HashSet<Rune>` |
| Go | `strings.Map` that returns -1 for a rune of `chars` |
| Rust | `s.chars().filter(...).collect()` |
| C | a loop over the UTF-8 sequences, and `memcmp` with each sequence of `chars` |
| C++ | the same loop, with `std::string_view` |
| Bash | `${s//"$c"/}` for each code point `c` of `chars` |

- **Python.** For `str.maketrans`, "If there is a third argument, it must be a string, whose characters will be mapped to None in the result"[^1]. `translate` removes each character that maps to `None`.
- **JavaScript and TypeScript.** `Array.from` and `new Set` use the iterator of a string, which gives code points. So a code point above U+FFFF is one element.
- **Java.** `codePoints()` gives the code points as an `IntStream`. `indexOf(int)` finds a code point above U+FFFF in `chars`.
- **C#.** A `Rune` is one code point. A surrogate pair is one `Rune`.
- **Go.** "If mapping returns a negative value, the character is dropped from the string with no replacement"[^2].
- **Rust.** `chars()` gives each `char`, which is a code point. To change a `String` in place, use `retain`, which keeps "only the characters specified by the predicate"[^3].
- **C and C++.** The first byte of a UTF-8 sequence gives its length: 1 byte below 0x80, 2 bytes from 0xC0, 3 bytes from 0xE0 and 4 bytes from 0xF0. The loop compares whole sequences.
- **Bash.** In the C locale, the loop takes the bytes of each code point of `chars` from the first byte. The quotes in `"$c"` make it a plain string, not a pattern.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `empty-set`: an empty string stays empty, and an empty set removes nothing.
- `none-found` and `all`: if no character is in the set, the string does not change, and if each character is in the set, the result is empty.
- `adjacent`: in `aab`, the two `a` characters are next to each other, and both are removed.
- `astral` and `astral-other`: U+1F642 and U+1F643 have the same high surrogate in UTF-16. The removal of U+1F643 must keep U+1F642.
- `non-ascii`: `à` (U+00E0) and `é` (U+00E9) have the same first byte 0xC3 in UTF-8. The removal of `é` must keep `à`.
- `combining-mark` and `combining-base`: the removal of U+0301 from `e` and U+0301 leaves `e`. The removal of `e` leaves U+0301 alone.
- `composed`: `é` as one code point is not `e` and U+0301, so it removes nothing. See [[normalize-unicode-text]].

## Pitfalls

- **Python strip removes only the ends.** `strip("-")` removes the `-` at the ends, and it keeps the others. Use `translate`, or `replace` for one character:

```python run id=python-strip-not-remove
s = "a-b-c-"
print(s.strip("-"))
print(s.translate(str.maketrans("", "", "-")))
print(s.replace("-", ""))
```

```output for=python-strip-not-remove
a-b-c
abc
abc
```

- **JavaScript replace with a string removes only the first match.** "If pattern is a string, only the first occurrence will be replaced"[^4]. Use `replaceAll`. Also, `split("")` gives UTF-16 code units, so it cuts an emoji in two:

```javascript run id=javascript-replace-first
console.log("a-b-c".replace("-", ""));
console.log("a-b-c".replaceAll("-", ""));
console.log("\u{1f642}".split("").length, Array.from("\u{1f642}").length);
```

```output for=javascript-replace-first
ab-c
abc
2 1
```

- **Java deleteCharAt in a loop skips a char.** After a removal, the next char moves to the same index, and `i++` skips it. Go from the end to the start. Also, "If the character at the given index is a supplementary character, this method does not remove the entire character"[^5]:

```java run id=java-delete-skip
public class Main {
  public static void main(String[] args) {
    StringBuilder up = new StringBuilder("aab");
    for (int i = 0; i < up.length(); i++) {
      if (up.charAt(i) == 'a') {
        up.deleteCharAt(i);
      }
    }
    System.out.println(up);
    StringBuilder down = new StringBuilder("aab");
    for (int i = down.length() - 1; i >= 0; i--) {
      if (down.charAt(i) == 'a') {
        down.deleteCharAt(i);
      }
    }
    System.out.println(down);
  }
}
```

```output for=java-delete-skip
ab
b
```

- **C# Remove(int) removes the end of the string.** `Remove(1)` returns "a new string in which all the characters in the current instance, beginning at a specified position and continuing through the last position, have been deleted"[^6]. To remove one character at an index, use `Remove(i, 1)`. To remove each copy of a character, use `Replace`:

```csharp run id=csharp-remove-index
using System;

Console.WriteLine("hello".Remove(1));
Console.WriteLine("hello".Remove(1, 1));
Console.WriteLine("hello".Replace("l", ""));
```

```output for=csharp-remove-index
h
hllo
heo
```

- **Go strings.IndexByte compares bytes.** A loop over the bytes of `s` removes each byte of `é`, and it removes the first byte of `à` too. The result is not valid UTF-8. `strings.Map` removes whole runes:

```go run id=go-bytes-not-runes
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	s, chars := "\u00e0\u00e9", "\u00e9"
	var b []byte
	for i := 0; i < len(s); i++ {
		if strings.IndexByte(chars, s[i]) < 0 {
			b = append(b, s[i])
		}
	}
	fmt.Println(len(b), utf8.Valid(b))
	r := strings.Map(func(r rune) rune {
		if strings.ContainsRune(chars, r) {
			return -1
		}
		return r
	}, s)
	fmt.Printf("%U\n", []rune(r))
}
```

```output for=go-bytes-not-runes
1 false
[U+00E0]
```

- **Rust String::remove takes a byte index.** It "Panics if idx is larger than or equal to the String’s length, or if it does not lie on a char boundary"[^3]. Byte 1 of `é` is in the middle of the char. Use `retain` to remove each match:

```rust run id=rust-retain-boundary
fn main() {
    let mut s = String::from("e\u{301}x");
    s.retain(|c| c != '\u{301}');
    println!("{s}");
    let t = String::from("\u{e9}x");
    println!("{}", t.is_char_boundary(1));
}
```

```output for=rust-retain-boundary
ex
false
```

- **C strchr compares bytes.** With `chars` set to `é`, the loop removes the bytes 0xC3 and 0xA9. It also removes the first byte of `à`, and only 0xA0 stays:

```c run id=c-strchr-bytes
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *s = "\xc3\xa0\xc3\xa9";
    const char *chars = "\xc3\xa9";
    for (const char *p = s; *p != '\0'; p++) {
        if (strchr(chars, *p) == NULL) {
            printf("%02x\n", (unsigned char)*p);
        }
    }
    return 0;
}
```

```output for=c-strchr-bytes
a0
```

- **C++ std::remove does not change the size.** "A call to remove or remove_if is typically followed by a call to a container's erase member function to actually remove elements from the container"[^7]. In C++20 and later, `std::erase` does both:

```cpp run id=cpp-erase-remove
#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::string s = "banana";
    auto end = std::remove(s.begin(), s.end(), 'a');
    std::cout << s.size() << " " << (end - s.begin()) << "\n";
    s.erase(end, s.end());
    std::cout << s << "\n";
    std::string t = "banana";
    std::erase(t, 'n');
    std::cout << t << "\n";
}
```

```output for=cpp-erase-remove
6 3
bnn
baaa
```

- **Bash [$chars] is a pattern.** "If the first character following the ‘[’ is a ‘!’ or a ‘^’ then any character not within the range matches"[^8]. If `chars` starts with `!`, the pattern removes each other character:

```bash run id=bash-bracket-negate
s='banana!'
chars='!a'
echo "${s//[$chars]/}"
echo "${s//[a!]/}"
```

```output for=bash-bracket-negate
aaa
bnn
```

## Related tasks

- [[replace-a-substring]]: replace a substring with another string, or remove it.
- [[trim-whitespace]]: remove characters only from the two ends.
- [[iterate-over-the-characters-of-a-string]]: visit each code point, the step that the removal does.
- [[normalize-unicode-text]]: make `é` one form before the removal.

## References
