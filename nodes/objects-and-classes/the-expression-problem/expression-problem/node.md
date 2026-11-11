---
tag: 656G08
kind: concept
slug: expression-problem
name: Expression problem
aliases: [the expression problem, extensibility problem, two-dimensional extensibility]
area: objects-and-classes/the-expression-problem
level: core
summary: >-
  The expression problem asks how to add both new cases and new operations to a data type without changing existing code and with static checks. Classes make one easy, pattern matching the other.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1998
  people: [Philip Wadler, John C. Reynolds, William R. Cook]
  note: Wadler named the problem in 1998, from the comparison of data abstraction styles by Reynolds in 1975 and Cook in 1990.
see_also: [dynamic-dispatch, sum-types, pattern-matching, visitor-pattern, interfaces, class-inheritance, object-oriented-programming, functional-programming]
same_as:
  wikidata: Q5421707
  wikipedia: Expression_problem
sources:
- url: https://homepages.inf.ed.ac.uk/wadler/papers/expression/expression.txt
  title: Philip Wadler, The Expression Problem, message to the java-genericity mailing list, 12 November 1998
  locator: The opening paragraphs
- url: https://doc.rust-lang.org/reference/items/implementations.html
  title: Implementations, The Rust Reference
  locator: Trait implementation coherence; Orphan rules
- url: https://clojure.org/reference/protocols
  title: Protocols, Clojure Reference
  locator: Motivation
---

## Idea

The expression problem asks how to extend a data type in two directions at once: with new cases, and with new operations over all the cases, without changing or recompiling the existing code and without giving up static type safety. Philip Wadler named it in 1998: "The goal is to define a datatype by cases, where one can add new cases to the datatype and new functions over the datatype, without recompiling existing code, and while retaining static type safety (e.g., no casts)."[^1] Object-oriented code makes new cases easy and new operations hard. Functional code with pattern matching makes new operations easy and new cases hard. Every language design picks a side, or adds a mechanism to get both.

```python run id=idea
from dataclasses import dataclass
from functools import singledispatch


@dataclass
class Num:  # the first two cases of the data type
    value: int


@dataclass
class Add:
    left: object
    right: object


@singledispatch  # an operation: one generic function, one method for each case
def evaluate(e):
    raise TypeError(f"evaluate has no case for {type(e).__name__}")


@evaluate.register
def _(e: Num):
    return e.value


@evaluate.register
def _(e: Add):
    return evaluate(e.left) + evaluate(e.right)


@singledispatch
def show(e):
    raise TypeError(f"show has no case for {type(e).__name__}")


@show.register
def _(e: Num):
    return str(e.value)


@show.register
def _(e: Add):
    return f"({show(e.left)} + {show(e.right)})"


print(show(Add(Num(1), Num(2))), "=", evaluate(Add(Num(1), Num(2))))


# Later, without a change to the code above: a new case ...
@dataclass
class Neg:
    inner: object


evaluate.register(Neg, lambda e: -evaluate(e.inner))
show.register(Neg, lambda e: f"-{show(e.inner)}")


# ... and a new operation.
@singledispatch
def depth(e):
    raise TypeError(f"depth has no case for {type(e).__name__}")


depth.register(Num, lambda e: 1)
depth.register(Add, lambda e: 1 + max(depth(e.left), depth(e.right)))
depth.register(Neg, lambda e: 1 + depth(e.inner))

e = Add(Num(1), Neg(Add(Num(2), Num(3))))
print(show(e), "=", evaluate(e), "depth", depth(e))


@dataclass
class Mul:  # a case that nobody taught evaluate
    left: object
    right: object


try:
    evaluate(Mul(Num(2), Num(3)))
except TypeError as err:
    print(err)
```

```output for=idea
(1 + 2) = 3
(1 + -(2 + 3)) = -4 depth 4
evaluate has no case for Mul
```

The data type is a small language of arithmetic expressions. Each operation is a generic function from `functools.singledispatch`, which chooses a method by the type of its argument, so the cases and the operations are separate and either side can grow. The code after the first `print` adds the case `Neg` and the operation `depth` without a change to the code above it. The cost shows at the end: Python itself checks nothing until the program runs, so a case that an operation does not know about fails only when the operation reaches it. Wadler's problem asks for the same freedom with the check done by the compiler.

