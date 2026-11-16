---
tag: F834HP
kind: task
slug: declare-a-constant
name: Declare a constant
aliases: [constant declaration, named constant, define a constant, const, final, constexpr, readonly, compile-time constant, magic number]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  Give a fixed value a name that the program cannot assign again, with const, final, constexpr or readonly. Know which constants the compiler computes, and why a constant object can still change inside.
contract: contract.yaml
category: basics
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Constant_(computer_programming)
  title: Constant (computer programming), Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/library/typing.html
  title: typing, Support for type hints, Python documentation
  locator: typing.Final
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
  title: const, MDN Web Docs
  locator: Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.4 final Variables
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/const
  title: The const keyword, C# language reference
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Constant declarations; Constants
- url: https://doc.rust-lang.org/reference/items/constant-items.html
  title: Constant items, The Rust Reference
  locator: items.const
- url: https://en.cppreference.com/w/c/language/const.html
  title: const type qualifier, cppreference.com (C)
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/constexpr.html
  title: constexpr specifier, cppreference.com (C++)
  locator: Explanation
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, GNU Bash Reference Manual
  locator: declare -r; readonly
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/freeze
  title: Object.freeze(), MDN Web Docs
  locator: Introduction
---

## Task

"In computer programming, a constant is a value that is not altered by the program during normal execution."[^1] A named constant gives the value a name, such as `SECONDS_PER_DAY`, so the program does not repeat a bare number such as 86400.

The function `seconds_in(days)` returns the number of seconds in `days` days of 24 hours. It declares four named constants: 60 seconds in a minute, 60 minutes in an hour, 24 hours in a day, and the seconds in a day, which the program computes from the other three. The argument is from 0 to 1000000, so the result can be larger than a 32-bit integer.

## Solutions

| Language | Declaration | Checked |
|---|---|---|
| Python | `SECONDS_PER_DAY: Final = ...` | by a type checker, not when the program runs |
| JavaScript | `const SECONDS_PER_DAY = ...` | when the program runs |
| TypeScript | `const SECONDS_PER_DAY = ...` | by the compiler |
| Java | `static final long SECONDS_PER_DAY = ...` | by the compiler |
| C# | `const long SecondsPerDay = ...` | by the compiler |
| Go | `const SecondsPerDay = ...` | by the compiler |
| Rust | `const SECONDS_PER_DAY: i64 = ...` | by the compiler |
| C | `enum { SECONDS_PER_DAY = ... }` | by the compiler |
| C++ | `constexpr long long seconds_per_day = ...` | by the compiler |
| Bash | `readonly SECONDS_PER_DAY=...` | when the program runs |

- **Python.** Python has no constants. A name in upper case is a convention, and `typing.Final` tells a type checker: "Final names cannot be reassigned in any scope."[^2] But "There is no runtime checking of these properties."[^2]
- **JavaScript and TypeScript.** `const` makes a binding that gets no new value. "The value of a constant can't be changed through reassignment using the assignment operator, but if a constant is an object, its properties can be added, updated, or removed."[^3]
- **Java.** "A final variable may only be assigned to once."[^4] A `static final` field of a primitive type or `String` with a constant expression is a constant variable: "A constant variable is a final variable of primitive type or type String that is initialized with a constant expression"[^4].
- **C#.** "Constant fields and locals aren't variables and can't be modified."[^5] The value of a `const` must be known at compile time. For a value that the program computes when it runs, C# has `readonly` fields.
- **Go.** "A constant declaration binds a list of identifiers (the names of the constants) to the values of a list of constant expressions."[^6] The constants of the solution have no type. "An untyped constant has a default type which is the type to which the constant is implicitly converted in contexts where a typed value is required"[^6], so `SecondsPerDay` takes the type `int64` in `days * SecondsPerDay`.
- **Rust.** "Constants must be explicitly typed."[^7] A constant has no fixed place in memory: "Constants are essentially inlined wherever they are used, meaning that they are copied directly into the relevant context when used."[^7]
- **C.** A `const` variable is read-only, but it is not a constant expression: "unlike in C++, expressions of const-qualified type in C are not constant expressions; they may not be used as case labels or to initialize static and thread storage duration objects"[^8]. So the solution uses the names of an `enum`, which are integer constant expressions. `#define` is the other common way.
- **C++.** "The constexpr specifier declares that it is possible to evaluate the value of the entities at compile time."[^9] The solution checks the value with `static_assert(seconds_per_day == 86400)`, which fails at compile time if the value is wrong.
- **Bash.** `readonly` or `declare -r` marks a name. "These names cannot then be assigned values by subsequent assignment statements or unset."[^10]

## Edge cases

- **Zero days.** The result is 0.
- **Past 32 bits.** 24855 days give 2147472000 seconds, which still fits in a 32-bit signed integer. 24856 days give 2147558400, which does not. The solutions use 64-bit types for the argument and the result.
- **A large argument.** 1000000 days give 86400000000 seconds.

## Pitfalls

- **A constant object can still change.** `const`, `final` and `Final` stop a new assignment to the name, but not a change to the object that the name refers to. In Python, the name is not checked at all when the program runs:

```python wrong id=python-final-list
from typing import Final

WEEKDAYS: Final = ["Mon", "Tue", "Wed", "Thu", "Fri"]
WEEKDAYS.append("Sat")
print(len(WEEKDAYS))
WEEKDAYS = []
print(WEEKDAYS)
```

```output for=python-final-list
6
[]
```

A type checker reports the second assignment, but not the `append`. For a constant collection, use a type that cannot change, such as a tuple or a `frozenset`:

```python run id=python-tuple-constant
from typing import Final

WEEKDAYS: Final = ("Mon", "Tue", "Wed", "Thu", "Fri")
try:
    WEEKDAYS.append("Sat")
except AttributeError:
    print("a tuple has no append")
print(len(WEEKDAYS))
```

```output for=python-tuple-constant
a tuple has no append
5
```

In JavaScript, "The Object.freeze() static method freezes an object."[^11] In Java, `List.of` makes a list that cannot change, and a `final` field can refer to it.

- **Overflow in the constant expression.** In C, `24 * 60 * 60 * 1000` has the type `int`. A larger product can overflow before it reaches a `long long` variable. Write one operand as `1000LL`.
- **A change through a pointer in C.** "Any attempt to modify an object whose type is const-qualified results in undefined behavior."[^8] A cast that removes `const` does not make the change safe.
- **A copied constant.** C#, Rust and Java copy the value of a compile-time constant into the code that uses it. If a library changes a public `const`, code that was compiled against the old library keeps the old value until it is compiled again.

## Related tasks

- [[constant]]: constants as a concept, and the kinds of constants.
- [[declare-a-variable]]: a name whose value can change.
- [[immutability]]: values that cannot change, as opposed to names that cannot get a new value.
- [[single-assignment]]: names that get one value, in all code.

## References
