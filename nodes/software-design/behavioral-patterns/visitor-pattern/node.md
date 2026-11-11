---
tag: JQCQK2
kind: pattern
slug: visitor-pattern
name: Visitor pattern
aliases: [Visitor, double dispatch, tree walker]
area: software-design/behavioral-patterns
level: core
summary: >-
  An operation on an object structure becomes a separate object with one method for each kind of element. Operations are then cheap to add, and kinds of element are expensive to add.
family: behavioral-design
polarity: positive
intent: "Represent an operation to be performed on the elements of an object structure, so that an operation can be added without a change to the classes of the elements."
also_known_as: []
refactorings: [move-accumulation-to-visitor]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with the operations of a compiler on an abstract syntax tree as the motivating example.
see_also: [python/match-statement, composite-pattern, strategy-pattern, iterators, recursion, static-and-dynamic-typing]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Visitor (Intent, Motivation, Applicability, Participants, Collaborations, Consequences, Implementation, Related Patterns)
  - url: https://homepages.inf.ed.ac.uk/wadler/papers/expression/expression.txt
    title: Philip Wadler, The Expression Problem, email to the java-genericity mailing list, 12 November 1998
    locator: opening section and section 1, A solution
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (Visitor is among the patterns affected by first-class functions), slide 23 (The Type/Operation Matrix), slide 24 (Multimethods) and slide 61 (Class-Oriented or Class-Obsessed?)
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/ast.py#L482-L516
    title: CPython v3.14.0 source, Lib/ast.py
    locator: class NodeVisitor, lines 482 to 516
  - url: https://docs.python.org/3.14/library/typing.html#typing.assert_never
    title: The Python Standard Library, typing, Python 3.14
    locator: typing.assert_never
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.11.1.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.11.1.1 Exhaustive Switch Blocks
  - url: https://openjdk.org/jeps/441
    title: JEP 441, Pattern Matching for switch
    locator: Description, Exhaustiveness of switch expressions and statements, and Exhaustiveness and sealed classes
  - url: https://doc.rust-lang.org/book/ch06-02-match.html#matches-are-exhaustive
    title: The Rust Programming Language, chapter 6.2, The match Control Flow Construct
    locator: Matches Are Exhaustive
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#exhaustiveness-checking
    title: TypeScript Handbook, Narrowing
    locator: Exhaustiveness checking
  - url: https://eel.is/c++draft/variant.visit
    title: Working Draft, Programming Languages, C++
    locator: "[variant.visit] Visitation"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/nio/file/FileVisitor.html
    title: Java SE 25 API, java.nio.file.FileVisitor
    locator: interface description
  - url: https://pkg.go.dev/go/ast#Walk
    title: Go standard library, package go/ast
    locator: func Walk and type Visitor
  - url: https://docs.python.org/3.14/library/functools.html#functools.singledispatch
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.singledispatch
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 10, Accumulation, Move Accumulation to Visitor
---

## Intent

Represent an operation on the elements of an object structure as an object. A program can then add an operation and not change the classes of the elements.[^1] Each visitor holds one operation, with one method for each kind of element.

## Problem

A compiler holds a program as an abstract syntax tree. The tree has a class for each kind of node: assignments, variable references, arithmetic expressions. The compiler needs many operations on the tree: type checking, code generation, pretty printing, metrics. Design Patterns starts from this example.[^1]

The first design puts each operation in the node classes as a method. Then each node class has a method for type checking, a method for code generation, and a method for each other operation. The code for one operation is spread over all the classes. To add an operation, a programmer must change every node class. And the node classes mix unrelated concerns.

The forces are these. The node classes rarely change. The operations change often, and each operation should be in one place. And an operation must run different code for each kind of node, without a chain of type tests.

## Solution

Design Patterns names five participants.[^1]

- **Visitor** declares a visit operation for each class of ConcreteElement.
- **ConcreteVisitor** implements each visit operation. It is one operation of the program, and it can keep state while it runs.
- **Element** declares an `accept` operation that takes a visitor.
- **ConcreteElement** implements `accept` by calling the visit operation for its own class.
- **ObjectStructure** holds the elements and can let a visitor visit each of them.

