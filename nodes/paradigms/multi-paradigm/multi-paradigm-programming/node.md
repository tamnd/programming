---
tag: HNXXA1
kind: concept
slug: multi-paradigm-programming
name: Multi-paradigm programming
aliases: [multiparadigm programming, multi-paradigm language, hybrid language, mixed paradigms, functional core imperative shell]
area: paradigms/multi-paradigm
level: core
summary: >-
  A multi-paradigm language supports several paradigms, such as imperative, object-oriented and functional, so that each part of a program can use the style that fits it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: [Timothy Budd, Bjarne Stroustrup, Peter Van Roy]
  note: Budd's book on Leda named multiparadigm programming in 1995, the year Stroustrup called C++ a multi-paradigm language.
see_also: [imperative-programming, object-oriented-programming, functional-programming, declarative-programming, procedural-programming, logic-programming, closures, immutability]
same_as:
  wikidata: Q4306983
  wikipedia: Programming_paradigm
sources:
- url: https://webperso.info.ucl.ac.be/~pvr/VanRoyChapter.pdf
  title: 'Peter Van Roy, Programming Paradigms for Dummies: What Every Programmer Should Know, 2009'
  locator: section 1, Introduction
- url: https://docs.python.org/3/howto/functional.html
  title: 'Python documentation: Functional Programming HOWTO'
  locator: Introduction
- url: https://www.stroustrup.com/oopsla.pdf
  title: Bjarne Stroustrup, Why C++ is not just an Object-Oriented Programming Language, OOPSLA 1995
  locator: abstract; section 4
---

## Idea

A multi-paradigm language supports more than one programming paradigm, so a programmer can write each part of a program in the style that fits it. Most languages in wide use today are multi-paradigm: Python, JavaScript, Java, C#, C++, Kotlin, Rust, Scala and Swift all combine imperative statements, objects with dynamic dispatch, and first-class functions. Multi-paradigm programming is the practice of choosing and combining these styles inside one program, for example a pure function to compute a result, a loop to update a table, and an interface to vary the output.

```python run id=idea
from abc import ABC, abstractmethod
from dataclasses import dataclass


@dataclass(frozen=True)
class Order:  # object-oriented: a value type with named fields
    customer: str
    amount: int


def parse(line):  # functional: a pure function from text to a value
    name, amount = line.split(",")
    return Order(name.strip(), int(amount))


def totals(orders):  # imperative: a loop that updates a dictionary
    out = {}
    for o in orders:
        out[o.customer] = out.get(o.customer, 0) + o.amount
    return out


class Report(ABC):  # object-oriented: one interface, many formats
    @abstractmethod
    def line(self, name, total): ...


class Plain(Report):
    def line(self, name, total):
        return f"{name} {total}"


class Csv(Report):
    def line(self, name, total):
        return f"{name},{total}"


text = "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5"
orders = [parse(line) for line in text.splitlines()]
big = sorted(((n, t) for n, t in totals(orders).items() if t > 50),
             key=lambda pair: -pair[1])  # declarative: filter and sort
for report in (Plain(), Csv()):
    for name, total in big:
        print(report.line(name, total))
```

```output for=idea
bo 75
ana 55
bo,75
ana,55
```

The program uses four styles in thirty lines. `Order` is an immutable value, `parse` is a pure function, `totals` is a plain loop over mutable state, the comprehension and `sorted` state what to keep and in which order, and `Report` lets the same data print in two formats through dynamic dispatch. Each style does the job that it does best, and no part forces its style on the others.

## How it works

**A paradigm is a set of concepts.** Peter Van Roy defines it this way: "A programming paradigm is an approach to programming a computer based on a mathematical theory or a coherent set of principles."[^1] Each paradigm rests on a few concepts: mutable state and sequence for [imperative programming](/imperative-programming/), objects and dispatch for [object-oriented programming](/object-oriented-programming/), functions as values and immutability for [functional programming](/functional-programming/), relations and search for [logic programming](/logic-programming/). A language is multi-paradigm when it gives the concepts of more than one paradigm.

**Different parts need different concepts.** Van Roy argues: "Solving a programming problem requires choosing the right concepts. All but the smallest toy problems require different sets of concepts for different parts."[^1] The Python documentation gives an example: "In a large program, different sections might be written using different approaches; the GUI might be object-oriented while the processing logic is procedural or functional, for example."[^2] A common modern pattern is a functional core with an imperative shell: pure functions compute the decisions, and a thin layer of imperative code reads input, writes output and holds state.

**Concepts that compose.** The concepts of a multi-paradigm language must work together. A lambda can capture an object, a method can return a function, a class can implement an interface for iteration, and a loop can run over the result of a pipeline. Van Roy states the goal: "A language should ideally support many concepts in a well-factored way, so that the programmer can choose the right concepts whenever they are needed without being encumbered by the others."[^1] Where the concepts meet badly, bugs appear, as when a closure captures a loop variable that the loop keeps changing. The wrong blocks of the Python, JavaScript and C# files show this.

**Styles have a default.** Even a multi-paradigm language makes one style easier than the others. Python and Go favor the imperative style, Java favors classes, Scala and F# favor functional code, and C++ leaves the choice to the programmer. Bjarne Stroustrup wrote of C++: "C++ directly supports a variety of programming styles. In this, C++ deliberately differs from languages designed to support a single way of writing programs."[^3] He preferred the label: "If I must apply a descriptive label, I use the phrase “multi-paradigm language” to describe C++."[^3]

