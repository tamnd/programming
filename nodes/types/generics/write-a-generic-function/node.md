---
tag: ZHH4SB
kind: task
slug: write-a-generic-function
name: Write a generic function
aliases: [generic method, function with a type parameter, bounded type parameter, type constraint, count the items greater than a value]
area: types/generics
level: core
summary: >-
  Write count_greater(items, limit), a function with a type parameter T and the constraint that values of T can be ordered, and use it to count the items of a list that are greater than a limit.
contract: contract.yaml
category: functions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [parametric-polymorphism, polymorphism, interfaces, write-a-higher-order-function, find-the-largest-element]
sources:
- url: https://docs.oracle.com/javase/tutorial/java/generics/boundedTypeParams.html
  title: Generic Methods and Bounded Type Parameters, The Java Tutorials
  locator: Whole page
- url: https://en.wikipedia.org/wiki/Generic_programming
  title: Generic programming, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3.14/library/typing.html
  title: typing, Python 3.14 documentation
  locator: TypeVar; Protocol
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Less_than
  title: Less than (<), MDN Web Docs
  locator: Description
- url: https://www.typescriptlang.org/docs/handbook/2/generics.html
  title: Generics, The TypeScript Handbook
  locator: Generic Constraints
- url: https://docs.oracle.com/javase/tutorial/java/generics/methods.html
  title: Generic Methods, The Java Tutorials
  locator: Whole page
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/generics/constraints-on-type-parameters
  title: Constraints on type parameters, C# programming guide
  locator: Introduction; Constraints on reference types
- url: https://en.cppreference.com/w/c/algorithm/qsort.html
  title: qsort, qsort_s, C reference, cppreference.com
  locator: Description; Example
- url: https://en.cppreference.com/w/cpp/language/constraints.html
  title: Constraints and concepts, C++ reference, cppreference.com
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/concepts/totally_ordered.html
  title: std::totally_ordered, C++ reference, cppreference.com
  locator: Description
- url: https://go.dev/doc/tutorial/generics
  title: "Tutorial: Getting started with generics, The Go Programming Language"
  locator: Add a generic function to handle multiple types
- url: https://pkg.go.dev/cmp
  title: Package cmp, Go standard library
  locator: type Ordered
- url: https://doc.rust-lang.org/book/ch10-01-syntax.html
  title: Generic Data Types, The Rust Programming Language
  locator: In Function Definitions
---

## Task

Write a generic function `count_greater(items, limit)`. It has a type parameter `T`. It takes a list of values of type `T` and one value `limit` of type `T`, and it returns how many items are greater than `limit`. An item that is equal to `limit` does not count. For example, `count_greater([34, 50, 25, 100, 65], 50)` is 2, because only 100 and 65 are greater than 50.

The body compares two values of type `T`, so `T` must be a type whose values can be ordered. Give the type parameter a constraint that says so. The Java tutorial uses this function to show why a constraint is necessary: without one, the method "does not compile because the greater than operator (>) applies only to primitive types"[^1]. The fix is "a type parameter bounded by the Comparable<T> interface"[^1].

The entry of the contract is `count_greater(items, limit)` with a list of integers. The list has from 0 to 10000 items, and each item and the limit fit in a signed 32-bit integer. The native tests also call the function with strings, so each solution must work for more than one type.

A generic function is the basic unit of generic programming, where "algorithms are written in terms of data types to-be-specified-later that are then instantiated when needed for specific types provided as parameters"[^2]. [[parametric-polymorphism]] explains the idea, and [[polymorphism]] compares it with the other kinds of polymorphism.

## Solutions