### Double dispatch

A call such as `node.op()` picks the method by one class, the class of `node`. The Visitor pattern needs the code to depend on two classes: the kind of node and the operation. It gets this with two calls. `node.accept(visitor)` picks a method by the class of the node. That method calls `visitor.visitAdd(this)`, which picks a method by the class of the visitor. Design Patterns calls this double dispatch.[^1] The JavaScript and the first Java example use it.

### The expression problem

The examples hold the expression `(2 + 3) * 4` as a tree of `Num`, `Add` and `Mul` nodes. They run three operations: `show`, `evaluate` and `count`. Think of the program as a table. Each kind of node is a row, and each operation is a column. Norvig draws this matrix of types and operations.[^3]

| Change | Methods in the node classes | Visitors, or a sum type and a match |
|---|---|---|
| Add an operation | change every class | add one visitor or one function |
| Add a kind of node | add one class | change every visitor or function |

Wadler named this the expression problem. The goal is to define a data type by cases, and then add cases and add functions without recompiling the existing code and without casts. In his words, cases are rows and functions are columns. In a functional language, the rows are fixed, and a column can be added without a change to the existing code. In an object-oriented language, the columns are fixed, and a row can be added in the same way.[^2] The Visitor pattern moves an object-oriented program to the functional side of the table. Design Patterns says the same: with visitors, an operation is cheap to add and an element class is expensive to add.[^1] Neither design makes both changes cheap. Wadler cites earlier work by Reynolds (1975), Cook (1990), and Krishnamurthi, Felleisen and Friedman (1998), and his own solution in GJ uses a `Visitor` interface.[^2]

### Sum types and pattern matching

A language with sum types and pattern matching has the functional side of the table as a built-in form. A sum type, such as a Rust `enum` or a Java `sealed` interface with records, lists every kind of node. A match on it is an operation with a case for each kind. No `accept` method and no visitor interface are needed.

The compiler can check that a match covers every case. A Rust `match` must cover every variant.[^8] A Java `switch` over a sealed type with no `default` must cover each permitted subtype.[^6][^7] TypeScript checks a discriminated union when the `default` case assigns the value to a variable of type `never`.[^9] C++ `std::visit` does not compile when an alternative of the `std::variant` has no overload.[^10] With these checks, a kind of node added to the type turns each operation that lacks it into a compile error. The classic Visitor gives the same check, because each visitor class must implement every method of the interface.

In Python, a [[python/match-statement|match statement]] runs, but nothing checks it before it runs. This program adds `Neg` to the type and forgets to add a case to `show`:

```python wrong id=missing-case
from dataclasses import dataclass


@dataclass
class Num:
    value: int


@dataclass
class Neg:
    operand: "Expr"


type Expr = Num | Neg


def show(e: Expr) -> str:
    match e:
        case Num(value):
            return str(value)


print("show:", show(Neg(Num(2))))
```

```output for=missing-case
show: None
```

No case matches, so the `match` does nothing, and the function returns `None`. The program prints a wrong result and does not stop. The fix is a last case that calls `typing.assert_never`. A static type checker reports an error when it finds that the call can be reached. At run time the call raises an exception.[^5]

```python fails expect=exception:AssertionError id=assert-never
from dataclasses import dataclass
from typing import assert_never


@dataclass
class Num:
    value: int


@dataclass
class Neg:
    operand: "Expr"


type Expr = Num | Neg


def show(e: Expr) -> str:
    match e:
        case Num(value):
            return str(value)
        case _:
            assert_never(e)


print("show:", show(Neg(Num(2))))
```

### Who traverses the structure

Design Patterns gives three places for the traversal: the object structure, the visitor, or a separate iterator.[^1] In the examples, most operations recurse on their own. The TypeScript example puts the traversal in one `fold` function, and each visitor receives the results of the children. Go's `ast.Walk` holds the traversal, and `Visit` returns the visitor to use for the children, or `nil` to skip them.[^12] Python's `ast.NodeVisitor` has `generic_visit`, which visits the children of a node.[^4]

