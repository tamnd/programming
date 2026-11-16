---
tag: NEC2B0
kind: concept
slug: constant
name: Constant
aliases: [constant, named constant, symbolic constant, const, final, readonly, constexpr, val, compile-time constant, run-time constant, read-only variable]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  A constant is a name for a value that does not change while the program runs. A compile-time constant has a value that the compiler knows, and a run-time constant gets its value once and keeps it.
applies_to: core-languages
viz: none
formal: none
see_also: [variable-concept, assignment, initialization-concept, immutability, value, binding]
same_as:
  wikidata: Q1284190
  wikipedia: Constant_(computer_programming)
sources:
- url: https://en.wikipedia.org/wiki/Constant_(computer_programming)
  title: Constant (computer programming), Wikipedia
  locator: Introduction; Use; Dynamically-valued constants; Naming conventions
- url: https://doc.rust-lang.org/reference/items/constant-items.html
  title: Constant items, The Rust Reference
  locator: items.const.intro; items.const.behavior
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/const
  title: The const keyword, C# reference
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/language/constexpr.html
  title: constexpr specifier, cppreference.com (C++)
  locator: Explanation
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Constants; Constant expressions
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.4 final Variables
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/readonly
  title: readonly, C# reference
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/const
  title: const, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3/library/typing.html
  title: typing, Support for type hints, Python documentation
  locator: typing.Final
- url: https://en.cppreference.com/w/c/language/const.html
  title: const type qualifier, cppreference.com (C)
  locator: Explanation
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, Bash Reference Manual
  locator: declare -r; readonly
---

## Idea

Some values in a program never change: the number of days in a week, a maximum size, a tax rate for this year. A constant gives such a value a name. "In computer programming, a constant is a value that is not altered by the program during normal execution."[^1] A [[variable-concept]], in contrast, "is an identifier with a value that can be changed during normal execution"[^1].

In JavaScript, `const` declares a constant, and an assignment to it fails:

```javascript run id=idea
const DAYS_PER_WEEK = 7;
const weeks = 3;
console.log(weeks * DAYS_PER_WEEK);
try {
  DAYS_PER_WEEK = 8;
} catch (e) {
  console.log(e.name);
}
console.log(DAYS_PER_WEEK);
```

```output for=idea
21
TypeError
7
```

The name `DAYS_PER_WEEK` tells the reader what 7 means, and a change in one place changes every use. "A constant value is defined once and can be referenced many times throughout a program."[^1]

## How it works

**Why constants help.** "Constants are useful for both programmers and compilers: for programmers, they are a form of self-documenting code and allow reasoning about correctness, while for compilers, they allow compile-time and run-time checks that verify that constancy assumptions are not violated"[^1]. A name such as `MAX_RETRIES` is clearer than the number 3 in many places, which is often called a magic number.

**Compile-time constants.** The compiler knows the value, because it is a literal or an expression of literals. The compiler can put the value at each use. In Rust, "A constant item is an optionally named constant value which is not associated with a specific memory location in the program"[^2]. In C#, "A constant expression is an expression that the compiler can fully evaluate at compile time"[^3], and the value of a `const` must be such an expression. In C++, "The constexpr specifier declares that it is possible to evaluate the value of the entities at compile time"[^4]. Go constants are values of the compiler only: "Numeric constants represent exact values of arbitrary precision and do not overflow"[^5].

**Run-time constants.** The value comes from the input or from a computation at run time, but after the first assignment it does not change. "Besides the static constants described above, many procedural languages such as Ada and C++ extend the concept of constantness toward global variables that are created at initialization time, local variables that are automatically created at runtime"[^1], and parameters. In Java, "A final variable may only be assigned to once"[^6]. In C#, a `readonly` field gets its value "during the declaration or in a constructor in the same class"[^7].

**A constant name is not a constant object.** In most languages, the keyword stops a new assignment to the name. It does not stop a change inside the object that the name refers to. MDN says: "The const declaration creates an immutable reference to a value. It does not mean the value it holds is immutable"[^8]. Java says the same of `final`: "If a final variable holds a reference to an object, then the state of the object may be changed by operations on the object, but the variable will always refer to the same object."[^6] For a value that cannot change at all, see [[immutability]].