| Language | The type parameter and its constraint | The comparison |
|---|---|---|
| Python | `[T: Comparable]`, where `Comparable` is a `Protocol` with `__lt__` | `limit < item` |
| JavaScript | no type parameter; the function takes any values | `item > limit` |
| TypeScript | `<T extends number \| string \| bigint>` | `item > limit` |
| Java | `<T extends Comparable<T>>` | `item.compareTo(limit) > 0` |
| C# | `<T>` with `where T : IComparable<T>` | `item.CompareTo(limit) > 0` |
| C | no type parameter; `const void *` and the size of an item | a comparison function that the caller passes |
| C++ | `template <std::totally_ordered T>` | `item > limit` |
| Go | `[T cmp.Ordered]` | `item > limit` |
| Rust | `<T: PartialOrd>` | `item > limit` |

- **Python.** The function uses the type parameter syntax with an upper bound. The typing documentation shows the form `class StrSequence[S: str]`, where S "is a TypeVar with a `str` upper bound"[^3]. The bound "can be a concrete type, abstract type (ABC or Protocol), or even a union of types"[^3]. The solution defines a `Protocol` with `__lt__`, so any type with `<` fits, because protocols give "structural subtyping (static duck-typing)"[^3]. Python itself does not check the bound when the program runs: "Type variables exist primarily for the benefit of static type checkers"[^3].
- **JavaScript.** The language has no type parameters, so every function is generic. `>` works on numbers and on strings. "If both values are strings, they are compared as strings, based on the values of the UTF-16 code units (not Unicode code points) they contain"[^4]. Nothing stops a call with a mix of types, which the pitfalls below show.
- **TypeScript.** A constraint uses `extends`. The handbook says that to limit a type parameter, "we must list our requirement as a constraint on what Type can be"[^5]. The solution limits `T` to the primitive types that `>` orders. The types are erased when the program runs, so the JavaScript that runs is the same as the JavaScript solution.
- **Java.** "Generic methods are methods that introduce their own type parameters"[^6]. The solution is the tutorial's method with a `List` in place of an array: `<T extends Comparable<T>>` lets the body call `compareTo`, because "You cannot use the > operator to compare objects"[^1]. `Integer` and `String` both implement `Comparable`.
- **C#.** "Constraints inform the compiler about the capabilities a type argument must have"[^7]. The guide names the constraint for an ordering: "apply the where T : IEquatable<T> or where T : IComparable<T> constraint"[^7]. `int` and `string` implement `IComparable<T>`. The solution compares strings with `CompareTo`, which uses the rules of the current culture, so the tests use only lowercase ASCII words.
- **C.** C has no type parameters. The standard function `qsort` shows the usual way to write code for any type: it takes a pointer to the items, the count, the size of one item and a comparison function. "Function pointed to by comp is used for object comparison"[^8]. The solution has the same shape, and `count_greater_int` is the entry for `int`.
- **C++.** A function template takes a type parameter, and a concept limits it. "Each concept is a predicate, evaluated at compile time, and becomes a part of the interface of a template where it is used as a constraint"[^9]. The solution uses `std::totally_ordered`, which "specifies that the comparison operators ==,!=,<,>,<=,>= on a type yield results consistent with a strict total order on the type"[^10].
- **Go.** "Each type parameter has a type constraint that acts as a kind of meta-type for the type parameter"[^11]. The solution uses `cmp.Ordered`, which "is a constraint that permits any ordered type: any type that supports the operators < <= >= >"[^12].
- **Rust.** The Rust book builds a generic `largest` function, and the first version does not compile. The fix is to "restrict the types valid for T to only those that implement PartialOrd"[^13]. The solution uses the same bound. The function takes `&[T]` and `&T`, so it does not need to copy or own the items.

### One function, several types

The Python solution works for any type with `<`. Here the same function counts integers, strings and dates:

```python run id=python-several-types
from collections.abc import Iterable
from datetime import date
from typing import Protocol, Self


class Comparable(Protocol):
    def __lt__(self, other: Self, /) -> bool: ...


def count_greater[T: Comparable](items: Iterable[T], limit: T) -> int:
    return sum(1 for item in items if limit < item)


print(count_greater([34, 50, 25, 100, 65], 50))
print(count_greater(["pear", "apple", "plum"], "orange"))
print(count_greater([date(2024, 1, 5), date(2025, 3, 1)], date(2024, 12, 31)))
```

