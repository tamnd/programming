---
tag: ZVDVMM
kind: concept
slug: declaration
name: Declaration
aliases: [declaration, definition, declaration and definition, forward declaration, prototype, function prototype, extern declaration, tentative definition, one definition rule, ODR, hoisting, declare before use]
area: syntax-and-semantics/scope-and-binding
level: core
summary: >-
  A declaration introduces a name and tells the compiler what the name is, such as a variable of a type or a function with a signature. A definition is a declaration that also gives the thing itself.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur, John Backus, Friedrich L. Bauer, John McCarthy, Alan J. Perlis, Heinz Rutishauser, Adriaan van Wijngaarden]
  note: The ALGOL 60 report, from the Paris conference of January 1960, has a section named Declarations. It says that declarations define properties of the quantities in the program and associate them with identifiers, and that a declaration is valid for one block.
see_also: [binding, identifier, lexical-scope, compilers-and-interpreters]
same_as:
  wikidata: Q1183659
  wikipedia: Declaration_(computer_programming)
sources:
- url: https://en.wikipedia.org/wiki/Declaration_(computer_programming)
  title: Declaration (computer programming), Wikipedia
  locator: Introduction; Declarations and definitions
- url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
  title: ISO/IEC 9899:2024 working draft N3220, Programming languages, C
  locator: 6.7.1, paragraph 6; 6.9.3, paragraph 2
- url: https://en.cppreference.com/w/cpp/language/definition.html
  title: Definitions and ODR (One Definition Rule), C++ reference, cppreference.com
  locator: Introduction; One Definition Rule
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-6.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 6
  locator: 6.3 Scope of a Declaration
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Declarations and scope; Type declarations
- url: https://doc.rust-lang.org/reference/items.html
  title: Items, The Rust Reference
  locator: items.decl-order; items.name-resolution
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/local-functions
  title: Local functions, C# programming guide
  locator: Local function syntax
- url: https://developer.mozilla.org/en-US/docs/Glossary/Hoisting
  title: Hoisting, MDN Web Docs Glossary
  locator: Introduction
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7.2.2 Annotated assignment statements; 7.12 The global statement; 7.13 The nonlocal statement
- url: https://doc.rust-lang.org/reference/items/external-blocks.html
  title: External blocks, The Rust Reference
  locator: items.extern.intro
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/partial-member
  title: Partial member, C# language reference
  locator: Introduction
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60, J. W. Backus and others, edited by Peter Naur
  locator: Introduction; 5. Declarations
---

## Idea

A program uses names, and the compiler or the interpreter must know what each name is. A declaration introduces a name and gives its properties: a variable and its type, a function and its parameters, or a type and its parts. A definition is a declaration that also gives the thing itself: the storage for a variable, the body of a function or the layout of a type. Some languages keep the two apart, so that one file can declare a function and another file can define it. Other languages join them, so that each declaration is also a definition.

```python run id=idea
class Config:
    name: str  # a declaration only: a name and a type, but no value
    retries: int = 3  # a declaration and a definition: it also gives the value


print(Config.__annotations__)
print(hasattr(Config, "name"), Config.retries)

# def is a statement that runs, so the function exists only after the def has run.
try:
    early()
except NameError as e:
    print("NameError:", e)


def early():
    return "now it exists"


print(early())


# A function body looks up names when it runs, so it can use a function defined below it.
def first():
    return second()


def second():
    return "second"


print(first())
```

```output for=idea
{'name': <class 'str'>, 'retries': <class 'int'>}
False 3
NameError: name 'early' is not defined
now it exists
second
```

`name: str` declares a name and a type, but it gives no value, so the class has no attribute `name`. `retries: int = 3` declares the name and also defines it. In Python, `def` is a statement that runs, so a call before the `def` fails with `NameError`. A function body looks up names when it runs, so `first` can call `second`, which is defined below it.

## How it works