## How it works

**A table of cases and operations.** Write the cases of the data type as the rows of a table and the operations as its columns. Each cell is the code for one operation on one case. Wadler described the two usual layouts: "In a functional language, the rows are fixed (cases in a datatype declaration) but it is easy to add new columns (functions). In an object-oriented language, the columns are fixed (methods in a class declaration) but it is easy to add new rows (subclasses). We want to make it easy to add either rows or columns."[^1] A program keeps the table in one of two orientations, and the orientation decides which kind of change touches existing code.

**The object-oriented side.** Each case is a class, and each operation is a method that every class implements. A new case is a new class, written in one place, with no change to the others. A new operation is a new method in the base type, which every existing class must implement, so the change spreads over every case, often across modules that belong to other people. See [dynamic dispatch](/dynamic-dispatch/) and [class inheritance](/class-inheritance/).

**The functional side.** The cases are the variants of a [sum type](/sum-types/), and each operation is a function that uses [pattern matching](/pattern-matching/) over the variants. A new operation is one new function. A new case is a new variant, and every function that matches over the type must add a branch for it. A compiler that checks exhaustiveness, as in Rust, Java with sealed types, Haskell or OCaml, lists every function to change, but the change still reaches code that was finished.

**The visitor pattern moves the problem.** In an object-oriented language, the [visitor pattern](/visitor-pattern/) puts each operation in a class of its own, with one method for each case. This turns the class hierarchy into the functional layout: a new operation is a new visitor, and a new case adds a method to the visitor interface and so to every visitor. The pattern changes which direction is easy but does not make both easy.

**Static type safety is the hard part.** A dynamic language can extend both sides with open classes, registries or generic functions, as the Python example does, and report a missing cell at run time. The problem asks for more: the compiler must reject a program in which some operation has no code for some case, and the extension must happen in new modules that the compiler checks separately. Wadler treated this as a test of a language: "Whether a language can solve the Expression Problem is a salient indicator of its capacity for expression."[^1]

**Mechanisms that solve it.** Several language features give both directions with static checks. Type classes and traits keep each operation as its own interface, with an implementation for each case type, written anywhere: a new operation is a new trait, and a new case is a new type with impls of the existing traits. Rust limits where impls may appear, to keep one impl for each pair of trait and type: "The orphan rule states that a trait implementation is only allowed if either the trait or at least one of the types in the implementation is defined in the current crate. It prevents conflicting trait implementations across different crates and is key to ensuring coherence."[^2] Clojure protocols have the same goal, to "Avoid the 'expression problem' by allowing independent extension of the set of types, protocols, and implementations of protocols on types, by different parties".[^3] Generic programming with type parameters, as in Wadler's own solution in Generic Java, and object algebras give further solutions in languages with classes.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | classes; `functools.singledispatch`; checks at run time |
| JavaScript | idiom | open prototypes; methods keyed by symbols |
| TypeScript | idiom | discriminated unions with exhaustive `switch` |
| Java | idiom | classes; visitors; sealed interfaces with `switch` |
| Kotlin | idiom | sealed classes with `when`; extension functions |
| C# | idiom | classes; extension methods with `switch` expressions |
| Go | idiom | implicit interfaces; type switches |
| Rust | idiom | one trait for each operation; enums with `match` |
| C | idiom | tagged unions; tables of function pointers |
| C++ | idiom | virtual functions; `std::variant` with `std::visit` |
| Bash | n/a | no user-defined data types |
| PowerShell | idiom | `Update-TypeData` adds members to existing types |
| SQL | n/a | no user-defined sum types or dispatch |
| Regex | n/a | no data types |
| HTML | n/a | no data types |
| CSS | n/a | no data types |

No core language of this site solves the expression problem with one feature, and each gives a set of partial solutions. Every example file builds the same small language of numbers, addition and negation, adds a case and an operation, and shows where the language makes one of the two changes reach existing code. The failing and wrong blocks show the side that is closed in each language: a `switch` that misses a case, a sealed type, a method that cannot go on a foreign type, or a table with a hole.

