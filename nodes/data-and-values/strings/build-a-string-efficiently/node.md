---
tag: ASRJAM
kind: task
slug: build-a-string-efficiently
name: Build a string efficiently
aliases: [string builder, StringBuilder, strings.Builder, append to a string in a loop, string concatenation in a loop, accumulate a string]
area: data-and-values/strings
level: core
summary: Build a long string from many pieces in a loop with a builder, a buffer that grows, or a list and one join, and not with a + that copies the whole result on each step.
contract: contract.yaml
category: text-and-strings
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
  title: The Python Standard Library, Built-in Types
  locator: Common Sequence Operations, note 6
- url: https://docs.python.org/3/library/io.html#io.StringIO
  title: The Python Standard Library, io
  locator: class io.StringIO
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/join
  title: MDN Web Docs, Array.prototype.join()
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StringBuilder.html
  title: Java SE 25 API, java.lang.StringBuilder
  locator: Class description; Constructors
- url: https://learn.microsoft.com/en-us/dotnet/standard/base-types/stringbuilder
  title: .NET fundamentals, Use the StringBuilder class in .NET
- url: https://pkg.go.dev/strings#Builder
  title: Go standard library, package strings
  locator: type Builder
- url: https://doc.rust-lang.org/std/string/struct.String.html
  title: The Rust Standard Library, struct String
  locator: with_capacity; impl Add<&str> for String
- url: https://en.cppreference.com/w/c/string/byte/strcat
  title: cppreference.com, strcat, strcat_s
- url: https://en.cppreference.com/w/c/memory/realloc
  title: cppreference.com, realloc
- url: https://en.cppreference.com/w/cpp/string/basic_string/push_back
  title: cppreference.com, std::basic_string push_back
  locator: Complexity
- url: https://en.cppreference.com/w/cpp/string/basic_string/reserve
  title: cppreference.com, std::basic_string reserve
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual, on the site of the Bash maintainer Chet Ramey
  locator: Arrays
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/strings/common-tasks/concatenate
  title: C# fundamentals, How to concatenate multiple strings
---

## Task

Build a long string from many small pieces. The function `number_line(n)` returns the numbers 1 to `n` with a comma between each two, so `number_line(5)` is `"1,2,3,4,5"` and `number_line(0)` is the empty string. The code makes the pieces one at a time in a loop.

A loop of `s = s + piece` on an immutable string copies the whole result on each step. For `n` pieces, the copies add up to O(n²) characters. A builder keeps one buffer with free space at the end, so an append copies only the new piece, and the buffer grows by a factor when it is full. Then the cost of all the appends is O(n). A list of pieces and one join at the end has the same cost.

## Solutions

| Language | Tool |
|---|---|
| Python | a `list` and `str.join`, or `io.StringIO` |
| JavaScript, TypeScript | an array and `join` |
| Java | `StringBuilder` |
| C# | `StringBuilder` |
| Go | `strings.Builder` |
| Rust | `String` with `push`, `push_str` and `write!` |
| C | a buffer that keeps its length and grows with `realloc` |
| C++ | `std::string` with `+=` |
| Bash | an array and `"${parts[*]}"` |

