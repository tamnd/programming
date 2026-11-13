---
tag: HJPBMY
kind: task
slug: repeat-a-string
name: Repeat a string
aliases: [string repeat, repeat, multiply a string, string times n, replicate a string, str_repeat]
area: data-and-values/strings
level: intro
summary: Make a string of n copies of a string, and know what each language does with a count of 0, a negative count and a count that overflows.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 2
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/repeat
  title: MDN Web Docs, String.prototype.repeat()
  locator: Exceptions, Examples
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/String.html
  title: Java SE 25 API, Class String
  locator: repeat(int)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.repeat?view=net-10.0
  title: .NET API reference, Enumerable.Repeat method
  locator: Summary
- url: https://pkg.go.dev/strings#Repeat
  title: Go standard library, package strings
  locator: Repeat
- url: https://doc.rust-lang.org/std/primitive.str.html#method.repeat
  title: Rust standard library, primitive type str
  locator: repeat
- url: https://learn.microsoft.com/en-us/dotnet/api/system.string.-ctor?view=net-10.0
  title: .NET API reference, String constructors
  locator: String(Char, Int32)
- url: https://en.cppreference.com/w/cpp/string/basic_string/basic_string
  title: cppreference.com, std::basic_string constructors
  locator: overload 3
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Brace-Expansion
  title: Bash Reference Manual
  locator: 3.5.1 Brace Expansion
---

## Task

Make a string of `n` copies of the string `s`. The function `repeat(s, n)` returns the copies joined with nothing between them. For example, `repeat("ab", 3)` is `ababab`, and `repeat("-", 5)` is `-----`. If `n` is 0 or `s` is empty, the result is the empty string.

The count `n` is from 0 to 1000 in the contract. A negative count is not in the contract, because the languages do not agree on it. The result has `len(s) * n` units, so the code can reserve the memory one time.

## Solutions

| Language | Repeat | Note |
|---|---|---|
| Python | `s * n` | a negative `n` gives the empty string |
| JavaScript, TypeScript | `s.repeat(n)` | a negative `n` throws `RangeError` |
| Java | `s.repeat(n)` | a negative `n` throws |
| C# | a `StringBuilder` with the full capacity | `new string(c, n)` is only for one `char` |
| Go | `strings.Repeat(s, n)` | a negative `n` panics |
| Rust | `s.repeat(n)` | `n` is a `usize`, so it is never negative |
| C | `malloc`, then `memcpy` `n` times | check that `len * n` does not overflow |
| C++ | `reserve`, then `+=` `n` times | `std::string(n, c)` is only for one `char` |
| Bash | a loop with `+=` | |

- **Python.** The `*` operator of a sequence and an int repeats the sequence. "Values of n less than 0 are treated as 0 (which yields an empty sequence of the same type as s)"[^1].
- **JavaScript and TypeScript.** `repeat` returns a new string "containing the specified number of copies of the given string"[^2].
- **Java.** `repeat` returns "a string whose value is the concatenation of this string repeated count times"[^3]. It is in Java 11 and later.
- **C#.** `string.Concat(Enumerable.Repeat(s, n))` also works. `Enumerable.Repeat` gives "a sequence that contains one repeated value"[^4].
- **Go.** "Repeat returns a new string consisting of count copies of the string s"[^5].
- **Rust.** `repeat` makes "a new String by repeating a string n times"[^6].
- **C.** The code finds the length one time, allocates `len * n + 1` bytes, and copies `s` to each place.
- **C++.** `reserve` allocates the memory one time, so each `+=` does not allocate again.
- **Bash.** The loop appends `s` to a variable `n` times, and `printf '%s'` prints it with no newline.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: 0 copies of `abc` is the empty string.
- `empty` and `empty-zero`: copies of the empty string are the empty string.
- `many`: 100 copies of `ab` are 200 characters. The expected value in each test is one literal, so the test does not use the repeat of the language.
- `astral`: 3 copies of an emoji above U+FFFF are 3 code points, 6 UTF-16 units and 12 UTF-8 bytes.
- `combining`: `e` followed by U+0301, 2 times, is 2 graphemes and 4 code points. The repeat copies code units, so each copy stays whole.

## Pitfalls