The languages fall into three groups. Python and JavaScript extend both sides with no static check, by registries and open prototypes. Java, C#, C++, Kotlin and TypeScript give the object-oriented layout through classes and the functional layout through sealed types or variants, and a program picks one layout for each data type. Rust, with traits, comes closest to a full solution among the core languages. Go lets a new case come from any package, because a type satisfies an interface without a declaration, but its type switches have no exhaustiveness check. Outside the core set, Haskell type classes, Clojure protocols and the multimethods of Common Lisp and Julia give this kind of extension directly.

## Variations and related ideas

- **The visitor pattern.** Puts each operation in a class with one method for each case, which makes new operations easy in an object-oriented language and new cases hard.
- **Sealed types and exhaustive matching.** Close the set of cases on purpose, so that the compiler can check every operation. This is the functional side of the problem, chosen for data whose cases are known.
- **Type classes and traits.** Separate each operation from the data, with implementations written for each type. Haskell type classes, Rust traits and Swift protocols with extensions work this way. See [interfaces](/interfaces/).
- **Multimethods and open generic functions.** Choose code by the run-time types of one or more arguments, and let any module add methods. CLOS, Julia and Clojure have them, and Python's `singledispatch` gives a form with one argument.
- **Clojure protocols.** Named sets of functions that any type, including a type from Java, can implement after the fact.
- **Object algebras.** Bruno Oliveira and William Cook showed in 2012 a solution in Java and C# that uses only generic interfaces, with one factory interface for the cases and one implementation for each operation.
- **Tagless final.** Represents a term of a language as a function that is generic over its interpretation, so new interpreters and, with type classes, new constructs can be added separately.
- **Open classes and monkey patching.** Ruby, JavaScript and Objective-C categories let any module add methods to existing classes, which solves both sides at run time with the risk of name clashes.

## Pitfalls

- **A switch that misses a new case.** In a language without exhaustiveness checks, a function over the cases returns a default value or nothing for a new case. The wrong blocks in the Python and JavaScript files show this. End every such function with a branch that raises an error.
- **A default branch in an exhaustive match.** A `_` or `default` arm turns off the check that would find the missing case when someone adds a variant. Leave it out when the cases are meant to be closed.
- **Static binding mistaken for dispatch.** Extension methods in C# and Kotlin, and overloads in Java, choose code by the static type of the argument, so an overload for a new case does not run through a variable of the base type. The wrong block in the C# file shows this.
- **Name clashes in open extension.** Two modules that add a method of the same name to the same class overwrite each other. Use symbols, namespaced names or traits with coherence rules.
- **The orphan rule.** In Rust, code cannot implement a trait from one crate for a type from another. Wrap the type in a local newtype. The failing block in the Rust file shows the error.
- **Choosing the wrong side.** Most data types grow mainly in one direction. Pick the layout that makes the common change local: classes for a set of plug-ins that grows, a closed sum type for a syntax tree that many passes read.

## Cost

The mechanisms differ little in run time. A virtual call or an interface call costs an indirect jump, and a `match` on a sum type costs a jump through a table or a short series of comparisons. A visitor costs two indirect calls for each node. A registry such as `singledispatch` costs a lookup in a dictionary, which it caches. Generic solutions with traits or type parameters cost nothing at run time when the compiler specializes them, but they increase compile time and code size. The larger cost is in maintenance: the side that a design closes decides how many files a common change touches.

## History

Cases and operations as two dimensions of a program appear in John Reynolds's 1975 paper on user-defined types and procedural data structures, which compared abstract data types with procedural data abstraction, the style of objects. William Cook returned to the comparison in 1990, and Shriram Krishnamurthi, Matthias Felleisen and Daniel Friedman described the extensibility problem and an extension of the visitor pattern in 1998. Philip Wadler gave the problem its name in a message to the java-genericity mailing list on 12 November 1998, writing that "The Expression Problem is a new name for an old problem", and proposed a solution in Generic Java.[^1]

Many solutions followed. Martin Odersky and Matthias Zenger solved it in Scala with abstract types and mixins in 2005, Bruno Oliveira and William Cook proposed object algebras in 2012, and the tagless final style of Jacques Carette, Oleg Kiselyov and Chung-chieh Shan used type classes. Clojure added protocols in version 1.2 in 2010, with the expression problem as a stated motivation.[^3] The coherence rules of Rust traits answer a part of the same question: which crate may add the code for a given pair of trait and type.[^2]

## References