- **Python.** "Concatenating immutable sequences always results in a new object. This means that building up a sequence by repeated concatenation will have a quadratic runtime cost in the total sequence length"[^1]. The documentation says that "you can build a list and use str.join() at the end"[^1], and the code does that. `io.StringIO` is "A text stream using an in-memory text buffer"[^2], and it is the other choice.
- **JavaScript and TypeScript.** `join` "returns a new string that is the concatenation of all elements in this array, separated by commas or a specified separator string"[^3]. The code pushes each number to an array and joins once.
- **Java.** "Every string builder has a capacity. As long as the length of the character sequence contained in the string builder does not exceed the capacity, it is not necessary to allocate a new internal buffer. If the internal buffer overflows, it is automatically made larger"[^4]. `append(int)` writes the digits, and `toString` makes the result.
- **C#.** "The String object is immutable"[^5], and the .NET documentation says that "using the StringBuilder class can boost performance when concatenating many strings together in a loop"[^5]. The code appends each number with `ToString(CultureInfo.InvariantCulture)`, so the culture cannot change the digits.
- **Go.** "A Builder is used to efficiently build a string using Builder.Write methods. It minimizes memory copying. The zero value is ready to use"[^6]. The code calls `WriteByte` for the comma and `WriteString` for the digits.
- **Rust.** A `String` is a builder. `String::with_capacity` "creates an empty String, but one with an initial buffer that can hold at least capacity bytes"[^7]. The code uses `write!` from `std::fmt::Write`, which formats the number into the buffer and makes no temporary `String`.
- **C.** C has no builder. `strcat` finds the end of the destination on each call, because "The character src[0] replaces the null terminator at the end of dest"[^8], so a loop of `strcat` calls reads the result again on each step. The code keeps the length and the capacity in a struct, writes at the end with `memcpy`, and doubles the capacity with `realloc` when the buffer is full[^9]. The caller frees the result.
- **C++.** `std::string` grows like a builder. The complexity of `push_back` is "Amortized constant"[^10], and `reserve` can make the buffer large enough before the loop[^11]. The code appends with `+=` and `std::to_string`.
- **Bash.** "If the word is double-quoted, ${name[*]} expands to a single word with the value of each array member separated by the first character of the IFS variable"[^12]. The function collects the numbers in an array, sets a local `IFS` to a comma and prints `"${parts[*]}"`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: the loop does not run, and the result is the empty string. A join of an empty list gives the empty string in every language.
- `one`: one number and no comma. Code that writes a comma after each number and does not remove the last one fails here.
- `nine`, `ten`, `hundred` and `hundred-and-one`: the numbers get one more digit at 10 and at 100, so the result grows faster after each of these points.
- `thousand` and `large`: `number_line(1000)` has 3892 characters. Each test file also checks `number_line(100000)`, which has 588894 characters, by its length and its two ends. A solution that copies the whole result on each step copies about 29 thousand million characters for it and fails the time limit.

## Pitfalls

- **Python `join` takes only strings.** It raises `TypeError` for an `int`, so convert each item first. `io.StringIO` collects the pieces, and `getvalue` returns "a str containing the entire contents of the buffer"[^2]:

```python run id=python-join-needs-strings
import io

nums = [1, 2, 3]
try:
    print(",".join(nums))
except TypeError as e:
    print("TypeError:", e)
print(",".join(map(str, nums)))

buf = io.StringIO()
for n in nums:
    buf.write(f"{n};")
print(buf.getvalue())
```

```output for=python-join-needs-strings
TypeError: sequence item 0: expected str instance, int found
1,2,3
1;2;3;
```

- **JavaScript `join` changes some items.** "If an element is undefined or null, it is converted to an empty string"[^3], and an array item is converted with its own `join`, so it brings its own commas:

```javascript run id=javascript-join-null-and-nested
console.log([1, null, 3, undefined].join());
console.log([1, [2, 3], 4].join(";"));
console.log(["a"].join("-"));
console.log([].join("-") === "");
```

```output for=javascript-join-null-and-nested
1,,3,
1;2,3;4
a
true
```

- **Java `char` values are numbers.** `'a' + 'b'` is an addition of two `char` values, so `append` gets the `int` 195. `new StringBuilder('x')` calls the constructor that takes an `int`, and it "Constructs a string builder with no characters in it and an initial capacity specified by the capacity argument"[^4], so the builder is empty and its capacity is 120, the code of `x`:

```java run id=java-builder-char-pitfalls
public class Main {
  public static void main(String[] args) {
    StringBuilder a = new StringBuilder();
    a.append('a' + 'b');
    a.append(',').append("" + 'a' + 'b');
    System.out.println(a);

    StringBuilder b = new StringBuilder('x');
    System.out.println("[" + b + "] capacity " + b.capacity());

    StringBuilder c = new StringBuilder(1000);
    System.out.println("capacity " + c.capacity() + ", length " + c.length());
  }
}
```

