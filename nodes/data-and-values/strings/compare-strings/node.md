---
tag: 51ENB7
kind: task
slug: compare-strings
name: Compare strings
aliases: [string comparison, compare two strings, strcmp, compareTo, string equality, lexicographic order, sort strings, three-way comparison]
area: data-and-values/strings
level: intro
summary: Compare two strings for equality and order, and avoid a compare of addresses, of UTF-16 code units or of locale rules when you want the order of the code points.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/reference/expressions.html#value-comparisons
  title: The Python Language Reference, Expressions
  locator: Value comparisons
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Less_than
  title: MDN Web Docs, Less than (<)
  locator: Description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.compare?view=net-10.0
  title: .NET API reference, String.Compare method
  locator: Remarks
- url: https://pkg.go.dev/strings#Compare
  title: Go standard library, package strings
  locator: Compare, EqualFold
- url: https://doc.rust-lang.org/std/primitive.str.html#impl-Ord-for-str
  title: The Rust Standard Library, primitive type str
  locator: impl Ord for str; eq_ignore_ascii_case
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: cppreference.com, strcmp
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Bash-Conditional-Expressions
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Bash Conditional Expressions
- url: https://docs.python.org/3/library/stdtypes.html#str.casefold
  title: The Python Standard Library, Built-in Types
  locator: str.casefold
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/localeCompare
  title: MDN Web Docs, String.prototype.localeCompare()
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: compareTo(String); equals(Object)
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/best-practices-strings
  title: .NET documentation, Best practices for comparing strings
  locator: Recommendations for string usage
- url: https://en.cppreference.com/w/cpp/string/basic_string/operator_cmp
  title: cppreference.com, std::basic_string comparison operators
---

## Task

Compare two strings. The function `compare(a, b)` returns -1 if `a` is before `b`, 0 if the strings are equal, and 1 if `a` is after `b`. The function `equals(a, b)` returns true if the strings have the same code points in the same order. For example, `compare("apple", "banana")` is -1, and `compare("pear", "apple")` is 1.

The order is the order of the code points. At the first position where the strings are different, the string with the smaller code point is first, and a prefix is before the longer string. This order is not the order of a dictionary: `Z` is before `a`, and `10` is before `9`.

## Solutions

| Language | Equal | Order of code points | Order of the library |
|---|---|---|---|
| Python | `a == b` | `(a > b) - (a < b)` | the same |
| JavaScript, TypeScript | `a === b` | a loop with `codePointAt` | UTF-16 code units |
| Java | `a.equals(b)` | a loop with `codePointAt` | UTF-16 code units |
| C# | `string.Equals(a, b, StringComparison.Ordinal)` | a loop with `char.ConvertToUtf32` | the current culture |
| Go | `a == b` | `strings.Compare(a, b)` | the same |
| Rust | `a == b` | `a.cmp(b)` | the same |
| C | `strcmp(a, b) == 0` | the sign of `strcmp(a, b)` | the same |
| C++ | `a == b` | the sign of `a.compare(b)` | the same |
| Bash | `[[ $a == "$b" ]]` | `[[ $a < $b ]]` with `LC_ALL=C` | the current locale |

- **Python.** "Strings (instances of str) compare lexicographically using the numerical Unicode code points (the result of the built-in function ord()) of their characters"[^1]. A `bool` is an `int`, so `(a > b) - (a < b)` is -1, 0 or 1.
- **JavaScript and TypeScript.** The `<` operator compares strings "based on the values of the UTF-16 code units (not Unicode code points) they contain"[^2]. A code point above U+FFFF has a high surrogate from U+D800 to U+DBFF as its first code unit, so `<` puts it before U+E000 to U+FFFF. The loop compares code points, and it moves 2 code units for a code point above U+FFFF.
- **Java.** `compareTo` compares `char` values, which are UTF-16 code units, so the code uses the same loop as JavaScript. `equals` compares the characters of two strings.
- **C#.** `string.Compare` "uses the current culture to obtain culture-specific information such as casing rules and the alphabetic order of individual characters"[^3]. `CompareOrdinal` compares UTF-16 code units, so the code uses a loop over the code points.
- **Go.** "The result will be 0 if a == b, -1 if a < b, and +1 if a > b"[^4]. Go compares the bytes of the strings, and the order of UTF-8 bytes is the order of the code points.
- **Rust.** "Strings are ordered lexicographically by their byte values"[^5]. `cmp` returns an `Ordering`, and the code changes it to -1, 0 or 1.
- **C.** "The sign of the result is the sign of the difference between the values of the first pair of characters (both interpreted as unsigned char) that differ in the strings being compared"[^6]. The code keeps only the sign.
- **C++.** `std::string` compares with `Traits::compare`, which for `char` compares the characters as `unsigned char`. `compare` returns a negative number, 0 or a positive number, and the code keeps the sign.
- **Bash.** "When used with [[, the ‘<’ and ‘>’ operators sort lexicographically using the current locale"[^7]. With `LC_ALL=C`, the locale is the C locale, and the order is the order of the bytes. The function sets it in a subshell.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `equals` are in `cases/equal.jsonl`.

