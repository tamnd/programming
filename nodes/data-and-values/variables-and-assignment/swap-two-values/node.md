---
tag: S47ZZ4
kind: task
slug: swap-two-values
name: Swap two values
aliases: [swap, swap two variables, exchange two values, generic swap, std::swap, mem::swap, tuple swap, XOR swap, Generic swap]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  Exchange the values of two variables or two items of a list: with a temporary variable, with a tuple assignment, or with std::swap or mem::swap. The XOR trick fails when both are the same place.
contract: contract.yaml
category: basics
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Generic-swap/00-TASK.txt
  title: Generic swap, Rosetta Code (the RosettaCodeData mirror)
  locator: Task description
- url: https://en.wikipedia.org/wiki/Swap_(computer_programming)
  title: Swap (computer programming), Wikipedia
  locator: Using a temporary variable; Parallel assignment; Swap through addition and subtraction
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.16 Evaluation order
- url: https://en.cppreference.com/w/cpp/algorithm/swap.html
  title: std::swap, cppreference.com (C++)
  locator: Introduction
- url: https://doc.rust-lang.org/std/mem/fn.swap.html
  title: swap in std::mem, The Rust Standard Library
  locator: Description
- url: https://en.wikipedia.org/wiki/XOR_swap_algorithm
  title: XOR swap algorithm, Wikipedia
  locator: Proof of correctness; Reasons for avoidance in practice
---

## Task

The Rosetta Code task asks for "a generic swap function or operator which exchanges the values of two variables (or, more generally, any two storage places that can be assigned), regardless of their types"[^1]. After a swap of `a = 1` and `b = 2`, `a` is 2 and `b` is 1.

The function `swap_items(xs, i, j)` swaps two storage places of one list: the items at the indexes `i` and `j`. It returns a new list and does not change `xs`. When `i` equals `j`, the result equals `xs`. The C solution swaps the items in place, because C has no simple way to return a new array.

## Solutions

There are three common ways:

- **A temporary variable.** "The simplest and probably most widely used method to swap two variables is to use a third temporary variable"[^2]: `tmp = a; a = b; b = tmp`. This works in all languages and for all types.
- **A tuple assignment.** `a, b = b, a` assigns two values at once. "This is shorthand for an operation involving an intermediate data structure: in Python and Julia, a tuple; in Ruby, an array."[^2] The right side is computed first: "while evaluating an assignment, the right-hand side is evaluated before the left-hand side"[^3].
- **A library function.** C++ `std::swap` "Exchanges the given values."[^4] Rust `std::mem::swap` "Swaps the values at two mutable locations, without deinitializing either one."[^5] For two items of one slice, Rust has `slice::swap(i, j)`.

| Language | Swap of two items | Copy of the list |
|---|---|---|
| Python | `out[i], out[j] = out[j], out[i]` | `list(xs)` |
| JavaScript, TypeScript | `[out[i], out[j]] = [out[j], out[i]]` | `[...xs]` |
| Java | a temporary variable | `xs.clone()` |
| C# | `(copy[i], copy[j]) = (copy[j], copy[i])` | `(long[])xs.Clone()` |
| Go | `out[i], out[j] = out[j], out[i]` | `slices.Clone(xs)` |
| Rust | `out.swap(i, j)` | `xs.to_vec()` |
| C | a temporary variable, in place | none |
| C++ | `std::swap(xs[i], xs[j])` | the parameter, which is passed by value |
| Bash | a temporary variable in an array | `local -a xs=("$@")` |

In Python, the tuple swap of two variables is one line:

```python run id=python-tuple-swap
a, b = 1, 2
a, b = b, a
print(a, b)

xs = [1, 2, 3, 4]
xs[0], xs[3] = xs[3], xs[0]
print(xs)
```

```output for=python-tuple-swap
2 1
[4, 2, 3, 1]
```

In Rust, `std::mem::swap(&mut out[i], &mut out[j])` does not compile, because it needs two mutable borrows of one slice at the same time. `slice::swap` does the same job with two indexes.

## Edge cases

- **The same index.** `swap_items([1, 2, 3], 1, 1)` gives `[1, 2, 3]`. A list with one item has only this case.
- **The higher index first.** The order of `i` and `j` does not matter: `(3, 0)` gives the same list as `(0, 3)`.
- **Equal values.** `[4, 4]` stays `[4, 4]`.
- **Negative values and the limits of 32 bits.** `[2147483647, -2147483648]` gives `[-2147483648, 2147483647]`. A swap that copies values cannot overflow.

## Pitfalls

- **Two assignments without a temporary variable.** `a = b; b = a` loses the first value of `a`: both variables get the value of `b`.
- **The XOR swap.** `x ^= y; y ^= x; x ^= y` swaps two integers with no temporary variable. But "the algorithm fails if x and y use the same storage location, since the value stored in that location will be zeroed out by the first XOR instruction"[^6]:

```python wrong id=python-xor-same-place
def xor_swap(xs, i, j):
    xs[i] ^= xs[j]
    xs[j] ^= xs[i]
    xs[i] ^= xs[j]


xs = [1, 2, 3]
xor_swap(xs, 0, 2)
print(xs)
xor_swap(xs, 1, 1)
print(xs)
```

```output for=python-xor-same-place
[3, 2, 1]
[3, 0, 1]
```

The trick is also not faster: "On modern CPU architectures, the XOR technique can be slower than using a temporary variable to do swapping."[^6] Use a temporary variable, a tuple assignment or the library function.

- **The swap by addition.** `a = a + b; b = a - b; a = a - b` has the same problem with one place, and more: "When swapping variables of a fixed size, arithmetic overflow becomes an issue."[^2] With 32-bit integers, `2147483647 + 1` overflows.
- **A function that gets copies.** In Python, Java and JavaScript, a function gets the values of its arguments. A function `swap(a, b)` that swaps its parameters does not change the variables of the caller:

```python wrong id=python-swap-parameters
def swap(a, b):
    a, b = b, a


x, y = 1, 2
swap(x, y)
print(x, y)
```

```output for=python-swap-parameters
1 2
```

Swap the variables where they are, or swap two items of a list or the fields of an object that the function gets. C and C++ pass pointers or references: `void swap(int *a, int *b)` and `std::swap(T& a, T& b)`.

- **A line before `[a, b] = [b, a]` in JavaScript.** Without a semicolon at the end of the line before, JavaScript reads `[a, b]` as an index into the value on that line.

## Related tasks

- [[destructuring-assignment]]: the tuple and array assignment that swaps in one line.
- [[assignment]]: why the order of two assignments matters.
- [[declare-a-variable]]: the temporary variable.
- [[pointers-and-references]]: how a function can change the variables of its caller.

## References
