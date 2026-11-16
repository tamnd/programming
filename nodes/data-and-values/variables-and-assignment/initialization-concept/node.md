---
tag: MB0CJZ
kind: concept
slug: initialization-concept
name: Initialization
aliases: [initialization, initialisation, initializer, initial value, initialize a variable, zero value, definite assignment, default initialization, temporal dead zone, initializer list]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  Initialization gives a variable its first value. Some languages give a default value such as zero, others reject a read before it, and in C and C++ such a read can give a garbage value.
applies_to: core-languages
viz: none
formal: none
see_also: [variable-concept, assignment, constant, declaration, constructors, undefined-behavior, value]
same_as:
  wikidata: Q6034165
  wikipedia: Initialization_(computer_programming)
sources:
- url: https://en.wikipedia.org/wiki/Initialization_(computer_programming)
  title: Initialization (computer programming), Wikipedia
  locator: Introduction
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Variable declarations; The zero value
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.12.5 Initial Values of Variables
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
  locator: Introduction; Implicit initialization; Empty initialization
- url: https://en.cppreference.com/w/cpp/language/default_initialization.html
  title: Default-initialization, cppreference.com (C++)
  locator: Explanation; Indeterminate and erroneous values
---

## Idea

A [[variable-concept]] needs a value before the program can read it. "In computer programming, initialization or initialisation is the assignment of an initial value for a data object or variable."[^1] A sum starts at 0, a count starts at 0, and a flag starts at false:

```python run id=idea
total = 0
for price in [3, 4, 5]:
    total = total + price
print(total)
```

```output for=idea
12
```

The line `total = 0` is the initialization. Without it, the first `total + price` has no value to add to.

## How it works

**Declaration and initialization.** A [[declaration]] makes a name known. Initialization gives the variable its first value. "Initialization is distinct from (and preceded by) declaration, although the two can sometimes be conflated in practice."[^1] In most languages, one line does both: `int count = 0;` declares `count` and initializes it. The part after `=` is the initializer. "Programming constructs which perform initialization are typically called initializers and initializer lists."[^1]

**Initialization is not assignment.** An [[assignment]] replaces the value of a variable that already has one. Initialization gives the first value. In most languages, the two look the same, but some rules apply only to one of them: a [[constant]] accepts an initializer and rejects a later assignment, and a C++ object is made by a constructor at initialization, not by the assignment operator.

**A variable without an initializer.** Languages do one of four things:

1. **A default value.** The language puts a known value into the variable. In Go, "If a list of expressions is given, the variables are initialized with the expressions following the rules for assignment statements. Otherwise, each variable is initialized to its zero value."[^2] Each element "is set to the zero value for its type: false for booleans, 0 for numeric types, "" for strings, and nil for pointers, functions, interfaces, slices, channels, and maps"[^2]. Java does the same for fields and array elements: "Each class variable, instance variable, or array component is initialized with a default value when it is created"[^3].
2. **A compile-time check.** The compiler proves that each path to a read gives the variable a value first, and rejects the program if not. In Java, "For every access of a local variable declared by a statement x, or blank final field x, x must be definitely assigned before the access, or a compile-time error occurs."[^4] In C#, "A variable shall be definitely assigned (§9.4) before its value can be obtained."[^5] Rust has the same rule: "Local variables can be used only after they have been initialized through all reachable control flow paths."[^6]
3. **An error at run time.** In JavaScript, a `let` variable is in a "temporal dead zone" (TDZ) "from the start of the block until code execution reaches the place where the variable is declared and initialized"[^7]. A read in this zone fails: "any attempt to access it will result in a ReferenceError"[^7]. In Python, a name exists only after it is bound to a value, and a read before that raises an exception: "If the current scope is a function scope, and the name refers to a local variable that has not yet been bound to a value at the point where the name is used, an UnboundLocalError exception is raised."[^8]
4. **No initialization.** In C, "objects with automatic storage duration are initialized to indeterminate values (which may be trap representations)"[^9]. The local variables of a function get whatever bits are in the memory. In C++, "If no initialization is performed for an object, that object retains an indeterminate value until that value is replaced."[^10] A read of such a value is [[undefined-behavior]] in C and in C++ before C++26.