**Declaration and definition.** "The term "declaration" is frequently contrasted with the term "definition", but meaning and usage varies significantly between languages"[^1]. The C standard gives the exact rule: "A declaration specifies the interpretation and properties of a set of identifiers." A definition is a declaration that, for an object, "causes storage to be reserved for that object", and for a function "includes the function body"[^2]. C++ uses the same split: "Definitions are declarations that fully define the entity introduced by the declaration"[^3].

**What a declaration gives.** A declaration usually gives some of these properties:

- The kind of thing: a variable, a constant, a function, a type, a module or a label.
- The type, or the signature of a function. Some languages infer the type from the value, with `var`, `auto` or `:=`.
- The scope, which is the part of the program where the name is valid. See [lexical scope](/lexical-scope/).
- The storage and the linkage: `static`, `extern`, `const`, `final` and similar words.

**Declare before use, or any order.** C reads a file from top to bottom, so a function must have a declaration before the first call. A prototype such as `int sum(int a, int b);` gives the declaration, and the definition can come later or in another file. Other languages let the program use a name before its declaration:

- In Java, "The scope of a declaration of a member m declared in or inherited by a class or interface C ... is the entire body of C"[^4]. A local variable is different: its scope "is the rest of the block, starting with the declaration's own initializer"[^4].
- In Go, the scope of a name declared at top level "is the package block"[^5], so the order of the top-level declarations does not matter.
- In Rust, "Items may be defined in any order"[^6], and this is also true for an item inside a block.
- In C#, "You can declare a local function before or after the statements that call it"[^7].
- In JavaScript, hoisting lets code use some names before their declarations. MDN describes it as "the process whereby the interpreter appears to move the declaration of functions, variables, classes, or imports to the top of their scope, prior to execution of the code"[^8]. A function declaration is hoisted with its body. A `var` is hoisted, but its value is not, so it reads as `undefined`. A `class` or a `let` cannot be read before its declaration runs.

**Declarations in Python.** Python has no declaration of a variable. An assignment makes the name. The `global` statement "causes the listed identifiers to be interpreted as globals", and the `nonlocal` statement "causes the listed identifiers to refer to names previously bound in nonlocal scopes"[^9]. An annotation such as `total: int` declares a type for tools and for `__annotations__`, but when the right side is not present, no value is bound[^9]. `def` and `class` are definitions that run as statements.

**Declarations in Go.** "A declaration binds a non-blank identifier to a constant, type, type parameter, variable, function, label, or package. Every identifier in a program must be declared"[^5]. Go has two forms of type declaration. "A type definition creates a new, distinct type with the same underlying type and operations as the given type", and "An alias declaration binds an identifier to the given type"[^5].

**One definition.** A program can declare a name many times, but it must define it only once. C++ requires "One and only one definition of every non-inline function or variable that is odr-used" in the entire program, and "The compiler is not required to diagnose this violation"[^3]. C has a special case: a file-scope declaration of an object without an initializer and without `extern` is a tentative definition. If the file has no other definition, the declaration acts as a definition with a zero value[^2].

**Declaration without definition.** A declaration can name a thing that another unit gives. In C and C++, an `extern` declaration or a prototype in a header file names a variable or a function that another file defines, and the linker joins them. In Rust, "External blocks provide declarations of items that are not defined in the current crate"[^10]. In C#, "A partial member has one declaring declaration and often one implementing declaration. The declaring declaration doesn't include a body"[^11].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `def`, `class`, `global`, `nonlocal`, annotations; no declaration of a variable |
| JavaScript | native | `function`, `class`, `var`, `let`, `const`; hoisting |
| TypeScript | native | as JavaScript; type annotations; `declare` and `.d.ts` files |
| Java | native | members in any order; local variables before use; `var` |
| Kotlin | native | `val`, `var`, `fun`; `lateinit`; `expect` and `actual` |
| C# | native | members in any order; local functions; `partial` members; `out var` |
| Go | native | `var`, `const`, `type`, `func`, `:=`; top level in any order |
| Rust | native | items in any order; `let`; `extern` blocks |
| C | native | declare before use; prototypes; `extern`; tentative definitions |
| C++ | native | forward declarations; one definition rule; `inline` variables |
| Bash | native | `declare`, `local`, `readonly`; function definitions |
| PowerShell | native | `function`, `param`; typed variables such as `[int]$n` |
| SQL | native | `CREATE` statements declare schema objects; `DECLARE` in procedural SQL |
| Regex | idiom | named groups declare a name for a group |
| HTML | idiom | the `id` attribute declares a name that links and labels refer to |
| CSS | native | `@property` declares a custom property with a type and an initial value |