- `both-empty` and `empty-first`: two empty strings are equal, and the empty string is before each other string.
- `prefix` and `prefix-longer`: `app` is before `apple`, because it is a prefix of it.
- `upper-before-lower` and `lower-after-upper`: the code point of `Z` is U+005A and the code point of `a` is U+0061, so `Zebra` is before `apple`.
- `digits`: `10` is before `9`, because `1` is before `9`. Compare numbers as numbers.
- `non-ascii`: `é` is U+00E9, so it is after `z`, which is U+007A.
- `astral-after-bmp` and `astral`: U+FF61 is before U+1F642 in code point order. A compare of UTF-16 code units gives the opposite result, so this case fails in JavaScript, Java and C# with the library operator.
- `combining` and `eq-combining`: `café` and `café` look the same, but they have different code points, so they are not equal. `e` is U+0065 and `é` is U+00E9, so `café` is first.
- `eq-case` and `eq-trailing-space`: a difference in case or one more space makes the strings different.

## Pitfalls

- **Python sorts the capital letters first.** `sorted` uses the order of the code points, so `Cherry` is before `apple`. For a sort with no case, use `key=str.casefold`. Casefolding "is intended to remove all case distinctions in a string"[^8], so it also matches `ß` and `SS`:

```python run id=python-sort-case
words = ["banana", "Cherry", "apple"]
print(sorted(words))
print(sorted(words, key=str.casefold))
print("10" < "9", int("10") < int("9"))
print("Straße".casefold() == "STRASSE".casefold())
```

```output for=python-sort-case
['Cherry', 'apple', 'banana']
['apple', 'banana', 'Cherry']
True False
True
```

- **JavaScript sort and < use UTF-16 code units.** U+FF61 is after an emoji with `<`. The default `sort` also puts `B` before `a`. For text that people read, use `localeCompare`, which "returns a number indicating whether this string comes before, or after, or is the same as the given string in sort order"[^9]:

```javascript run id=javascript-utf16-order
const a = "\uff61";
const b = "\u{1f642}";
console.log(a < b);
console.log(["b", "a", "B", "A"].sort());
console.log(["b", "a", "B", "A"].sort((x, y) => x.localeCompare(y, "en")));
console.log("a".localeCompare("B", "en"));
```

```output for=javascript-utf16-order
false
[ 'A', 'B', 'a', 'b' ]
[ 'a', 'A', 'b', 'B' ]
-1
```

- **Java == compares references.** Two `String` objects with the same text are different objects, so `==` is false. Use `equals`. `compareTo` does not return only -1 and 1: it "returns the difference of the two character values"[^10]:

```java run id=java-equals-not-reference
public class Main {
  public static void main(String[] args) {
    String a = "apple";
    String b = new String("apple");
    System.out.println(a == b);
    System.out.println(a.equals(b));
    System.out.println("a".compareTo("c"));
    System.out.println("apple".equalsIgnoreCase("APPLE"));
  }
}
```

```output for=java-equals-not-reference
false
true
-2
true
```

- **C# string.Compare uses the current culture.** In the `en-US` culture, `a` is before `B`, but in the ordinal order `B` is first. The .NET guide says: "Use comparisons with StringComparison.Ordinal or StringComparison.OrdinalIgnoreCase for better performance"[^11]. The `==` operator is an ordinal compare:

