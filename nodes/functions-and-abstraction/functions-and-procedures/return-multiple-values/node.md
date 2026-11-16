---
tag: 111F4Y
kind: task
slug: return-multiple-values
name: Return multiple values
aliases: [multiple return values, return a tuple, return two values, divmod, out parameters, tuple return]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  Return the quotient and the remainder of a floor division from one function, as a tuple, a record, a struct or out parameters, and unpack the two results at the call.
contract: contract.yaml
category: functions
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/library/functions.html#divmod
  title: Built-in Functions, The Python Standard Library
  locator: divmod()
- url: https://docs.python.org/3/reference/expressions.html#binary-arithmetic-operations
  title: Expressions, The Python Language Reference
  locator: 6.7 Binary arithmetic operations
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Math.html
  title: Class Math, Java SE 25 API
  locator: floorDiv(int, int); floorMod(int, int)
- url: https://en.wikipedia.org/wiki/Modulo
  title: Modulo, Wikipedia
  locator: Variants of the definition
- url: https://docs.python.org/3/tutorial/datastructures.html#tuples-and-sequences
  title: Data Structures, The Python Tutorial
  locator: 5.3 Tuples and Sequences
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
  title: Destructuring, MDN Web Docs JavaScript Reference
  locator: the introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
  title: Remainder (%), MDN Web Docs JavaScript Reference
  locator: the introduction
- url: https://www.typescriptlang.org/docs/handbook/2/objects.html#tuple-types
  title: Object Types, The TypeScript Handbook
  locator: Tuple Types
- url: https://docs.oracle.com/en/java/javase/25/language/records.html
  title: Record Classes, Java Platform, Standard Edition Java Language Updates
  locator: the introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/value-tuples
  title: Tuple types, C# reference
  locator: Use cases of tuples
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters
  title: Method parameters and modifiers, C# reference
  locator: out parameter modifier
- url: https://go.dev/doc/effective_go#multiple-returns
  title: Effective Go
  locator: Multiple return values
- url: https://en.cppreference.com/w/c/numeric/math/div.html
  title: div, ldiv, lldiv, imaxdiv, C reference, cppreference.com
  locator: the introduction; Notes
- url: https://en.cppreference.com/w/c/language/operator_arithmetic.html
  title: Arithmetic operators, C reference, cppreference.com
  locator: Division
- url: https://en.cppreference.com/w/cpp/language/structured_binding.html
  title: Structured binding declaration, C++ reference, cppreference.com
  locator: the introduction
- url: https://en.cppreference.com/w/cpp/utility/pair.html
  title: std::pair, C++ reference, cppreference.com
  locator: the introduction
- url: https://go.dev/ref/spec#Arithmetic_operators
  title: The Go Programming Language Specification
  locator: Integer operators
- url: https://doc.rust-lang.org/book/ch04-01-what-is-ownership.html
  title: What Is Ownership?, The Rust Programming Language
  locator: Return Values and Scope
- url: https://doc.rust-lang.org/book/ch03-02-data-types.html
  title: Data Types, The Rust Programming Language
  locator: Numeric Operations; The Tuple Type
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, Bash Reference Manual
  locator: read
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Comma_operator
  title: Comma operator (,), MDN Web Docs JavaScript Reference
  locator: the introduction
---

## Task

Define a function `div_mod(a, b)` that returns two results, the quotient and the remainder of the division of `a` by `b`. The caller gets both results from one call and unpacks them into two variables.

The division is floor division, as in the Python function `divmod`: "For integers, the result is the same as (a // b, a % b)."[^1] For `//`, "the result is that of mathematical division with the ‘floor’ function applied to the result"[^2], and "The modulo operator always yields a result with the same sign as its second operand"[^2]. Java describes the same remainder: it "has the same sign as the divisor y or is zero, and is in the range of -abs(y) < r < +abs(y)."[^3]

For example, `div_mod(17, 5)` is (3, 2), `div_mod(-17, 5)` is (-4, 3) and `div_mod(17, -5)` is (-4, -3). The arguments and the results are 32-bit signed integers. `b` is not 0, and the pair `a = -2147483648`, `b = -1` is not allowed, because its quotient 2147483648 does not fit. Java says that for this pair "integer overflow occurs and the result is equal to Integer.MIN_VALUE."[^3]

