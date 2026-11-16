---
tag: 7XDC0M
kind: task
slug: use-a-tuple
name: Use a tuple
aliases: [return two values, return multiple values, pair, unpack a tuple, destructure a tuple, multiple return values]
area: types/algebraic-data-types
level: intro
summary: Return the smallest and the largest number of a list as one tuple, then unpack it into two variables, in each tier 1 language.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, 5.3 Tuples and Sequences
    locator: 5.3 Tuples and Sequences
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
    title: MDN Web Docs, Destructuring
    locator: Array destructuring
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html#the-tuple-type
    title: The Rust Programming Language, 3.2 Data Types
    locator: The Tuple Type
    accessed: 2026-11-16
  - url: https://www.typescriptlang.org/docs/handbook/2/objects.html#tuple-types
    title: TypeScript Handbook, Object Types
    locator: Tuple Types; readonly Tuple Types
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/utility/tuple
    title: cppreference.com, std::tuple
    locator: Description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/cpp/language/structured_binding
    title: cppreference.com, Structured binding declaration
    locator: Binding process
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-tuples
    title: C# reference, Tuple types
    locator: Tuple field names; Tuple assignment and deconstruction; Tuple equality
    accessed: 2026-11-16
  - url: https://openjdk.org/jeps/395
    title: "JEP 395: Records"
    locator: Summary
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/Map.html
    title: Java SE 27 API, Interface Map
    locator: entry(K, V); Unmodifiable Maps
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Return statements; Assignment statements
    accessed: 2026-11-16
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.9.1
    title: ISO/IEC 9899:201x, committee draft N1570 (C11)
    locator: 6.9.1 Function definitions
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual
    locator: 3.2.3 Pipelines
    accessed: 2026-11-16
---

## Task

Return two values from one function as a tuple. The function `min_max(numbers)` takes a list with at least one integer. It returns the pair `(smallest, largest)`. The caller then unpacks the pair into two variables. For example, `min_max([3, 1, 2])` is `(1, 3)`. Each number fits in a signed 32-bit integer, and the function must not change the list.

A tuple groups values by position. A record groups them by name (see [[define-a-record-type]]). For the idea behind tuples, see [[tuple]].

## Solutions

| Language | Result type | Unpack |
|---|---|---|
| Python | `tuple[int, int]`, made by `return lo, hi` | `lo, hi = min_max(numbers)` |
| JavaScript | an array of length 2 | `const [lo, hi] = minMax(numbers)` |
| TypeScript | `readonly [min: number, max: number]` | `const [lo, hi] = minMax(numbers)` |
| Java | `record Pair(int min, int max)` | a record pattern, `instanceof Pair(int lo, int hi)` |
| C# | the value tuple `(int Min, int Max)` | `var (lo, hi) = MinMax.Of(numbers)` |
| Go | two results, `(lo, hi int32)` | `lo, hi := MinMax(numbers)` |
| Rust | `(i32, i32)` | `let (lo, hi) = min_max(&numbers)` |
| C | `struct min_max { int32_t min; int32_t max; }` | read `.min` and `.max` |
| C++ | `std::tuple<std::int32_t, std::int32_t>` | `auto [lo, hi] = min_max(numbers)` |
| Bash | the line `MIN MAX` on standard output | `read -r lo hi < <(min_max ...)` |

- **Make and unpack.** In Python, "A tuple consists of a number of values separated by commas"[^1], so `return lo, hi` makes a tuple without parentheses. The caller unpacks it: "This is called, appropriately enough, sequence unpacking and works for any sequence on the right-hand side."[^1] JavaScript has no tuple type. A function returns an array, and "The destructuring syntax is a JavaScript syntax that makes it possible to unpack values from arrays, or properties from objects, into distinct variables."[^2] Rust uses a pattern with `let`, and the book says: "This is called destructuring because it breaks the single tuple into three parts."[^3]
- **A type for each position.** In Rust, "A tuple is a general way of grouping together a number of values with a variety of types into one compound type."[^3] In TypeScript, "A tuple type is another sort of Array type that knows exactly how many elements it contains, and exactly which types it contains at specific positions."[^4] The solution makes the tuple `readonly`, because "Tuples tend to be created and left un-modified in most code, so annotating types as readonly tuples when possible is a good default."[^4] In C++, "Class template std::tuple is a fixed-size collection of heterogeneous values."[^5] A structured binding unpacks it: it "Binds the specified names to subobjects or elements of the initializer."[^6]
- **C# names the elements.** "The tuples feature provides a concise syntax to group multiple data elements in a lightweight data structure."[^7] The solution names the elements `Min` and `Max`, but "The default names of tuple fields are Item1 , Item2 , Item3 , and so on."[^7] Two tuples compare with `==`: "Tuple types support the == and != operators."[^7]
- **Java uses a record.** A Java method returns one value. A record is a short way to return two values, and "Records can be thought of as nominal tuples."[^8] The names `min` and `max` also tell the reader which value is which. For a quick pair, `Map.entry(k, v)` "Returns an unmodifiable Map.Entry containing the given key and value."[^9]
- **Go returns two results.** A Go function can return several results, and "A tuple assignment assigns the individual elements of a multi-valued operation to a list of variables."[^10] The results cannot be kept together in one variable. To keep them together, define a struct.
- **C returns a struct.** "The return type of a function shall be void or a complete object type other than array type."[^11] A struct is such a type, so the solution returns a small struct by value. No heap memory and no output pointers are necessary.
- **Bash prints the values.** A Bash function returns only an exit status. The solution prints the two numbers on one line, and the caller reads them with `read -r lo hi`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `one`: a list with one number gives that number twice.
- `negative`: all of the numbers are below 0, so a start value of 0 would give a wrong largest number. The solutions start from the first number.
- `all-same`: equal numbers give an equal pair.
- `int32-extremes`: the smallest and the largest 32-bit integers are in the list. A start value such as `-1` or `1 << 30` would be wrong.
- `sorted` and `reversed`: the order of the list does not change the answer.