```output for=java-builder-char-pitfalls
195,ab
[] capacity 120
capacity 1000, length 0
```

- **C# `+=` in a loop.** "Each + or += operation creates a new string, because strings are immutable"[^13]. Use one `StringBuilder` for the whole loop, and call `ToString` once at the end.
- **Go `strings.Builder` must not be copied.** The documentation says "Do not copy a non-zero Builder"[^6]. A write to a copy panics:

```go run id=go-builder-copy-panics
package main

import (
	"fmt"
	"strings"
)

func main() {
	var a strings.Builder
	a.WriteString("abc")
	b := a // a copy of a Builder that is not empty
	defer func() {
		fmt.Println("panic:", recover())
	}()
	fmt.Println(b.String())
	b.WriteString("def")
}
```

```output for=go-builder-copy-panics
abc
panic: strings: illegal use of non-zero Builder copied by value
```

- **C `strcat` in a loop reads the result again on each step.** For the 1000 numbers, the loop of `strcat` calls reads 1893996 bytes to find the end of a result of 3892 bytes. A pointer to the end reads none:

```c run id=c-strcat-scans-again
#include <stdio.h>
#include <string.h>

static char buf[4096];

int main(void) {
    // strcat finds the end of buf on each call, so it reads all the bytes
    // that are already there. A pointer to the end does not.
    size_t scanned = 0;
    buf[0] = '\0';
    for (int i = 1; i <= 1000; i++) {
        char d[16];
        snprintf(d, sizeof d, i > 1 ? ",%d" : "%d", i);
        scanned += strlen(buf);
        strcat(buf, d);
    }
    printf("strcat: %zu bytes, %zu bytes read to find the end\n", strlen(buf), scanned);

    char *end = buf;
    for (int i = 1; i <= 1000; i++) {
        end += snprintf(end, sizeof buf - (size_t)(end - buf), i > 1 ? ",%d" : "%d", i);
    }
    printf("end pointer: %zu bytes, 0 bytes read to find the end\n", (size_t)(end - buf));
}
```

```output for=c-strcat-scans-again
strcat: 3892 bytes, 1893996 bytes read to find the end
end pointer: 3892 bytes, 0 bytes read to find the end
```

- **Rust: reserve when you know the size.** With a capacity that is large enough, the appends are "all done without reallocating"[^7]. Without it, a push "may make the string reallocate"[^7] when the buffer is full:

```rust run id=rust-with-capacity
use std::fmt::Write;

fn main() {
    let n = 1000;
    let mut s = String::with_capacity(3892);
    let cap = s.capacity();
    for i in 1..=n {
        if i > 1 {
            s.push(',');
        }
        write!(s, "{i}").unwrap();
    }
    println!("length {}, enough capacity at the start: {}", s.len(), cap >= 3892);
    println!("reallocated: {}", s.capacity() != cap);
}
```

```output for=rust-with-capacity
length 3892, enough capacity at the start: true
reallocated: false
```

- **Bash `"${parts[*]}"` uses only the first character of `IFS`.** With `IFS=', '`, the separator is a comma with no space. When `IFS` is not set, the separator is a space. Set `IFS` in a function with `local`, or in a subshell, so the change does not affect the rest of the script:

```bash run id=bash-array-join-ifs
parts=(1 2 3)
IFS=', '
echo "${parts[*]}"
echo "${parts[@]}"
unset IFS
echo "${parts[*]}"
(IFS=-; echo "${parts[*]}")
echo "${parts[*]}"
```

```output for=bash-array-join-ifs
1,2,3
1 2 3
1 2 3
1-2-3
1 2 3
```

## Related tasks

- [[concatenate-strings]]: one concatenation of two strings, and the rules for a string and a number.
- [[split-a-string]]: a split takes a line like the result of this task back to its pieces.
- [[find-the-length-of-a-string]]: the length of the result in bytes, code units and code points.

## References