**How languages became multi-paradigm.** Few languages start this way. Most begin with one paradigm and add the concepts of others over time: Java and C# added lambdas, streams, records and pattern matching to a class-based core; C++ added templates, lambdas and ranges to C; JavaScript added classes, modules and `async` functions to a language with closures and prototypes. Each addition must fit the existing syntax and semantics, which is why the same concept looks different in each language.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | statements, classes, first-class functions, comprehensions |
| JavaScript | native | closures, prototypes and classes, the event loop |
| TypeScript | native | as JavaScript, with structural types |
| Java | native | classes and interfaces, lambdas, streams, records |
| Kotlin | native | classes, data and sealed classes, lambdas, coroutines |
| C# | native | classes, LINQ, records, pattern matching, `async` |
| Go | native | structs and implicit interfaces, closures, goroutines |
| Rust | native | traits, algebraic data types, closures, iterators |
| C | idiom | procedures; function pointers for callbacks and interfaces |
| C++ | native | procedural code, classes, templates, lambdas, ranges |
| Bash | idiom | commands and pipelines |
| PowerShell | native | scripts, an object pipeline, classes, script blocks |
| SQL | native | declarative queries with procedural extensions |
| Regex | n/a | one declarative notation |
| HTML | n/a | one declarative markup style |
| CSS | n/a | one declarative style of rules |

Every core language of this site is multi-paradigm except C, Bash and the three notations that have no paradigm to choose. Each example file solves the same task, the totals of customers whose orders exceed 50, with a value type, a pure function, a loop that updates a map, a filter and sort, and an interface with two formats. The files differ in how much each style costs to write. In Python, JavaScript and C# every style is short. Java puts every function in a class. Go has no filter function and writes a loop instead. Rust makes the shared and mutable parts explicit in the types. C writes an interface as a table of function pointers by hand.

Languages also differ in which paradigm is the default and which is a guest. Scala and F# make immutable values and expressions the default and allow mutation. OCaml has objects but most code uses modules and functions. Common Lisp combines functional code, a powerful object system, and macros that add new styles to the language.

## Variations and related ideas

- **Hybrid object-functional languages.** Scala, F#, OCaml, Kotlin and Swift combine classes or modules with algebraic data types, pattern matching and immutable values.
- **Lisp and its descendants.** Common Lisp, Scheme, Racket and Clojure let macros add new paradigms as libraries, so a program can define its own style. See [functional programming](/functional-programming/).
- **Oz and Mozart.** A research language built to support many paradigms in one coherent model, used in the textbook of Van Roy and Haridi.
- **Embedded languages.** A program can host a different paradigm as a language inside it, such as SQL queries, regular expressions or a rules engine. See [declarative programming](/declarative-programming/).
- **Polyglot programming.** A system can use several languages, each with its own paradigm, connected through processes, APIs or a shared runtime such as the JVM or .NET.
- **Functional core, imperative shell.** A design rule that keeps decisions in pure functions and effects at the edges. See [immutability](/immutability/).
- **Paradigm features in libraries.** Reactive streams, actors and logic solvers come as libraries in many languages. See [reactive programming](/reactive-programming/) and [the actor model](/actor-model/).

## Pitfalls

- **Inconsistent style.** When each author picks a different style for the same kind of code, readers must switch models at every file. Agree on which style to use for which part, and write the rule down.
- **Closures over changing state.** A closure captures a variable, not its current value, so closures made in a loop can all see the last value. The wrong blocks in the Python, JavaScript and C# files show this.
- **Mutation inside a functional pipeline.** A `map` or `filter` whose function changes shared state makes the result depend on the order and the number of calls, and breaks when the pipeline becomes lazy or parallel. Keep the functions of a pipeline pure.
- **Objects that hide shared state.** Passing an object into code written in a functional style can bring hidden mutable state with it. Use immutable values at the boundary.
- **Using a style the language does not support well.** Deep recursion in Python or Java can overflow the stack, because neither eliminates tail calls, and a long chain of immutable copies in a language without persistent collections costs time and memory. Choose the style the language makes cheap for code on a hot path.
- **Too many abstractions.** Combining class hierarchies, generic types and higher-order functions for a small task adds layers without benefit. Use the simplest style that solves the problem.

## Cost

Multi-paradigm design costs nothing at run time by itself. Each style has its own costs: a virtual call costs an indirect jump, a closure costs an allocation for its captured variables in most languages, an immutable update costs a copy or a persistent structure, and a lazy pipeline costs a small object for each stage. Compilers for C++, Rust and the JVM remove much of this cost by inlining. The main cost is for people: a language with many paradigms has more features to learn, and a team needs more rules to keep a code base readable.

## History

Early languages mixed ideas from the start. Lisp, from 1958, combined functions as values with assignment and loops, and Simula 67 added classes and objects to the procedural Algol 60. Around 1990, research languages such as Leda, by Timothy Budd, set out to unite imperative, object-oriented, functional and logic programming, and Budd's 1995 book named the practice multiparadigm programming. Stroustrup described C++ as a multi-paradigm language in a 1995 paper.[^3] Common Lisp, standardized by ANSI in 1994, combined functional code with the Common Lisp Object System.

Peter Van Roy and Seif Haridi built the Oz language and wrote *Concepts, Techniques, and Models of Computer Programming* in 2004, which presents paradigms as sets of concepts that a language can combine.[^1] Scala, released by Martin Odersky in 2004, and F#, by Don Syme at Microsoft Research in 2005, joined objects and functional programming on the JVM and .NET. From the late 2000s, mainstream languages added functional features: C# gained LINQ and lambdas in 2007, C++ gained lambdas in 2011 and Java in 2014, and most new languages, such as Kotlin, Swift and Rust, were multi-paradigm from their first release.

## References