## Pitfalls

- **Python a tuple with one item needs a comma.** The parentheses alone do not make a tuple: "a tuple with one item is constructed by following a value with a comma (it is not sufficient to enclose a single value in parentheses)."[^1] Also, "Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence."[^1]

```python run id=python-one-item-tuple
a = (5)
b = (5,)
print(type(a).__name__, type(b).__name__)
try:
    lo, hi = (1, 2, 3)
except ValueError as e:
    print(type(e).__name__)
```

```output for=python-one-item-tuple
int tuple
ValueError
```

- **JavaScript a missing position is `undefined`.** If the array is shorter than the pattern, "only the first N variables are assigned values. The values of the remaining variables will be undefined."[^2] No error occurs, and a later sum gives `NaN`. Check the length, or give a default value:

```javascript run id=javascript-missing-position
const [lo, hi] = [5];
console.log(hi, lo + hi);
const [a, b = a] = [5];
console.log(a, b);
```

```output for=javascript-missing-position
undefined NaN
5 5
```

- **TypeScript a tuple is an array at run time.** A tuple type is an Array type[^4], so a tuple type that is not `readonly` has `push`. The type checker accepts `push` on `[number, number]`, and the tuple then has three elements. Use `readonly [number, number]`, which has no `push`:

```typescript run id=typescript-tuple-push
const pair: [number, number] = [1, 3];
pair.push(5);
console.log(pair.length);
const fixed: readonly [number, number] = [1, 3];
console.log(fixed.length);
```

```output for=typescript-tuple-push
3
2
```

- **Java `Map.entry` does not accept null.** The entries "disallow null keys and values. Attempts to create them with null keys or values result in NullPointerException ."[^9] A record accepts null components:

```java run id=java-map-entry-null
import java.util.Map;

public class Main {
  record Pair(String a, String b) {}

  public static void main(String[] args) {
    try {
      Map.entry("a", null);
    } catch (NullPointerException e) {
      System.out.println("NullPointerException");
    }
    System.out.println(new Pair("a", null));
  }
}
```

```output for=java-map-entry-null
NullPointerException
Pair[a=a, b=null]
```

- **C# assignment ignores the element names.** Tuples are assigned by position: "The assignment process ignores the names of tuple fields"[^7]. A tuple with the names in the other order thus gets the values in the wrong fields, with no warning:

```csharp run id=csharp-tuple-names
using System;

(int Min, int Max) pair = (1, 3);
(int Max, int Min) swapped = pair;
Console.WriteLine(swapped.Max + " " + swapped.Min);
```

```output for=csharp-tuple-names
1 3
```

- **C++ `auto [a, b]` binds to a copy.** "A structured binding declaration first introduces a uniquely-named variable (here denoted by e ) to hold the value of the initializer"[^6]. With `auto`, that variable is a copy, so a change to `lo` does not change the tuple. Use `auto &` to change the tuple:

```cpp run id=cpp-binding-copy
#include <iostream>
#include <tuple>

int main() {
    std::tuple<int, int> t{1, 3};
    auto [lo, hi] = t;
    lo = 7;
    std::cout << std::get<0>(t) << '\n';
    auto &[rlo, rhi] = t;
    rlo = 7;
    std::cout << std::get<0>(t) << ' ' << hi + rhi << '\n';
}
```

```output for=cpp-binding-copy
1
7 6
```

- **Bash `read` at the end of a pipe loses the values.** "Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell, which is a separate process"[^12]. The variables that `read` sets are in that subshell, so they are empty after the pipe. Use process substitution:

```bash run id=bash-read-in-pipe
min_max() { echo "1 3"; }
min_max | read -r lo hi
echo "[$lo] [$hi]"
read -r lo hi < <(min_max)
echo "[$lo] [$hi]"
```

```output for=bash-read-in-pipe
[] []
[1] [3]
```

## Related tasks

- [[define-a-record-type]]: name the parts when a tuple is not clear.
- [[define-an-enumeration]]: a type with a fixed set of named values.
- [[use-an-enum-with-data]]: a type whose cases carry values.
- [[use-a-custom-key-in-a-map]]: a tuple as the key of a map.

## References
