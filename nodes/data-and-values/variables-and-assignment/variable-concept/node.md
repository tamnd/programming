---
tag: XYTFHJ
kind: concept
slug: variable-concept
name: Variable
aliases: [variable, variables, program variable, local variable, global variable, mutable variable, storage location, named storage, var, let]
area: data-and-values/variables-and-assignment
level: intro
summary: >-
  A variable is a name for a value that can change while the program runs. In some languages the name stands for a storage place that holds the value, and in others it refers to an object.
applies_to: core-languages
viz: optional
formal: none
see_also: [value, binding, declaration, constant, assignment, initialization-concept, lexical-scope, immutability]
same_as:
  wikidata: Q877977
  wikipedia: Variable_(high-level_programming_language)
sources:
- url: https://en.wikipedia.org/wiki/Variable_(high-level_programming_language)
  title: Variable (high-level programming language), Wikipedia
  locator: Introduction; Actions on a variable; Scope and extent; Typing
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Variables; Short variable declarations
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: Chapter 4, Types, Values, and Variables, The Java Language Specification, Java SE 25
  locator: 4.3.1; 4.12; 4.12.1
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/variables
  title: Variables, C# language specification
  locator: 9.1 General
- url: https://docs.python.org/3/reference/executionmodel.html
  title: Execution model, The Python Language Reference
  locator: 4.2.1 Binding of names
- url: https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/6515/sicp.zip/full-text/book/book-Z-H-20.html
  title: Structure and Interpretation of Computer Programs, 2nd edition, section 3.1, Assignment and Local State
  locator: 3.1.1; 3.1.3
- url: https://doc.rust-lang.org/book/ch03-01-variables-and-mutability.html
  title: Variables and Mutability, The Rust Programming Language
  locator: Introduction; Shadowing
- url: https://www.gnu.org/software/bash/manual/html_node/Shell-Parameters.html
  title: Shell Parameters, Bash Reference Manual
  locator: 3.4
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/var
  title: var, MDN Web Docs
  locator: Initialization of several variables
---

## Idea

A program keeps its inputs, its counters and its partial results under names, so that a later step can use them. Such a name is a variable. In the classic description, "a variable is a named container for a particular set of bits or type of data"[^1]. The program reads the variable to get the value, and it assigns to the variable to put a new value in it.

In Python, `=` gives a name a value, and a later `=` gives the same name a new value:

```python run id=idea
count = 0
for word in ["tea", "milk", "tea"]:
    if word == "tea":
        count = count + 1
print(count)
count = "done"
print(count)
```

```output for=idea
2
done
```

The name `count` holds 0, then 1, then 2, and at the end the text `done`. "While a variable's name, type, and location often remain fixed, the data stored in the location may be changed during program execution"[^1].

## How it works

**A variable has a name, a place, a value and often a type.** The name is in the source code. The place is memory that the compiler or the run time gives to the variable. The value is what the place holds now. "Compilers have to replace variables' symbolic names with the actual locations of the data"[^1].

**The box model.** In C, C++, Go, Java and C#, a variable is a box of a fixed type. The Go specification says: "A variable is a storage location for holding a value. The set of permissible values is determined by the variable's type."[^2] Java says the same: "A variable is a storage location and has an associated type"[^3], and C# says that "Variables represent storage locations"[^4]. An assignment copies a value into the box. Two variables are two boxes, so a change to one does not change the other.

**The label model.** In Python, a variable is a name that refers to an object: "Names refer to objects. Names are introduced by name binding operations."[^5] An assignment does not copy the object. It makes the name refer to an object, so two names can refer to the same object. In such languages, "a variable is more accurately described as a name associated with a value, with memory allocation handled transparently by the interpreter or runtime"[^1].

**References in a box.** The two models meet in the box languages too. In Java, "A variable of a primitive type always holds a primitive value of that exact primitive type"[^3], but a variable of a class type holds a reference: "The reference values (often just references) are pointers to these objects"[^3]. So `int b = a` copies a number, and `int[] ys = xs` copies a reference, and `xs` and `ys` then show the same array. The page [[pointers-and-references]] has the details.

**Change makes a variable a place.** A variable that never changes is only a name for a value, as in mathematics. Assignment changes this. In the words of SICP, once the value of a variable can change, "a variable can no longer be simply a name. Now a variable somehow refers to a place where a value can be stored, and the value stored at this place can change."[^6] The order of the steps of the program then matters, because the same name gives different values at different times.

**Imperative and functional languages.** "In imperative programming languages, values can generally be accessed or changed at any time. In pure functional and logic languages, variables are bound to expressions and keep a single value during their entire lifetime due to the requirements of referential transparency."[^1] Rust takes a middle way: "by default, variables are immutable"[^7], and the keyword `mut` allows changes.

