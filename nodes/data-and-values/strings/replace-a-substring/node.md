---
tag: 5S107Z
kind: task
slug: replace-a-substring
name: Replace a substring
aliases: [replace text, replaceAll, string replace, substitute a substring, find and replace, str.replace]
area: data-and-values/strings
level: intro
summary: Replace each match, or only the first match, of a substring in a string with other text, and know which replace functions take a pattern and which characters of the new text have a special meaning.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.replace
  title: The Python Standard Library, Built-in Types
  locator: str.replace; Text Sequence Type
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace
  title: MDN Web Docs, String.prototype.replace()
  locator: Description; Specifying a string as the replacement; Specifying a function as the replacement
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: replace(CharSequence, CharSequence); replaceAll; replaceFirst
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.replace?view=net-10.0
  title: .NET API reference, String.Replace method
  locator: Replace(String, String)
- url: https://pkg.go.dev/strings#Replace
  title: Go standard library, package strings
  locator: Replace, ReplaceAll
- url: https://doc.rust-lang.org/std/primitive.str.html#method.replace
  title: The Rust Standard Library, primitive type str
  locator: replace, replacen
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Shell-Parameter-Expansion
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Shell Parameter Expansion; Pattern Matching
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replaceAll
  title: MDN Web Docs, String.prototype.replaceAll()
  locator: Parameters
- url: https://en.cppreference.com/w/c/string/byte/strchr
  title: cppreference.com, strchr
- url: https://en.cppreference.com/w/cpp/string/basic_string/replace
  title: cppreference.com, std::basic_string replace
- url: https://en.cppreference.com/w/cpp/algorithm/ranges/replace
  title: cppreference.com, std::ranges::replace
---

## Task

Replace a substring in a string with other text. The function `replace(s, old, new)` returns `s` with each match of `old` replaced by `new`, and `replace_first(s, old, new)` replaces only the first match. For example, `replace("a-b-c", "-", "+")` is `"a+b+c"`, and `replace_first("a-b-c", "-", "+")` is `"a+b-c"`.