```csharp run id=csharp-compare-culture
using System;
using System.Globalization;

CultureInfo.CurrentCulture = new CultureInfo("en-US");
Console.WriteLine(string.Compare("a", "B"));
Console.WriteLine(Math.Sign(string.CompareOrdinal("a", "B")));
Console.WriteLine(string.Equals("a", "A", StringComparison.OrdinalIgnoreCase));
Console.WriteLine("a" == "a");
```

```output for=csharp-compare-culture
-1
1
True
True
```

- **Go strings.Compare is rarely necessary.** "It is usually clearer and always faster to use the built-in string comparison operators ==, <, >, and so on"[^4]. Use `Compare` as the function of `slices.SortFunc`. `strings.EqualFold` compares with no case:

```go run id=go-compare-operators
package main

import (
	"fmt"
	"slices"
	"strings"
)

func main() {
	fmt.Println("B" < "a", "a" < "b")
	fmt.Println(strings.EqualFold("Go", "GO"))
	words := []string{"b", "a", "B"}
	slices.SortFunc(words, strings.Compare)
	fmt.Println(words)
}
```

```output for=go-compare-operators
true true
true
[B a b]
```

- **Rust sort is not alphabetical.** The byte order "is not necessarily the same as “alphabetical” order, which varies by language and locale"[^5]. A sort with `to_lowercase` as the key ignores the case. `eq_ignore_ascii_case` changes only the ASCII letters:

```rust run id=rust-sort-bytes
fn main() {
    let mut words = vec!["banana", "Cherry", "apple"];
    words.sort();
    println!("{words:?}");
    words.sort_by_key(|w| w.to_lowercase());
    println!("{words:?}");
    println!("{}", "Rust".eq_ignore_ascii_case("RUST"));
}
```

```output for=rust-sort-bytes
["Cherry", "apple", "banana"]
["apple", "banana", "Cherry"]
true
```

- **C == compares pointers.** Two arrays with the same text have different addresses. Use `strcmp`, and use only the sign of its result, because the value can be any negative or positive number:

```c run id=c-strcmp-not-pointers
#include <stdio.h>
#include <string.h>

int main(void) {
    char a[] = "text";
    char b[] = "text";
    const char *p = a;
    const char *q = b;
    // p == q compares two addresses, not the characters.
    printf("%d\n", p == q);
    printf("%d\n", strcmp(a, b) == 0);
    int r = strcmp("a", "c");
    printf("%d\n", (r > 0) - (r < 0));
    return 0;
}
```

```output for=c-strcmp-not-pointers
0
1
-1
```

- **C++ compares text only when one side is a std::string.** Two `const char *` values compare as pointers. When one side is a `std::string`, "All comparisons are done via the compare() member function"[^12]. In C++20, `<=>` gives a three-way result:

```cpp run id=cpp-string-compare
#include <compare>
#include <iostream>
#include <string>

int main() {
    const char *p = "text";
    std::string s = "text";
    // s == p compares the characters, because one side is a std::string.
    std::cout << (s == p) << "\n";
    std::cout << (s.compare("texts") < 0) << "\n";
    std::cout << ((s <=> std::string("tex")) > 0) << "\n";
}
```

```output for=cpp-string-compare
1
1
1
```

- **Bash [ reads < as a redirection.** In `[ $a < $b ]`, the shell opens the file that `$b` names, and `[` sees only one argument. Write `\<` in `[`, or use `[[`. "The test command uses ASCII ordering"[^7], and `[[` uses the order of the locale:

```bash run id=bash-test-redirect
export LC_ALL=C
a='B'
b='a'
[[ $a < $b ]] && echo "[[: B < a"
[ "$a" \< "$b" ] && echo "[: B < a, with \\<"
d=$(mktemp -d)
cd "$d" || exit 1
# Without the backslash, < reads the file a, which does not exist.
{ [ "$a" < "$b" ]; } 2>/dev/null || echo "[ without \\: < is a redirection"
cd / && rmdir "$d"
```

```output for=bash-test-redirect
[[: B < a
[: B < a, with \<
[ without \: < is a redirection
```

## Related tasks

- [[change-the-case-of-a-string]]: change the case before a compare with no case.
- [[check-whether-a-string-starts-with-a-prefix]]: compare only the start or the end of a string.
- [[normalize-unicode-text]]: make `café` and `café` equal before a compare.

## References