## Example

| Language | Form |
|---|---|
| Python | dataclasses and `match`; a second program with `ast.NodeVisitor` |
| JavaScript | classes with `accept`, and visitors as objects of functions |
| TypeScript | a discriminated union, a `fold`, and visitors as typed records of functions |
| Java | a generic `Visitor<R>` with `accept`; a second program with a sealed interface and record patterns |
| C# | records and `switch` expressions with positional patterns |
| Go | an interface with an unexported method, and type switches |
| Rust | an enum and `match` |
| C++ | `std::variant` and `std::visit` with a set of lambdas |

## Consequences

Design Patterns lists these consequences.[^1] With visitors, an operation is cheap to add. A visitor gathers the code of one operation and separates unrelated operations. Adding a ConcreteElement class is hard, because each visitor needs a method for it. A visitor can visit classes that have no common parent, which an iterator cannot do. A visitor can accumulate state as it visits, such as a symbol table, so that the state does not travel in arguments. And a visitor breaks encapsulation, because the elements must give it enough access to do its work.

The pattern also has costs that come from the language. In a language with single dispatch, the `accept` methods are boilerplate that exists only to recover the class of the node. Norvig says that C++ encourages bookkeeping classes, and that the Visitor pattern serves only to get around this restriction.[^3] A visitor that keeps its result in a field cannot be called again while it runs, unless it saves and restores the field. A generic visitor that returns a value, such as `Visitor<R>` in the Java example, avoids this.

## When not to use it

Do not use a visitor when the kinds of element change more often than the operations. Each added class then changes every visitor. Put the operations in the classes, as methods.

Do not write `accept` methods in a language with sum types and pattern matching. An enum and a `match` give the same separation, with less code and an exhaustiveness check.

Do not use a visitor for one operation. A function with a match, or a method on each class, is shorter. A visitor pays off when there are many operations on a stable set of classes.

## Variants and related patterns

**Functions as visitors.** Norvig lists Visitor among the four patterns that first-class functions affect.[^3] A visitor can be a record of functions, as in the JavaScript and TypeScript examples. Python's `functools.singledispatch` picks a function by the type of its first argument, which is single dispatch without `accept`.[^13] Norvig's slide on multimethods shows dispatch on the classes of more than one argument, which removes the need for double dispatch.[^3]

**Reflective dispatch.** Python's `ast.NodeVisitor.visit` builds the method name `'visit_'` plus the class name of the node and looks it up with `getattr`. A node with no such method goes to `generic_visit`.[^4] The visitor needs no interface, and the nodes need no `accept`. The cost is that a misspelled method name is never called, and nothing reports it.

**Composite and Interpreter.** Design Patterns relates Visitor to Composite, because a visitor can apply an operation over a composite structure, and to Interpreter, because a visitor can do the interpretation.[^1] See [[composite-pattern]].

**Iterator.** An iterator visits the elements of a structure in order and gives them to the caller. A visitor runs code that depends on the kind of each element. A walk such as Java's `Files.walkFileTree` combines both: it traverses the tree and calls a `FileVisitor` for each directory and file.[^11] See [[iterators]].

**Strategy.** A visitor is a strategy with one method for each kind of element. See [[strategy-pattern]].

**Refactoring.** Kerievsky's Move Accumulation to Visitor moves code that gathers data from many classes into a visitor.[^14]

## Known uses

- CPython: [`ast.NodeVisitor` at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/ast.py#L482-L516) dispatches on the class name of each node of a Python syntax tree.[^4]
- Go: `go/ast.Walk` traverses a Go syntax tree in depth-first order and calls a `Visitor` for each node.[^12]
- Java: `java.nio.file.FileVisitor` is called by `Files.walkFileTree` before and after each directory and for each file.[^11]
- C++: `std::visit` applies a callable to the alternative that a `std::variant` holds.[^10]

## References
