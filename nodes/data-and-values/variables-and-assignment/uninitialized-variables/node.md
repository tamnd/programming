---
tag: MA9Q4F
kind: concept
slug: uninitialized-variables
name: Uninitialized variables
aliases: [uninitialized variable, uninitialised variable, use before assignment, read before write, garbage value, indeterminate value, definite assignment, maybe-uninitialized, UnboundLocalError]
area: data-and-values/variables-and-assignment
level: core
summary: >-
  An uninitialized variable has no value that the program set before a read. In C and C++ the read gives an unpredictable value, and Java, C# and Rust reject the read at compile time.
applies_to: core-languages
viz: none
formal: none
see_also: [initialization-concept, default-value, variable-concept, undefined-behavior]
same_as:
  wikidata: Q2912653
  wikipedia: Uninitialized_variable
sources:
- url: https://en.wikipedia.org/wiki/Uninitialized_variable
  title: Uninitialized variable, Wikipedia
  locator: Introduction; Example of the C language; Use in languages
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-16.html
  title: Chapter 16, Definite Assignment, The Java Language Specification, Java SE 25
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/variables
  title: Variables, C# language specification
  locator: 9.1 General
- url: https://doc.rust-lang.org/reference/variables.html
  title: Variables, The Rust Reference
  locator: variable.init
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/let
  title: let, MDN Web Docs
  locator: Temporal dead zone (TDZ)
- url: https://docs.python.org/3/reference/executionmodel.html
  title: Execution model, The Python Language Reference
  locator: 4.2.2 Resolution of names
- url: https://en.cppreference.com/w/c/language/initialization.html
  title: Initialization, cppreference.com (C)
  locator: Implicit initialization
- url: https://en.cppreference.com/w/cpp/language/default_initialization.html
  title: Default-initialization, cppreference.com (C++)
  locator: Indeterminate and erroneous values
- url: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  title: Options to Request or Suppress Warnings, GCC manual
  locator: -Wuninitialized; -Wmaybe-uninitialized
---

## Idea

"In computing, an uninitialized variable is a variable that is declared but is not set to a definite known value before it is used."[^1] A loop that adds to a total needs a start value. Python reports the missing value when the read runs:

```python run id=idea
def total_price(prices):
    for price in prices:
        total = total + price
    return total


try:
    total_price([3, 4])
except UnboundLocalError as e:
    print(type(e).__name__)
```

```output for=idea
UnboundLocalError
```

The fix is one line: `total = 0` before the loop. In C, the same mistake gives no error, and the result is a number that can change from run to run.

## How it works

**What the memory holds.** A new variable gets a place in memory. In C, the place is on the stack, and the program does not clear it. "While the computer will set aside the appropriate amount of space for the stack frame, it usually does so simply by adjusting the value of the stack pointer, and does not set the memory itself to any new state (typically out of efficiency concerns)."[^1] So "whatever contents of that memory at the time will appear as initial values of the variables which occupy those addresses"[^1].

**What the languages do.** "Uninitialized variables are a particular problem in languages such as assembly language, C, and C++, which were designed for systems programming."[^1] Other languages close the gap in one of three ways:

- **No read before a value, checked by the compiler.** The compiler follows each path through the code and rejects a read on a path with no assignment. In Java, "For every access of a local variable declared by a statement x, or blank final field x, x must be definitely assigned before the access, or a compile-time error occurs."[^2] C# has the same rule: "For an initially unassigned variable to be considered definitely assigned at a certain location, an assignment to the variable shall occur in every possible execution path leading to that location."[^3] In Rust, "Local variables can be used only after they have been initialized through all reachable control flow paths."[^4]
- **An error at run time.** In JavaScript, a `let` variable read before its declaration line throws: "any attempt to access it will result in a ReferenceError"[^5]. In Python, a local name without a value raises `UnboundLocalError`: "If the current scope is a function scope, and the name refers to a local variable that has not yet been bound to a value at the point where the name is used, an UnboundLocalError exception is raised."[^6]
- **A default value.** Go, and Java for fields, give each new variable a known value, see [[default-value]]. "Java does not have uninitialized variables."[^1]