```output for=python-several-types
2
2
1
```

`"pear"` and `"plum"` come after `"orange"` in the order of the code points, and one of the two dates is after the end of 2024.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: an empty list has no item greater than the limit, so the result is 0.
- `equal-not-counted`: `[5, 5, 5]` with the limit 5 gives 0, because the comparison is strict.
- `duplicates`: each copy of a value counts, so `[7, 1, 7, 7]` with the limit 6 gives 3.
- `negative`: `[-3, -1, 0, 2]` with the limit -2 gives 3.
- `min-limit`: with the limit -2147483648, the smallest 32-bit integer, every other item counts. A comparison that subtracts the two values overflows here.
- `max-limit`: no 32-bit integer is greater than 2147483647, so the result is 0.
- `single`: `[1]` with the limit 0 gives 1.

## Pitfalls

### A type parameter with no constraint

Without a constraint, the compiler knows nothing about `T`, so it does not allow `>`. The Rust book shows the error for its `largest` function[^13]:

```rust fails expect=compile id=rust-no-bound
fn count_greater<T>(items: &[T], limit: &T) -> usize {
    items.iter().filter(|item| *item > limit).count()
}

fn main() {
    println!("{}", count_greater(&[1, 5, 9], &4));
}
```

The error is E0369: the binary operation `>` cannot be applied to `&T`. Add the bound `T: PartialOrd`. Java gives the same kind of error for `>` on a `T`, and C# and Go do too, because the operator is not in the constraint.

### Numbers kept as strings

In JavaScript, the function takes values of any type. When the items and the limit are strings, `>` compares them as strings[^4], so `"10"` is less than `"9"`:

```javascript wrong id=javascript-strings-of-digits
function countGreater(items, limit) {
  return items.filter((item) => item > limit).length;
}

console.log(countGreater(["10", "9", "100"], "9"));
console.log(countGreater([10, 9, 100], 9));
```

```output for=javascript-strings-of-digits
0
2
```

Convert the input to numbers before the call. In TypeScript, the constraint does not stop this case either, because `string` is one of the allowed types. A call with a number list and a string limit is a type error in TypeScript, because both must have the same type `T`.

### NaN in a list of floats

`cmp.Ordered` in Go and `PartialOrd` in Rust allow floating-point types. "Note that floating-point types may contain NaN ("not-a-number") values. An operator such as == or < will always report false when comparing a NaN value with any other value, NaN or not."[^12] So a NaN item never counts, and a NaN limit gives 0:

```go wrong id=go-nan
package main

import (
	"cmp"
	"fmt"
	"math"
)

func CountGreater[T cmp.Ordered](items []T, limit T) int {
	n := 0
	for _, item := range items {
		if item > limit {
			n++
		}
	}
	return n
}

func main() {
	nan := math.NaN()
	fmt.Println(CountGreater([]float64{1, nan, 3}, 0))
	fmt.Println(CountGreater([]float64{1, 2, 3}, nan))
}
```

```output for=go-nan
2
0
```

If the data can hold NaN, decide what it means, and test for it with `math.IsNaN` before the call. In C++, `std::totally_ordered` requires a total order, which NaN breaks, so a call with NaN values breaks the requirement of the concept[^10].

### A comparison function that subtracts

In C, the comparison function returns a negative number, zero or a positive number. `return a - b;` is short, but it can overflow. The cppreference example of `qsort` marks it as an "erroneous shortcut: undefined behavior in case of integer overflow, such as with INT_MIN here"[^8]. Compare with `<` and `>`, as the solution does. The case `min-limit` catches the error.

## Related tasks

- [[find-the-largest-element]]: another function that compares the items of a list.
- [[sort-a-list]]: sort the items with the same ordering.
- [[write-a-higher-order-function]]: pass the comparison as a function, as the C solution does.
- [[compare-strings]]: the rules for the order of strings.

## References