Many languages round the quotient of `/` toward zero. "Despite the mathematical elegance of Knuth's floored division and Euclidean division, it is generally much more common to find a truncated division-based modulo in programming languages."[^4] So some solutions change the result of `/` and `%`: when the remainder is not 0 and its sign is not the sign of `b`, they subtract 1 from the quotient and add `b` to the remainder.

In most of the solutions below, the function returns one value that holds both results, such as a [[tuple]] or a record, and the caller takes it apart again. [[return-value]] describes the result of a function, and [[destructure-a-value]] shows more ways to unpack a value.

## Solutions

| Language | The two results | The unpack at the call |
|---|---|---|
| Python | a tuple, `return a // b, a % b` | `q, r = div_mod(a, b)` |
| JavaScript | an array, `return [q, r]` | `const [q, r] = divMod(a, b)` |
| TypeScript | the tuple type `[number, number]` | `const [q, r] = divMod(a, b)` |
| Java | the record `QuotientAndRemainder(int quotient, int remainder)` | `result.quotient()` and `result.remainder()` |
| C# | the tuple type `(int Quotient, int Remainder)` | `var (q, r) = Division.DivMod(a, b)` |
| C | the out parameters `int *quotient, int *remainder` | `div_mod(a, b, &q, &r)` |
| C++ | the struct `div_mod_result` | `auto [q, r] = div_mod(a, b)` |
| Go | the result list `(int32, int32)` | `q, r := DivMod(a, b)` |
| Rust | the tuple type `(i32, i32)` | `let (q, r) = div_mod(a, b);` |
| Bash | one line of output, `echo "$q $r"` | `read -r q r` |

- **Python.** "The statement t = 12345, 54321, 'hello!' is an example of tuple packing"[^5]. The statement `return a // b, a % b` packs the two results in the same way. At the call, `q, r = div_mod(a, b)` does the reverse: "This is called, appropriately enough, sequence unpacking and works for any sequence on the right-hand side."[^5] The operators `//` and `%` already give floor results[^2], and the built-in `divmod` returns the same pair[^1].
- **JavaScript.** The function returns an array of two numbers. "The destructuring syntax is a JavaScript syntax that makes it possible to unpack values from arrays, or properties from objects, into distinct variables."[^6] The remainder operator does not give the floor result: "It always takes the sign of the dividend."[^7] So the solution changes the quotient and the remainder as the Task section describes.
- **TypeScript.** The return type `[number, number]` is a tuple type. "A tuple type is another sort of Array type that knows exactly how many elements it contains, and exactly which types it contains at specific positions."[^8] So the type checker knows that the result has two numbers.
- **Java.** The solution returns a record. "Record classes, which are a special kind of class, help to model plain data aggregates with less ceremony than normal classes."[^9] For a record, "the appropriate accessors, constructor, equals, hashCode, and toString methods are created automatically."[^9] The caller reads the results with `quotient()` and `remainder()`. `Math.floorDiv` and `Math.floorMod` give floor results: "Normal integer division operates under the round to zero rounding mode (truncation). This operation instead acts under the round toward negative infinity (floor) rounding mode."[^3]
- **C#.** The solution returns a tuple with named fields. "One of the most common use cases for tuples is as a method return type."[^10] The caller can "work with the returned tuple instance directly or deconstruct it in separate variables."[^10] Out parameters are another way: "To use an out parameter, both the method definition and the calling method must explicitly use the out keyword."[^11]
- **C.** The solution writes the two results through pointer parameters. Effective Go calls this "modifying an argument passed by address" and names it one of "a couple of clumsy idioms in C programs"[^12]. The standard function `div` returns a structure instead. It "Computes both the quotient (the member quot) and the remainder (the member rem) of the division of the numerator x by the denominator y."[^13] But `/` gives the quotient "truncated towards zero"[^14], so the solution changes the result.
- **C++.** The solution returns a struct with two named fields, and the caller unpacks it with a structured binding. A structured binding "Binds the specified names to subobjects or elements of the initializer."[^15] A `std::pair` also works: "std::pair is a class template that provides a way to store two heterogeneous objects as a single unit."[^16] Its fields are `first` and `second`, so the struct gives clearer names.
- **Go.** "One of Go's unusual features is that functions and methods can return multiple values."[^12] The result list `(int32, int32)` comes after the parameters, and `q, r := DivMod(a, b)` assigns both results. The Go operator `/` gives a quotient "truncated towards zero"[^17], so the solution changes the result.
- **Rust.** "Rust does let us return multiple values using a tuple"[^18]. "A tuple is a general way of grouping together a number of values with a variety of types into one compound type."[^19] The pattern `let (q, r) = div_mod(a, b);` unpacks it. "Integer division truncates toward zero to the nearest integer."[^19] So the solution changes the result.
- **Bash.** A Bash function cannot return two numbers. The solution prints them on one line, and the caller reads the line with `read`. The `read` builtin will "split it into words as described above in Word Splitting, and assign the first word to the first name, the second word to the second name, and so on."[^20]