- **Python repeats the references.** A negative count gives the empty string with no error. For a list, "items in the sequence s are not copied; they are referenced multiple times"[^1], so the 3 rows are one list:

```python run id=python-repeat-references
print(repr("ab" * -1))
rows = [[]] * 3
rows[0].append(1)
print(rows)
```

```output for=python-repeat-references
''
[[1], [1], [1]]
```

- **JavaScript changes the count to an integer.** A count of 3.5 gives 3 copies "(count will be converted to integer)"[^2]. A `RangeError` is thrown "if count is negative or if count overflows maximum string length"[^2]:

```javascript run id=javascript-repeat-count
console.log("ab".repeat(2.9));
try {
  "ab".repeat(-1);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-repeat-count
abab
RangeError
```

- **Java repeat throws for a negative count.** It throws "IllegalArgumentException - if the count is negative"[^3]. Check the count first if it comes from a user:

```java run id=java-repeat-negative
public class Main {
  public static void main(String[] args) {
    System.out.println("ab".repeat(3));
    try {
      "ab".repeat(-1);
    } catch (IllegalArgumentException e) {
      System.out.println("IllegalArgumentException");
    }
  }
}
```

```output for=java-repeat-negative
ababab
IllegalArgumentException
```

- **C# has a constructor only for one char.** `new string('-', 5)` makes a string "to the value indicated by a specified Unicode character repeated a specified number of times"[^7]. For a string of more than one char, use `Enumerable.Repeat` or a `StringBuilder`:

```csharp run id=csharp-repeat-char
using System;
using System.Linq;

Console.WriteLine(new string('-', 5));
Console.WriteLine(string.Concat(Enumerable.Repeat("ab", 3)));
```

```output for=csharp-repeat-char
-----
ababab
```

- **Go strings.Repeat panics.** "It panics if count is negative or if the result of (len(s) * count) overflows"[^5]:

```go run id=go-repeat-panic
package main

import (
	"fmt"
	"strings"
)

func main() {
	defer func() {
		fmt.Println("panic:", recover())
	}()
	fmt.Println(strings.Repeat("ab", 3))
	fmt.Println(strings.Repeat("ab", -1))
}
```

```output for=go-repeat-panic
ababab
panic: strings: negative Repeat count
```

- **Rust repeat panics on overflow.** "This function will panic if the capacity would overflow"[^6]. `checked_mul` gives `None` if the length is too large, so the code can check before the call:

```rust run id=rust-repeat-overflow
fn main() {
    let s = "ab";
    println!("{}", s.repeat(3));
    println!("{:?}", s.len().checked_mul(usize::MAX));
}
```

```output for=rust-repeat-overflow
ababab
None
```

- **C size_t wraps on overflow.** `len * n` is an unsigned product, so it can wrap to a small number, and then `malloc` gives too little memory. Compare `n` with `(SIZE_MAX - 1) / len` first:

```c run id=c-repeat-overflow
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t len = 2;
    size_t n = SIZE_MAX / 2 + 1;
    printf("%zu\n", len * n);
    printf("%d\n", n > (SIZE_MAX - 1) / len);
    return 0;
}
```

```output for=c-repeat-overflow
0
1
```

- **C++ std::string(n, c) is for one char.** The constructor makes "a string with count copies of character ch"[^8]. For a string, append it in a loop:

```cpp run id=cpp-repeat-char
#include <iostream>
#include <string>

int main() {
    std::cout << std::string(5, '-') << "\n";
    std::string out;
    for (int i = 0; i < 3; i++) {
        out += "ab";
    }
    std::cout << out << "\n";
}
```

```output for=cpp-repeat-char
-----
ababab
```

- **Bash `{1..$n}` does not use the variable.** "Brace expansion is performed before any other expansions"[^9], so `$n` is not yet a number, and the text stays as it is. Use a loop with `((`:

```bash run id=bash-brace-variable
n=3
echo {1..$n}
out=''
for ((i = 0; i < n; i++)); do
  out+='ab'
done
echo "$out"
```

```output for=bash-brace-variable
{1..3}
ababab
```

## Related tasks

- [[build-a-string-efficiently]]: reserve the memory before many appends.
- [[concatenate-strings]]: join two strings, the step that a repeat does `n` times.
- [[join-a-list-of-strings]]: put a separator between the copies.

## References