**C and C++.** In C, "objects with automatic storage duration are initialized to indeterminate values (which may be trap representations)"[^7]. Wikipedia gives a loop that adds 1 to `k` ten times without a start value: "The final value of k is undefined."[^1] In C++, "If no initialization is performed for an object, that object retains an indeterminate value until that value is replaced."[^8] "Every other use of an indeterminate value is undefined behavior."[^8] See [[undefined-behavior]]. C++26 changes most of these values to erroneous values, "where each value is determined by the implementation independently of the state of the program"[^8], and "Every other use of an erroneous value is erroneous behavior: it may result in compile-time warning or error, run-time error, or an implementation-defined value."[^8]

**Compiler warnings.** C and C++ compilers warn about some reads. GCC has `-Wuninitialized`, to "Warn if an object with automatic or allocated storage duration is used without having been initialized"[^9], and `-Wmaybe-uninitialized` for a path that may skip the assignment. The warnings are not complete: "These warnings are only possible in optimizing compilation, because otherwise GCC does not keep track of the state of variables."[^9] And GCC "is known not to warn in many situations (false negatives)"[^9].

## In languages

| Language | Verdict | A read before a value |
|---|---|---|
| Python | native | `UnboundLocalError` for a local name, `NameError` for an unknown name |
| JavaScript | native | `ReferenceError` in the temporal dead zone of `let` and `const`; `undefined` after `let x;` |
| TypeScript | native | the rules of JavaScript; the compiler reports a read before an assignment |
| Java | native | a compile error for a local variable; fields have default values |
| Kotlin | native | a compile error for a local variable; `lateinit` throws at run time |
| C# | native | a compile error for a local variable; fields have default values |
| Go | absent | each variable has its zero value |
| Rust | native | a compile error; `MaybeUninit` in unsafe code |
| C | native | an indeterminate value, and undefined behavior |
| C++ | native | an indeterminate value; an erroneous value from C++26 |
| Bash | idiom | the empty string; `set -u` makes it an error |
| PowerShell | idiom | `$null`; strict mode makes it an error |
| SQL | n/a | each column of a row has a value or NULL |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML is markup, with no variables |
| CSS | n/a | each property has an initial value |

## Variations and related ideas

- **Partial initialization.** A struct or an array with some members set and others not. Wikipedia shows a C function that "leaks memory contents because it fails to fully initialize the members"[^1] of a struct before it copies the struct out.
- **Out parameters.** "C# has a special flavour of call-by-reference parameters to subroutines (specified as out instead of the usual ref), asserting that the variable is allowed to be uninitialized on entry but will be initialized afterwards."[^1]

## Pitfalls

- **A wrong start value is as bad as none.** In JavaScript, `let max;` starts at `undefined`, and a comparison with `undefined` is always false, so the loop never sets `max`:

```javascript wrong id=undefined-start
let max;
for (const x of [-5, -2, -9]) {
  if (x > max) {
    max = x;
  }
}
console.log(max);
```

```output for=undefined-start
undefined
```

Start with the first element, or with `-Infinity`.

- **"It works on my machine."** In C, an uninitialized variable can hold 0 by chance in a debug build and another value in an optimized build. A test that passes is no proof.
- **Silence of the compiler.** No warning does not mean no read. A read through a pointer, or in another function, is often not seen.
- **A loop variable that keeps its old value.** A variable declared outside a loop keeps the value of the last pass. Initialize it inside the loop when each pass needs a new start.

## Cost

The check costs nothing when the program runs: the compiler does it. A run-time check, such as the temporal dead zone of JavaScript, costs a test on some reads. To initialize a variable costs one write. C and C++ save this write for each automatic variable, at the price of the bugs above.

## History

Later languages reduced the problem: Java made definite assignment a compile-time rule and gave fields default values, and "many compilers will attempt to identify the use of uninitialized variables and report them as compile-time errors"[^1] even in languages that allow them.

## References