**How the compiler checks.** "Some programming languages make an explicit syntactic distinction between constant and variable symbols, for example considering assignment to a constant to be a syntax error"[^1]. Java, C#, Rust, Go, C and C++ reject the assignment at compile time. JavaScript throws a `TypeError` when the assignment runs, as the idea example shows. Python has no constants: `typing.Final` tells a type checker that "Final names cannot be reassigned in any scope"[^9], but the interpreter does not check it.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | no constants; a name in capitals such as `MAX_SIZE` by convention, and `typing.Final` for a type checker |
| JavaScript | native | `const`: the name cannot get a new value; the object can still change |
| TypeScript | native | `const`, and `readonly` properties and `as const` for literal types |
| Java | native | `final` for a variable assigned once; `static final` with a constant expression for a compile-time constant |
| Kotlin | native | `val` for a name assigned once, and `const val` for a compile-time constant |
| C# | native | `const` for a compile-time constant, `readonly` for a field set once at run time |
| Go | native | `const` for compile-time constants of a basic type, typed or untyped |
| Rust | native | `const` items are inlined at each use; `let` without `mut` is a run-time constant |
| C | native | the qualifier `const` on an object, and `#define` for a macro |
| C++ | native | `const`, and `constexpr` for a value known at compile time |
| Bash | native | `readonly NAME=value` or `declare -r NAME=value` |
| PowerShell | native | `Set-Variable -Option Constant` or `-Option ReadOnly` |
| SQL | idiom | a query uses literals; procedural extensions declare constants, and a table can hold fixed values |
| Regex | n/a | a pattern has literal characters, but no names for values |
| HTML | n/a | HTML is markup, with no names for values |
| CSS | idiom | a custom property that the style sheet never changes acts as a named constant |

**C.** "Any attempt to modify an object whose type is const-qualified results in undefined behavior."[^10] A cast can remove `const` from a pointer, and the compiler then does not stop the write, but the program is wrong.

**Bash.** The option `-r` of `declare` makes names read only: "These names cannot then be assigned values by subsequent assignment statements or unset."[^11]

## Variations and related ideas

- **Naming.** "Naming conventions for constants vary. Some simply name them as they would any other variable. Others use capitals and underscores for constants in a way similar to their traditional use for symbolic macros, such as SOME_CONSTANT."[^1]
- **Macros.** A C macro such as `#define MAX 100` replaces the text before the compiler runs. It has no type and no scope. A `const` object or an `enum` is often better.
- **Enumerations.** An enumeration gives a set of related constants one type, such as the days of the week.
- **Immutable objects.** "A constant data structure or object is referred to as "immutable" in object-oriented parlance."[^1] See [[immutability]].

## Pitfalls

- **The object behind a constant name can change.** A `const` array in JavaScript, or a `final` list in Java, still accepts new elements:

```javascript wrong id=const-array
const COLORS = ["red", "green"];
COLORS.push("blue");
console.log(COLORS);
const FROZEN = Object.freeze(["red", "green"]);
try {
  FROZEN.push("blue");
} catch (e) {
  console.log(e.name);
}
console.log(FROZEN.length);
```

```output for=const-array
[ 'red', 'green', 'blue' ]
TypeError
2
```

`Object.freeze` stops changes to the array itself. The elements are strings, which cannot change, so the array is now fully constant.

- **A Python constant is only a name.** `MAX_SIZE = 10` can get a new value anywhere, with no error at run time. Use a type checker with `Final`, or an `Enum`.
- **A compile-time constant is copied into other code.** The C# reference says: "Don't create a constant to represent information that you expect to change over time."[^3] The reason: "because compilers propagate constants, other code compiled with your libraries needs to be recompiled to see the changes"[^3]. Use a `readonly` field for a value that can change between versions, and configuration for a value such as a price or a URL.

## Cost

A compile-time constant costs nothing at run time: the compiler puts the value at each use, and it can compute expressions of constants once. A run-time constant costs the same as a variable. The compiler can use the fact that it does not change, for example to keep it in a register.

## History

Constants are as old as high-level languages, and the idea of a constant at run time has its own history: Wikipedia cites a 1995 article in SIGPLAN Notices, "Dynamically-Valued Constants: An Underused Language Feature", for the constants that Ada and C++ make at run time[^1]. Ada has untyped constants such as `pi : constant := 3.1415926535;`, "with the untyped variant being implicitly converted to the appropriate type upon each use"[^1].

## References