**Static and dynamic types.** "In statically typed languages such as C, C++, Java or C#, a variable also has a type, meaning that only certain kinds of values can be stored in it"[^1]. "In dynamically typed languages such as Python, a variable's type is inferred by its value, and can change according to its value"[^1]. The idea example shows this: `count` holds an integer and then a string.

**Scope and lifetime.** "The scope of a variable describes where in a program's text the variable may be used, while the extent (also called lifetime) of a variable describes when in a program's execution the variable has a (meaningful) value"[^1]. The page [[lexical-scope]] describes scope.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | a name bound to an object by `=`; no declaration; any type |
| JavaScript | native | `let` and `var` declare a variable, `const` a name that is not reassigned; any type |
| TypeScript | native | `let` and `const`, as in JavaScript, with a type annotation or an inferred type |
| Java | native | a typed storage location: `int n = 0;`, or `var n = 0;` with an inferred type |
| Kotlin | native | `var` for a variable and `val` for a name that is not reassigned |
| C# | native | a typed storage location: `int n = 0;`, or `var n = 0;` |
| Go | native | `var n int` or `n := 0`; a typed storage location |
| Rust | native | `let x = 5;` is immutable, `let mut x = 5;` can change |
| C | native | an object with storage: `int n = 0;`; `&n` gives its address |
| C++ | native | as in C, and `auto n = 0;` and references such as `int& r = n;` |
| Bash | native | a named shell parameter, `n=0`, read as `$n`; values are strings |
| PowerShell | native | `$n = 0`; any type, or a type constraint such as `[int]$n` |
| SQL | idiom | a query has no variables; procedural extensions declare local variables with `DECLARE` |
| Regex | n/a | a pattern has no variables; a group captures text, and a backreference such as `\1` reads it |
| HTML | n/a | HTML is markup, with no variables |
| CSS | idiom | a custom property such as `--gap: 8px`, read with `var(--gap)`; the cascade sets its value |

**Bash.** "A variable is a parameter denoted by a name. A variable has a value and zero or more attributes."[^8] The value is a string, and the attribute `-i` of `declare` makes the shell treat the value as an integer.

**Go.** A short variable declaration "is shorthand for a regular variable declaration with initializer expressions but no types"[^2]: `i, j := 0, 10` declares two variables of type `int`.

## Variations and related ideas

- **Local and global variables.** In Python, "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global. If a name is bound at the module level, it is a global variable."[^5] The other languages have the same split, with different rules.
- **Constants.** A name whose value does not change is a [[constant]]. Many languages use the same syntax with another keyword: `const` in JavaScript, `final` in Java and `val` in Kotlin.
- **Assignment and initialization.** The first value of a variable is its initialization, and each later change is an assignment. See [[initialization-concept]] and [[assignment]].
- **Binding.** The link between the name and the place or the object is a [[binding]]. Rebinding a name is a different thing from a change to the object that the name refers to.
- **Aliasing.** "A variable's storage location may be referenced by several different identifiers, a situation known as aliasing"[^1]. A pointer in C, a reference in C++, and two names for one object in Python are aliases.

## Pitfalls

- **Two names for one object.** In the label model, and for references in the box model, a change through one name shows through the other. A program that wants a separate value must copy it. In JavaScript:

```javascript wrong id=shared-array
const scores = [10, 20];
const backup = scores;
scores.push(30);
console.log(backup);
const copy = [...scores];
scores.push(40);
console.log(copy);
```

```output for=shared-array
[ 10, 20, 30 ]
[ 10, 20, 30 ]
```

`backup` is not a backup: it is the same array. The spread syntax `[...scores]` makes a new array.

- **A typo makes a new variable.** In Python, an assignment to a misspelled name makes a new variable, with no error. JavaScript outside strict mode has the same trap: an assignment to a name with no declaration "is an unqualified identifier assignment, which creates a global variable in non-strict mode"[^9].
- **A read before the first write.** A variable that has no value yet can give garbage, an error or a special value, by language. See [[initialization-concept]].
- **Long lived variables.** A global variable can change anywhere in the program, so a reader cannot see all the places that change it. Keep variables in the smallest scope that works.

## Cost

A local variable of a simple type costs a few bytes of stack memory or a register, and the compiler often removes it. A read or a write is one memory access or none. In the label model, each name is a reference, and the object lives on the heap, so a variable costs the reference and the object.

## History

Variables came from mathematics, but "Variables in programming may not directly correspond to the concept of variables in mathematics. The latter is abstract, having no reference to a physical object such as storage location."[^1] Assignment-based programming with variables as places is the base of the imperative languages. SICP shows the cost of the idea. With assignment, "Our programming language can no longer be interpreted in terms of the substitution model of procedure application"[^6], the model that explains a function call as the replacement of each parameter by its value.

## References
