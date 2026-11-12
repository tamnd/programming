---
tag: GK43CQ
kind: task
slug: join-a-list-of-strings
name: Join a list of strings
aliases: [join strings, join with a separator, implode, string join, comma-separated list, Collectors.joining]
area: data-and-values/strings
level: intro
summary: Join a list of strings into one string with a separator between each two items, and know how each language handles the empty list, items that are not strings and null items.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#str.join
  title: The Python Standard Library, Built-in Types
  locator: str.join
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/join
  title: MDN Web Docs, Array.prototype.join()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/Collectors.html
  title: Java SE 25 API, java.util.stream.Collectors
  locator: joining
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.join?view=net-10.0
  title: .NET API reference, String.Join method
  locator: Remarks
- url: https://pkg.go.dev/strings#Join
  title: Go standard library, package strings
  locator: Join
- url: https://doc.rust-lang.org/std/primitive.slice.html#method.join
  title: The Rust Standard Library, primitive type slice
  locator: join, concat
- url: https://en.cppreference.com/w/cpp/ranges/join_with_view
  title: cppreference.com, std::ranges::views::join_with
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Special-Parameters
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Special Parameters; Bash Builtin Commands, printf
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/toString
  title: MDN Web Docs, Array.prototype.toString()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, java.lang.String
  locator: join(CharSequence, CharSequence...)
---

## Task

Join a list of strings into one string, with a separator between each two items. The function `join(items, sep)` returns `items[0] + sep + items[1] + ... + sep + items[k-1]`. There is no separator at the start or the end. A list of one item gives the item itself, and the empty list gives the empty string.

A second function, `join_numbers(items, sep)`, joins a list of 32-bit integers. Some languages join items of any type, and some join only strings, so this is where the languages differ most.

## Solutions

| Language | Strings | Items that are not strings |
|---|---|---|
| Python | `sep.join(items)` | `sep.join(map(str, items))` |
| JavaScript, TypeScript | `items.join(sep)` | `items.map(String).join(sep)` |
| Java | `String.join(sep, items)` | `Collectors.joining(sep)` on a stream of strings |
| C# | `string.Join(sep, items)` | `string.Join(sep, items)` with an explicit culture |
| Go | `strings.Join(items, sep)` | convert to a `[]string` first |
| Rust | `items.join(sep)` | convert to a `Vec<String>` first |
| C | a loop with `memcpy` | `snprintf` for each item, then the join |
| C++ | a loop with `+=` | `std::to_string` for each item |
| Bash | a loop with `+=` | `printf '%d'` for each item, then the join |

- **Python.** `join` is a method of the separator. It returns "a string which is the concatenation of the strings in iterable", and "A TypeError will be raised if there are any non-string values in iterable"[^1], so the number function converts each item with `str`.
- **JavaScript and TypeScript.** `join` "returns a new string that is the concatenation of all elements in this array, separated by commas or a specified separator string"[^2]. It converts each item to a string, so `[1, 2].join(",")` works. The code calls `String` on each number to make the conversion clear.
- **Java.** `String.join` takes a separator and an `Iterable` of `CharSequence` items. For numbers, the code maps each `int` to a string and collects with `Collectors.joining`, which "concatenates the input elements, separated by the specified delimiter"[^3].
- **C#.** `string.Join` takes a separator and an array or an `IEnumerable<T>` of any type. For an item that is not a string, "The string representation of each object in the IEnumerable<T> collection is derived by calling that object's ToString method"[^4]. `ToString` uses the current culture, so the code converts each number with `CultureInfo.InvariantCulture`.
- **Go.** "Join concatenates the elements of its first argument to create a single string. The separator string sep is placed between elements in the resulting string"[^5]. It takes only a `[]string`, so the code converts each number with `strconv.Itoa`.
- **Rust.** `join` on a slice "Flattens a slice of T into a single value Self::Output, placing a given separator between each"[^6]. It works on `&[&str]` and on `Vec<String>`. For numbers, the code collects the strings into a `Vec` first.
- **C.** The code makes two passes. The first adds up the lengths of the items and the separators, and the second copies each part with `memcpy` into one allocation. The caller frees the result. `join_numbers` writes each number with `snprintf` into a buffer of 12 bytes, which holds `-2147483648` and the null byte.
- **C++.** C++23 adds `std::views::join_with`, which flattens a view of ranges "with every element of the delimiter inserted in between elements of the view"[^7]. The code uses a loop, so it works with a library that does not have `join_with` yet. It adds up the sizes, calls `reserve` once and appends with `+=`.
- **Bash.** In double quotes, `$*` joins the arguments "separated by the first character of the IFS variable"[^8], so it cannot use a separator of two or more characters. The function puts the first item in the result and appends the separator and the next item in a loop.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`, and the cases of `join_numbers` are in `cases/numbers.jsonl`.

- `empty-list`: the result is the empty string, not the separator.
- `single`: one item and no separator.
- `empty-items` and `empty-middle`: an empty item is still an item, so three empty items give two separators, `",,"`.
- `empty-sep`: the join is a concatenation of all the items.
- `long-sep`, `non-ascii` and `astral-sep`: the separator is any string. A solution that takes only one character, such as `IFS` in Bash, fails these cases.
- `sep-in-item`: `["a,b", "c"]` and `["a", "b", "c"]` give the same result with a comma. A join loses the boundaries when an item can hold the separator, so a split cannot always undo it. Quote or escape the items, as CSV does, if you must split the result again.
- `int32-ends`: the numbers `2147483647` and `-2147483648` need 10 and 11 characters.

## Pitfalls

- **Python `join` of a string joins its characters.** A `str` is an iterable of characters, so `",".join("abc")` is `"a,b,c"`. `None` and numbers raise `TypeError`:

```python run id=python-join-string-and-none
print(",".join("abc"))
print(",".join(["abc"]))
try:
    print(",".join(["a", None]))