**Static variables in C and C++.** C gives a zero value to the variables that live for the whole program. "objects with static and thread-local storage duration are empty-initialized"[^9]: "pointers are initialized to null pointer values of their types", "objects of integral types are initialized to unsigned zero" and "objects of floating types are initialized to positive zero"[^9]. So a global `int count;` is 0, but a local `int count;` is not.

**Objects.** In object-oriented languages, a constructor initializes a new object. "in object-oriented programming, initialization code may be part of a constructor (class method) or an initializer (instance method)."[^1] See [[constructors]].

## In languages

| Language | Verdict | Without an initializer |
|---|---|---|
| Python | native | no declaration; a name exists after its first binding, and a read before raises `NameError` or `UnboundLocalError` |
| JavaScript | native | `let` and `const` are in the temporal dead zone until their line; `let x;` and `var` give `undefined` |
| TypeScript | native | the rules of JavaScript, and the compiler reports a read of a `let` before an assignment |
| Java | native | fields and array elements get a default value; a local variable must be definitely assigned before a read |
| Kotlin | native | a local `val` or `var` must have a value before a read; a property needs an initializer or `lateinit` |
| C# | native | fields and array elements are initially assigned with the default value; a local variable must be definitely assigned |
| Go | native | each variable gets the zero value of its type |
| Rust | native | a `let` without a value must get one on all paths before a read; no default value |
| C | native | static variables are zero; automatic variables are indeterminate |
| C++ | native | as C for built-in types; a class type runs its default constructor |
| Bash | idiom | an unset variable expands to the empty string; `set -u` makes the expansion an error |
| PowerShell | idiom | an unassigned variable is `$null`; strict mode makes the read an error |
| SQL | native | a column default gives the value of a new row; without one, the value is NULL |
| Regex | n/a | a pattern has no variables |
| HTML | n/a | HTML is markup, with no variables |
| CSS | native | each property has an initial value, and the keyword `initial` sets it |

**C++.** C++ has many forms of initialization: `int a = 1;`, `int a(1);` and `int a{1};`. "Default-initialization is performed in three situations"[^10], the first "when a variable with automatic, static, or thread-local storage duration is declared with no initializer"[^10]. For an `int`, default initialization does nothing, but `int a{};` gives 0.

## Variations and related ideas

- **Default values.** A default value is the value that a variable gets when the program gives none, such as 0, `false`, `null` or the empty string.
- **Lazy initialization.** The program computes the first value at the first read, for a value that is costly to make and may not be needed.
- **Initialization order.** In a class, the fields get their values in a fixed order, and a field that reads another field before it has its value can see a default value.
- **Single assignment.** In some languages, initialization is the only assignment: a name gets a value once and keeps it.

## Pitfalls

- **A sum without a start value.** In JavaScript, `let total;` gives `undefined`, and `undefined + 3` is `NaN`:

```javascript wrong id=no-start-value
let total;
for (const price of [3, 4, 5]) {
  total += price;
}
console.log(total);
```

```output for=no-start-value
NaN
```

Write `let total = 0;`. In C, the same loop on an automatic variable without `= 0` reads an indeterminate value, and the result can change from run to run.

- **A default value hides a missing initialization.** In Go and in Java fields, a variable that the program forgot to set is 0 or `null`, with no error. The program goes on with a wrong value.
- **Reuse of the variable in a loop.** A variable that collects values for each item must be initialized inside the loop, not once before it. Otherwise it keeps the values of the item before.
- **A global zero is not a local zero.** In C and C++, `int count;` is 0 at file scope and indeterminate in a function.

## Cost

Initialization to a default value costs a write of zeros, which is often free: C programs keep static data in a part of the program image that the system fills with zeros. "this form of initialization for statics is normally implemented by allocating them in the .bss section of the program image"[^9]. For a large array, a language that zeroes each new array spends time on each element. C and C++ do not zero automatic variables, so a local variable without an initializer costs nothing. A compile-time check of definite assignment costs nothing when the program runs.

## History

C leaves an automatic variable without an initializer indeterminate. Later languages added rules: Java gave default values to fields and made the compiler check that each local variable "must have a definitely assigned value when any access of its value occurs"[^4]. Go made the zero value a rule for all variables, and C++26 changed the read of an uninitialized automatic variable from undefined behavior to an "erroneous value"[^10] that the implementation chooses.

## References