The search is literal and case-sensitive, and it goes from left to right. After a match, the search goes on after the end of the match, so the matches do not overlap, and the new text is not searched again. `old` is not empty, because the languages do not agree on what a replace of the empty string does. The characters of `new` have no special meaning, so `$`, `&` and `\` stay as they are.

## Solutions

| Language | All matches | First match | Literal? |
|---|---|---|---|
| Python | `s.replace(old, new)` | `s.replace(old, new, 1)` | yes |
| JavaScript, TypeScript | `s.replaceAll(old, () => nw)` | `s.replace(old, () => nw)` | the pattern yes, the new text no |
| Java | `s.replace(old, nw)` | `indexOf` and `substring` | `replace` yes, `replaceAll` and `replaceFirst` no |
| C# | `s.Replace(old, nw, StringComparison.Ordinal)` | `IndexOf` and `string.Concat` | yes |
| Go | `strings.ReplaceAll(s, old, nw)` | `strings.Replace(s, old, nw, 1)` | yes |
| Rust | `s.replace(old, new)` | `s.replacen(old, new, 1)` | yes |
| C | a loop with `strstr` and `memcpy` | the same loop, once | yes |
| C++ | a loop with `find` and `+=` | the same loop, once | yes |
| Bash | `${s//"$old"/$new}` | `${s/"$old"/$new}` | only with quotes |

- **Python.** `str.replace` returns "a copy of the string with all occurrences of substring old replaced by new"[^1]. "If count is given, only the first count occurrences are replaced"[^1], so `replace_first` passes 1.
- **JavaScript and TypeScript.** `replace` with a string pattern changes one match: "If pattern is a string, only the first occurrence will be replaced"[^2]. `replaceAll` changes each match. In a replacement string, `$&` "Inserts the matched substring"[^2], so the code passes a function that returns `nw`. "The function's result (return value) will be used as the replacement string"[^2], and the result has no special patterns.
- **Java.** `replace(CharSequence, CharSequence)` "Replaces each substring of this string that matches the literal target sequence with the specified literal replacement sequence"[^3]. Java has no literal `replaceFirst`, so the code finds the first match with `indexOf` and joins the two parts and `nw`.
- **C#.** `Replace` "Returns a new string in which all occurrences of a specified string in the current instance are replaced with another specified string"[^4]. The code passes `StringComparison.Ordinal`, so the search compares code units. For the first match, the code finds the index with `IndexOf` and joins the parts with `string.Concat`.
- **Go.** "Replace returns a copy of the string s with the first n non-overlapping instances of old replaced by new"[^5]. `ReplaceAll` is `Replace` with no limit.
- **Rust.** `replace` "Replaces all matches of a pattern with another string"[^6], and `replacen` "Replaces first N matches of a pattern with another string"[^6]. Both return a new `String`.
- **C.** C has no replace function. The code makes two passes with `strstr`. The first pass counts the matches, and the second pass copies the text between the matches and the new text into one allocation of the right size. The caller frees the result.
- **C++.** `std::string::replace` replaces a range of positions, not a match, so the code uses a loop. It appends the text before each match and `new_` to the result, and it searches again after the end of the match.
- **Bash.** In `${parameter//pattern/string}`, "The longest match of pattern in the expanded value is replaced with string"[^7]. `pattern` is a glob, and "The special pattern characters must be quoted if they are to be matched literally"[^7], so the function quotes `old`. With the option `patsub_replacement` of Bash 5.2, "any unquoted instances of ‘&’ in string are replaced with the matching portion of pattern"[^7]. The function turns the option off in a subshell and gives the new text in a variable.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `replace_first` are in `cases/first.jsonl`.

- `none` and `first-none`: no match gives the string with no change.
- `empty-s`: the empty string has no match of a non-empty `old`.
- `delete`: an empty `new` deletes each match.
- `new-contains-old`: `aXa` with `X` replaced by `XXX` is `aXXXa`. The search does not look at the new text again, so the replace ends.
- `overlap` and `overlap-odd`: `aaaa` with `aa` replaced by `b` is `bb`, and `aaa` gives `ba`. The matches do not overlap, and the search starts at the left.
- `case-sensitive`: `hello` does not match `Hello`.
- `non-ascii` and `astral`: the search compares whole code points, so a replace of `é` or of an emoji changes all of its bytes or code units.
- `combining` and `precomposed`: in `cafe\u0301`, the `e` is a separate code point, so a replace of `e` with `E` gives `E` with the accent on it. The precomposed `\u00e9` has no `e` in it, so the same replace changes nothing.
- `dot`, `dollar`, `ampersand` and `backslash`: these characters have a special meaning in a regex or in a replacement string, so a solution that takes a pattern fails these cases.

## Pitfalls

- **Python strings are immutable.** "Strings are immutable sequences of Unicode code points"[^1], so `replace` returns a new string and does not change `s`. A call that does not use the result does nothing. The matches do not overlap, so `'aaaa'.replace('aa', 'a')` gives `aa`, not `a`:

```python run id=python-replace-returns-new
s = "a-b-c"
s.replace("-", "+")
print(s)
s = s.replace("-", "+", 1)
print(s)
print("aaaa".replace("aa", "a"))
```

```output for=python-replace-returns-new
a-b-c
a+b-c
aa
```

- **JavaScript replace with a string changes only the first match.** Use `replaceAll`. `$&` in the new text inserts the match, so pass a function if the new text comes from a user. "If pattern is a regex, then it must have the global (g) flag set, or a TypeError is thrown"[^8]:

```javascript run id=javascript-replace-first-only
const s = "a-b-c";
console.log(s.replace("-", "+"), s.replaceAll("-", "+"));
console.log("price".replace("price", "$&!"), "price".replace("price", () => "$&!"));
try {
  s.replaceAll(/-/, "+");
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-replace-first-only
a+b-c a+b+c
price! $&!
TypeError
```

- **Java replaceAll takes a regex.** The regex `.` matches any character, so a `replaceAll` of `.` with `-` replaces all of the string. "Note that backslashes (\) and dollar signs ($) in the replacement string may cause the results to be different than if it were being treated as a literal replacement string"[^3]. A `$5` with no group 5 throws an exception. Use `replace` for literal text:

```java run id=java-replaceall-regex
public class Main {
  public static void main(String[] args) {
    String s = "a.b.c";
    System.out.println(s.replace(".", "-") + " " + s.replaceAll(".", "-"));
    System.out.println(s.replaceAll("\\.", "-"));
    try {
      "cost".replaceAll("cost", "$5");
    } catch (IndexOutOfBoundsException e) {
      System.out.println(e.getClass().getSimpleName());
    }
  }
}
```

```output for=java-replaceall-regex
a-b-c -----
a-b-c
IndexOutOfBoundsException
```

- **C# Replace throws for an empty old value.** It throws `ArgumentException` when "oldValue is the empty string"[^4]. "If newValue is null, all occurrences of oldValue are removed"[^4]. `Replace(char, char)` changes single characters:

```csharp run id=csharp-replace-empty-old
string s = "a-b-c";
Console.WriteLine(s.Replace("-", null));
Console.WriteLine(s.Replace('-', '+'));
try
{
    s.Replace("", "+");
}
catch (ArgumentException e)
{
    Console.WriteLine(e.GetType().Name);
}
```

```output for=csharp-replace-empty-old
abc
a+b+c
ArgumentException
```

- **Go and Rust insert the new text at each position for an empty old.** In Go, an empty old "matches at the beginning of the string and after each UTF-8 sequence, yielding up to k+1 replacements for a k-rune string"[^5]. In `strings.Replace`, "If n < 0, there is no limit on the number of replacements"[^5]:

```go run id=go-replace-empty-old
package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println(strings.Replace("a-b-c", "-", "+", 1), strings.Replace("a-b-c", "-", "+", -1))
	fmt.Println(strings.ReplaceAll("abc", "", "-"))
	fmt.Println(strings.ReplaceAll("naïve", "", "-"))
}
```

```output for=go-replace-empty-old
a+b-c a+b+c
-a-b-c-
-n-a-ï-v-e-
```

- **Rust replace takes any pattern.** The old text can be a `&str`, a `char` or a closure, so one call can replace each digit. `replace` "creates a new String"[^6], and the original `&str` does not change:

```rust run id=rust-replace-pattern
fn main() {
    let s = "a-b-c";
    let t = s.replace('-', "+");
    println!("{s} {t}");
    println!("{}", s.replacen('-', "+", 1));
    println!("{}", "abc".replace("", "-"));
    println!("{}", "a1b22c".replace(|c: char| c.is_ascii_digit(), "#"));
}
```

```output for=rust-replace-pattern
a-b-c a+b+c
a+b-c
-a-b-c-
a#b##c
```

- **C can replace in place only when the length does not change.** A loop with `strchr`, which "Finds the first occurrence of ch"[^9], can change each `-` to `+` in a `char` array. A longer or shorter new text needs a new buffer, as in the solution:

```c run id=c-replace-in-place
#include <stdio.h>
#include <string.h>

int main(void) {
    char s[] = "a-b-c";
    for (char *p = strchr(s, '-'); p != NULL; p = strchr(p + 1, '-')) {
        *p = '+';
    }
    printf("%s\n", s);
}
```

```output for=c-replace-in-place
a+b+c
```

- **C++ std::string::replace takes positions, not text.** It "Replaces the characters in the range"[^10] that a position and a count give. To replace single characters, use `std::ranges::replace`, which replaces "all elements that are equal to old_value"[^11]:

```cpp run id=cpp-replace-range
#include <algorithm>
#include <print>
#include <string>

int main() {
    std::string s = "Hello, world";
    s.replace(7, 5, "there");
    std::println("{}", s);
    std::string t = "a-b-c";
    std::ranges::replace(t, '-', '+');
    std::println("{}", t);
}
```

```output for=cpp-replace-range
Hello, there
a+b+c
```

- **Bash reads the old text as a glob.** In `${s//*/-}`, the `*` matches all of the string, so the result is one `-`. A `*` in quotes, or a variable in quotes, matches only the character `*`:

```bash run id=bash-replace-glob
s='a*b*c'
echo "${s//*/-}"
echo "${s//"*"/-}"
p='*'
echo "${s//$p/-}" "${s//"$p"/-}"
```

```output for=bash-replace-glob
-
a-b-c
- a-b-c
```

## Related tasks

- [[search-for-a-substring]]: find the index of a match without a change.
- [[split-a-string]]: cut a string at each match of a separator.
- [[remove-characters-from-a-string]]: delete characters, which is a replace with the empty string.

## References