except TypeError as e:
    print("TypeError:", e)
print(repr(",".join([])))
```

```output for=python-join-string-and-none
a,b,c
abc
TypeError: sequence item 1: expected str instance, NoneType found
''
```

- **JavaScript joins an array with a comma when it makes a string of it.** "The toString method of arrays calls join() internally"[^9], so a template literal gives `Ann,Bo` with no space. "If an element is undefined or null, it is converted to an empty string"[^2]:

```javascript run id=javascript-implicit-join
const names = ["Ann", "Bo"];
console.log(names.join());
console.log(`names: ${names}`);
console.log(String([]) === "");
console.log([null, undefined].join("-"));
```

```output for=javascript-implicit-join
Ann,Bo
names: Ann,Bo
true
-
```

- **Java `String.join` writes `null` as text.** For a `null` element, `String.join` adds the text `null`[^10]. `Collectors.joining` with a prefix and a suffix adds them "with the specified prefix and suffix"[^3], also for an empty stream:

```java run id=java-join-null-and-brackets
import java.util.List;
import java.util.stream.Collectors;

public class Main {
  public static void main(String[] args) {
    System.out.println(String.join(",", "a", null, "b"));
    System.out.println(List.of("a", "b").stream().collect(Collectors.joining(", ", "[", "]")));
    System.out.println(List.<String>of().stream().collect(Collectors.joining(", ", "[", "]")));
    System.out.println(List.of(1, 2, 3).stream().map(String::valueOf).collect(Collectors.joining("+")));
  }
}
```

```output for=java-join-null-and-brackets
a,null,b
[a, b]
[]
1+2+3
```

- **C# `string.Join` uses the current culture.** In the culture `de-DE`, `1.5` is `1,5`, so a join with a comma gives `1,5,2,25`, and nobody can split it again. "If any element in value is null, an empty string is used instead"[^4]:

```csharp run id=csharp-join-culture-and-null
using System.Globalization;

CultureInfo.CurrentCulture = new CultureInfo("de-DE");
double[] prices = { 1.5, 2.25 };
Console.WriteLine(string.Join(",", prices));
Console.WriteLine(string.Join(",", prices.Select(p => p.ToString(CultureInfo.InvariantCulture))));
Console.WriteLine(string.Join("|", new object?[] { 1, null, "x" }));
```

```output for=csharp-join-culture-and-null
1,5,2,25
1.5,2.25
1||x
```

- **A loop that adds the separator after each item.** It leaves one separator at the end. `strings.Join` adds it only between items. `fmt.Sprint` of a slice gives brackets and spaces, not a join:

```go run id=go-trailing-separator
package main

import (
	"fmt"
	"strings"
)

func main() {
	items := []string{"a", "b", "c"}
	var s string
	for _, it := range items {
		s += it + ","
	}
	fmt.Printf("%q\n", s)
	fmt.Printf("%q\n", strings.Join(items, ","))
	fmt.Printf("%q\n", strings.Join(nil, ","))
	fmt.Println(fmt.Sprint([]int{1, 2, 3}))
}
```

```output for=go-trailing-separator
"a,b,c,"
"a,b,c"
""
[1 2 3]
```

- **Rust `concat` has no separator, and `join` needs strings.** `join` takes a slice of `&str` or of `String`, so convert the numbers first:

```rust run id=rust-join-and-concat
fn main() {
    let words = ["red", "green", "blue"];
    println!("{}", words.join(", "));
    println!("{}", words.concat());
    let owned: Vec<String> = words.iter().map(|w| w.to_uppercase()).collect();
    println!("{}", owned.join("-"));
    let nums = [1, 2, 3];
    let parts: Vec<String> = nums.iter().map(|n| n.to_string()).collect();
    println!("{}", parts.join("+"));
}
```

```output for=rust-join-and-concat
red, green, blue
redgreenblue
RED-GREEN-BLUE
1+2+3
```

- **Bash `printf` with a separator in the format.** `printf '%s, '` repeats the format for each argument, so it leaves a comma and a space at the end, and `${line%, }` removes it. With no arguments, the format runs once, because "the extra format specifications behave as if a zero value or null string, as appropriate, had been supplied"[^8]. So the empty list gives a comma and a space, and not the empty string. `IFS=', '` joins with a comma only:

```bash run id=bash-printf-join
set -- a b c
(IFS=', '; echo "$*")
printf -v line '%s, ' "$@"
echo "[$line]"
echo "[${line%, }]"
set --
printf -v line '%s, ' "$@"
echo "[$line]"
```

```output for=bash-printf-join
a,b,c
[a, b, c, ]
[a, b, c]
[, ]
```

## Related tasks

- [[split-a-string]]: the reverse operation, which cuts a string at each separator.
- [[concatenate-strings]]: a join with one item on each side and an empty separator.
- [[build-a-string-efficiently]]: a join is one of the ways to build a long string in a loop.

## References