Every general-purpose language has declarations. The verdict is native for all of them, and the languages differ in three things: whether a variable must be declared, whether a name can be used before its declaration, and whether a declaration can exist without a definition.

The example files show these differences. Python shows `global`, `nonlocal`, an annotation without a value and a conditional `def`. JavaScript shows the hoisting of a function, a `var`, a `class` and a function expression. Java uses a method before its declaration and declares a local variable that the compiler checks on each path. C# calls a local function before its declaration and joins the two declarations of a `partial` method. Go puts top-level declarations after `main` and shows `iota` and the two forms of type declaration. Rust uses items before their declarations and calls the C function `abs` through an `extern` block. C declares a variable and a function before it defines them and shows a tentative definition. C++ shows a forward declaration of a type and the declaration and definition of a static data member.

## Variations and related ideas

- **Forward declaration.** A declaration before the definition lets two functions or two types refer to each other. In C, `struct node;` declares a type whose parts are not known yet, and a pointer to it is valid.
- **Header files and interface files.** "Declarations are often collected into header files"[^1]. A C header, a TypeScript `.d.ts` file and the declaring part of a C# `partial` member all give declarations for code that is defined in another place.
- **Implicit declaration.** Some languages declare a name when the program first uses it. In FORTRAN, a variable whose name starts with I, J, K, L, M or N is an integer unless a declaration says otherwise. Old C let a program call a function without a declaration and assumed that it returned `int`.
- **Type inference.** `var` in Java and C#, `auto` in C++, `:=` in Go and `let` in Rust declare a variable and let the compiler find its type from the value.
- **Declaration and binding.** A declaration usually makes a [binding](/binding/). In Python, the binding comes from an assignment, and a declaration such as `global` only changes where the binding goes.

## Pitfalls

- **A declaration without a definition.** A prototype or an `extern` declaration that has no definition in the program compiles, but the link fails with an undefined reference.
- **Two definitions.** Two definitions of one function or variable break the one definition rule. Some cases give a link error, and some give a program that is wrong with no error[^3].
- **Declarations that do not agree.** If one C file declares `extern int count;` and another defines `double count;`, the compiler cannot see the problem. Put the declaration in a header file and include the header in the file that defines the name.
- **Hoisting that hides an error.** A `var` that is read before its assignment gives `undefined` and not an error. Use `let` and `const`, which give a `ReferenceError`.
- **An annotation that looks like a definition.** In Python, `total: int` does not make the name, so a read of `total` raises `NameError`.
- **A forward declaration that is too weak.** An incomplete type can be used only through a pointer or a reference. The code that reads its fields or makes an object of it needs the full definition.

## Cost

A declaration costs nothing at run time in compiled languages. It gives information to the compiler and the linker. The cost is at build time: a C or C++ file that includes many headers makes the compiler read many declarations again for each file, and a forward declaration in place of an include can make the build faster. In Python, a `def` or a `class` is a statement that runs, so it costs a little time each time it runs, for example a `def` inside a loop.

## History

The ALGOL 60 report, from the conference in Paris in January 1960, has a section named Declarations. "Declarations serve to define certain properties of the quantities used in the program, and to associate them with identifiers. A declaration of an identifier is valid for one block"[^12]. This joined declarations to the block structure, which most later languages kept.

C made the split between declaration and definition exact, because a C program is compiled one file at a time and joined by a linker. The current C standard still defines a definition as a declaration that reserves storage or gives a function body, and still has tentative definitions[^2]. C++ added the one definition rule[^3]. Later languages such as Java, Go and Rust let a program use a member or an item before its declaration[^4][^5][^6], and Java "does not require separate declarations and definitions"[^1].

## References
