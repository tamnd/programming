---
tag: AZD81Y
kind: task
slug: check-whether-a-string-starts-with-a-prefix
name: Check whether a string starts with a prefix
aliases: [starts with, ends with, startswith, endswith, has prefix, has suffix, strip a prefix, remove a suffix]
area: data-and-values/strings
level: intro
summary: Check whether a string starts with a prefix or ends with a suffix, remove a prefix or a suffix exactly once, and make the test case-insensitive without a culture-sensitive surprise.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.startswith
  title: The Python Standard Library, Built-in Types
  locator: str.startswith; str.removeprefix; str.lstrip
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/startsWith
  title: MDN Web Docs, String.prototype.startsWith()
  locator: Return value; Exceptions; Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/endsWith
  title: MDN Web Docs, String.prototype.endsWith()
  locator: Parameters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: startsWith(String); regionMatches(boolean, int, String, int, int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.startswith?view=net-10.0
  title: .NET API reference, String.StartsWith method
  locator: StartsWith(String); StartsWith(Char)
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
  title: .NET documentation, Best practices for comparing strings
  locator: Recommendations for string usage
- url: https://pkg.go.dev/strings#HasPrefix
  title: Go standard library, package strings
  locator: HasPrefix, CutPrefix, TrimLeft
- url: https://doc.rust-lang.org/std/primitive.str.html#method.starts_with
  title: The Rust Standard Library, primitive type str
  locator: starts_with, strip_prefix
- url: https://en.cppreference.com/w/c/string/byte/strncmp
  title: cppreference.com, strncmp
- url: https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with
  title: cppreference.com, std::basic_string_view starts_with
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Conditional-Constructs
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Conditional Constructs
---

## Task

Check whether a string starts with a prefix. The function `starts_with(s, prefix)` returns true if the first code points of `s` are the code points of `prefix`, in order, and `ends_with(s, suffix)` does the same test at the end of `s`. For example, `starts_with("Hello, world", "Hello")` is true, and `ends_with("report.txt", ".csv")` is false.

The test is case-sensitive, and it compares code points, not the letters that a reader sees. The empty string is a prefix and a suffix of each string, and a prefix that is longer than `s` does not match.

## Solutions

| Language | Starts with | Ends with | Remove a prefix |
|---|---|---|---|
| Python | `s.startswith(p)` | `s.endswith(p)` | `s.removeprefix(p)` |
| JavaScript, TypeScript | `s.startsWith(p)` | `s.endsWith(p)` | `s.slice(p.length)` after the test |
| Java | `s.startsWith(p)` | `s.endsWith(p)` | `s.substring(p.length())` after the test |
| C# | `s.StartsWith(p, StringComparison.Ordinal)` | `s.EndsWith(p, StringComparison.Ordinal)` | `s[p.Length..]` after the test |
| Go | `strings.HasPrefix(s, p)` | `strings.HasSuffix(s, p)` | `strings.TrimPrefix(s, p)` or `strings.CutPrefix(s, p)` |
| Rust | `s.starts_with(p)` | `s.ends_with(p)` | `s.strip_prefix(p)` |
| C | `strncmp(s, p, strlen(p)) == 0` | `memcmp` at the end | `s + strlen(p)` after the test |
| C++ | `s.starts_with(p)` | `s.ends_with(p)` | `s.substr(p.size())` after the test |
| Bash | `[[ $s == "$p"* ]]` | `[[ $s == *"$p" ]]` | `${s#"$p"}` |

- **Python.** `startswith` returns "True if string starts with the prefix, otherwise return False"[^1]. "prefix can also be a tuple of prefixes to look for"[^1], so one call can test more than one prefix.
- **JavaScript and TypeScript.** `startsWith` returns "true if the given characters are found at the beginning of the string, including when searchString is an empty string"[^2]. `endsWith` has a second argument, `endPosition`, that "Defaults to str.length"[^3].
- **Java.** `startsWith` returns true "if the character sequence represented by the argument is a prefix of the character sequence represented by this string"[^4]. "Note also that true will be returned if the argument is an empty string"[^4].
- **C#.** `StartsWith(String)` "performs a word (case-sensitive and culture-sensitive) comparison using the current culture"[^5], so the code passes `StringComparison.Ordinal`. The .NET guide says "Use comparisons with StringComparison.Ordinal or StringComparison.OrdinalIgnoreCase for better performance"[^6].
- **Go.** "HasPrefix reports whether the string s begins with prefix"[^7]. `HasSuffix` is the test at the end.
- **Rust.** `starts_with` "Returns true if the given pattern matches a prefix of this string slice"[^8]. The pattern can be a `&str`, a `char` or a closure.
- **C.** `strncmp` "Compares at most count characters of two possibly null-terminated arrays"[^9]. With the length of the prefix as the count, the result is 0 only when `s` starts with the prefix. A shorter `s` stops at its null character, so the comparison does not read past its end. For the suffix, the code compares the lengths first and then uses `memcmp` on the last bytes.
- **C++.** `std::string_view::starts_with` (C++20) "Checks if the string view begins with the given prefix"[^10]. `ends_with` does the test at the end. Both also take a single character.
- **Bash.** With `==` in `[[`, "the string to the right of the operator is considered a pattern"[^11]. "If you quote any part of the pattern, using any of the shell’s quoting mechanisms, the quoted portion is matched literally"[^11], so the function quotes the prefix and leaves the `*` outside the quotes. The function gives the result as the exit status.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `ends_with` are in `cases/ends.jsonl`.

- `empty-prefix`, `empty-s` and `both-empty`: the empty prefix matches each string, also the empty string. A non-empty prefix does not match the empty string.
- `longer-prefix` and `ends-longer`: a prefix or a suffix that is longer than `s` does not match. In C, the suffix test must check the lengths before it moves the pointer.
- `case-sensitive` and `ends-case`: `Hello` does not start with `hello`, and `A.TXT` does not end with `.txt`.
- `non-ascii`, `cjk` and `astral`: the test compares whole code points, so the byte length or the code unit length of a character has no effect on the result.
- `combining` and `precomposed`: `cafe\u0301` starts with `cafe`, because the accent U+0301 is a separate code point after the `e`. `caf\u00e9` with the precomposed `é` does not start with `cafe`. In the same way, `cafe\u0301` does not end with `e`.

## Pitfalls

- **Python lstrip and rstrip remove characters, not a prefix.** "The chars argument is not a prefix; rather, all combinations of its values are stripped"[^1]. So `'report.txt'.rstrip('.txt')` gives `repor`. Use `removeprefix` and `removesuffix`, which remove the text once. For a case-insensitive test, change the case of `s` first:

```python run id=python-rstrip-not-suffix
name = "report.txt"
print(name.rstrip(".txt"))
print(name.removesuffix(".txt"))
print("Arthur: three!".lstrip("Arthur: "))
print("Arthur: three!".removeprefix("Arthur: "))
print("photo.JPG".lower().endswith((".jpg", ".png")))
```

```output for=python-rstrip-not-suffix
repor
report
ee!
three!
True
```

- **JavaScript startsWith does not take a regex.** It throws a `TypeError` when the argument "is a regex"[^2]. "This method is case-sensitive"[^2], so change the case of both strings for a case-insensitive test. The second argument gives the index where the test starts:

```javascript run id=javascript-startswith-regex
const s = "Hello, world";
console.log(s.startsWith("world", 7));
console.log(s.toLowerCase().startsWith("hello"));
try {
  s.startsWith(/Hello/);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-startswith-regex
true
true
TypeError
```

- **Java has no case-insensitive startsWith.** Use `regionMatches(true, 0, prefix, 0, prefix.length())`. It "Tests if two string regions are equal"[^4], and the first argument turns off the case test:

```java run id=java-regionmatches
public class Main {
  public static void main(String[] args) {
    String s = "Hello, world";
    System.out.println(s.startsWith("hello"));
    System.out.println(s.regionMatches(true, 0, "hello", 0, 5));
    System.out.println(s.startsWith("world", 7));
  }
}
```

```output for=java-regionmatches
false
true
true
```

- **C# StartsWith(String) uses the current culture.** The culture rules can ignore some characters and can compare a letter and its accent as one unit. So `cafe\u0301` does not start with `cafe`, and a string that starts with a soft hyphen (U+00AD) starts with `cafe`. With `StringComparison.Ordinal`, the results are the opposite. Use `OrdinalIgnoreCase` for a case-insensitive test:

```csharp run id=csharp-startswith-culture
using System;
using System.Globalization;

CultureInfo.CurrentCulture = new CultureInfo("en-US");
string s = "cafe\u0301";
Console.WriteLine(s.StartsWith("cafe"));
Console.WriteLine(s.StartsWith("cafe", StringComparison.Ordinal));
Console.WriteLine("\u00ADcafe".StartsWith("cafe"));
Console.WriteLine("Hello".StartsWith("hello", StringComparison.OrdinalIgnoreCase));
```

```output for=csharp-startswith-culture
False
True
True
True
```

- **Go TrimLeft is not TrimPrefix.** `TrimLeft` removes "all leading Unicode code points contained in cutset"[^7], so a `TrimLeft` of `a` from `aaab` gives `b`. `TrimPrefix` removes the prefix once. `CutPrefix` also "reports whether it found the prefix"[^7]:

```go run id=go-trimleft-not-prefix
package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println(strings.TrimLeft("aaab", "a"))
	fmt.Println(strings.TrimPrefix("aaab", "a"))
	after, found := strings.CutPrefix("key=value", "key=")
	fmt.Println(after, found)
}
```

```output for=go-trimleft-not-prefix
b
aab
value true
```

- **Rust strip_prefix returns an Option.** "Unlike trim_start_matches, this method removes the prefix exactly once"[^8], and it returns `None` when there is no prefix. A closure as the pattern tests the first character:

```rust run id=rust-strip-prefix
fn main() {
    println!("{:?}", "aaab".strip_prefix('a'));
    println!("{:?}", "aaab".trim_start_matches('a'));
    println!("{:?}", "b".strip_prefix('a'));
    println!("{}", "42 apples".starts_with(|c: char| c.is_ascii_digit()));
}
```

```output for=rust-strip-prefix
Some("aab")
"b"
None
true
```

- **C strncmp with the wrong count.** If the count is the length of `s`, a short `s` such as `ab` seems to start with `abc`, because only two characters are compared. Use the length of the prefix:

```c run id=c-strncmp-count
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *s = "ab";
    const char *prefix = "abc";
    // Wrong: the count is the length of s, so only "ab" is compared.
    printf("%d\n", strncmp(s, prefix, strlen(s)) == 0);
    // Right: the count is the length of the prefix.
    printf("%d\n", strncmp(s, prefix, strlen(prefix)) == 0);
    return 0;
}
```

```output for=c-strncmp-count
1
0
```

- **C++ before C++20 has no starts_with.** `s.rfind(p, 0) == 0` tests only the position 0. `s.find(p) == 0` gives the correct result, but it searches all of `s` when the prefix is not at the start:

```cpp run id=cpp-rfind-prefix
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello, world";
    // Before C++20: rfind with the position 0 looks only at the start.
    std::cout << (s.rfind("Hello", 0) == 0) << "\n";
    std::cout << s.starts_with("Hello") << " " << s.ends_with('d') << "\n";
    // find searches all of s, so it is slow when there is no match at 0.
    std::cout << (s.find("world") == 0) << "\n";
}
```

```output for=cpp-rfind-prefix
1
1 1
0
```

- **Bash reads an unquoted prefix as a pattern.** If the prefix is `*` and it has no quotes, each string matches. With the option `nocasematch`, "the match is performed without regard to the case of alphabetic characters"[^11]:

```bash run id=bash-prefix-glob
p='*'
s='Hello'
[[ $s == $p* ]] && echo "unquoted: match"
[[ $s == "$p"* ]] || echo "quoted: no match"
shopt -s nocasematch
[[ $s == "hello"* ]] && echo "nocasematch: match"
```

```output for=bash-prefix-glob
unquoted: match
quoted: no match
nocasematch: match
```

## Related tasks

- [[search-for-a-substring]]: find a match at any position, not only at the start.
- [[trim-whitespace]]: remove the whitespace at the start and at the end.
- [[change-the-case-of-a-string]]: change the case before a case-insensitive test.
- [[compare-strings]]: compare two whole strings.

## References
