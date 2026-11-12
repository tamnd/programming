This area covers the ways to give a part of a program a name and hide its detail. The main tools are functions, closures and modules. The area also holds the advanced forms of control that functions make possible, such as generators and continuations.

## What it holds

- **Functions and procedures.** What a function is, how a program defines and calls one, and how it returns results.
- **Parameters and arguments.** How values go into a function: the parameter list, the passing modes, and the forms of arguments.
- **Recursion and tail calls.** Functions that call themselves, and how tail calls let recursion run in constant stack space.
- **Functions as values.** Functions that can be stored, passed and returned, and the techniques that this makes possible.
- **Closures.** Functions that keep the variables of the scope where they were made.
- **Currying and partial application.** Functions of several arguments as chains of one-argument functions, and fixing some arguments ahead of time. Small by nature.
- **Pure functions and effects.** Functions without side effects, the reasoning that they allow, and caching their results.
- **Lazy and strict evaluation.** When arguments and expressions are evaluated: at once, or only when needed.
- **Iterators, generators and coroutines.** Values produced one at a time, and functions that can pause and resume.
- **Continuations.** The rest of a computation as a value: continuation-passing style, first-class continuations and delimited continuations. Small by nature.
- **Functional abstractions.** The common shapes of functional code: functors, applicatives, monads, monoids, folds and traversals. CS2023 FPL-Functional and the core list (candidate "Monad") name them.
- **Lambda calculus and combinators.** The small formal language of functions behind functional programming. CS2023 FPL-Functional names it, and the core item "Lambda calculus" needs a home.
- **Modules, namespaces and packages.** How languages group code into units with names, imports and exports.
- **Abstraction and information hiding.** Abstraction as the main tool of programming, and information hiding as the rule that guides module interfaces.

## What it does not hold

- Async functions and event loops. They are in [Concurrency and parallelism](/concurrency-and-parallelism/).
- Package managers and the distribution of modules. They are in [Tools and workflow](/tools-and-workflow/).
- Methods and classes. They are in [Objects and classes](/objects-and-classes/).
- Recursive algorithms as algorithms, such as merge sort. They are on [the algorithm site](https://algorithm.tamnd.com/).