### Unpack the results or keep them together

The caller can unpack the tuple into two names, or keep it as one value and index it:

```python run id=python-unpack
def div_mod(a: int, b: int) -> tuple[int, int]:
    return a // b, a % b


q, r = div_mod(-17, 5)
print(q, r)

result = div_mod(17, 5)
print(result)
print(result[0], result[1])
```

```output for=python-unpack
-4 3
(3, 2)
3 2
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `negative-dividend`, `negative-divisor` and `both-negative`: the signs of 17 and 5 change. The remainder takes the sign of `b`, so the results are (-4, 3), (-4, -3) and (3, -2).
- `small-negative`: -3 divided by 10 gives (-1, 7). A truncated division gives (0, -3).
- `exact-negative`: -20 divided by 4 has the remainder 0, so the solution does not change the quotient -5. In JavaScript, the remainder is `-0`, as in the MDN example `-4 % 2; // -0`[^7]. The JavaScript solution adds 0 to each result, so the result is 0.
- `zero-dividend` and `zero-negative-divisor`: 0 divided by 7 or by -7 gives (0, 0).
- `divisor-one`: 9 divided by 1 gives (9, 0).
- `int-max`, `int-min`, `min-by-max` and `max-by-min`: the largest and the smallest 32-bit integers. The change to the result never overflows, because the remainder and `b` have opposite signs when it adds `b`.

## Pitfalls

### Truncated division in C

The C operators `/` and `%` round the quotient toward zero[^14]:

```c wrong id=c-truncated-division
#include <stdio.h>

int main(void) {
    printf("%d %d\n", -17 / 5, -17 % 5);
    return 0;
}
```

```output for=c-truncated-division
-3 -2
```

The floor result is (-4, 3). The integer operator `/` in Java, Go and Rust also rounds toward zero[^3][^17][^19]. Use the change from the Task section, or a function such as `Math.floorDiv` and `Math.floorMod` in Java[^3].

### The comma operator in a JavaScript return

`return q, r;` does not return two values in JavaScript. "The comma (,) operator evaluates each of its operands (from left to right) and returns the value of the last operand."[^21]

```javascript wrong id=javascript-comma-return
function divMod(a, b) {
  return Math.floor(a / b), a % b;
}

console.log(divMod(17, 5));
```

```output for=javascript-comma-return
2
```

The function returns only the remainder. Return an array, `[q, r]`, or an object, `{ quotient: q, remainder: r }`.

### Too many names at the unpack in Python

"Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence."[^5]

```python wrong id=python-unpack-count
try:
    q, r, extra = divmod(17, 5)
except ValueError as e:
    print(e)
```

```output for=python-unpack-count
not enough values to unpack (expected 3, got 2)
```

Use one name for each value in the tuple.

## Related tasks

- [[define-a-function]]: define a function with one result.
- [[use-a-tuple]]: make a tuple and read its parts.
- [[destructure-a-value]]: unpack a tuple, an array or a record into names.
- [[define-a-record-type]]: define a record type, such as the Java result here.
- [[sum-and-multiply-a-list]]: another function with two results.

## References
