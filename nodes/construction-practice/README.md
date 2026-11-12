This area is about writing code that other people can read and change. It covers the daily habits of a programmer: naming, comments, style, refactoring, code review and work with old code.

## What it holds

- **Construction fundamentals.** What software construction is and the habits that a programmer brings to every line: planning, small steps, feedback and care.
- **Names in code.** How to choose names for variables, functions, types, modules and files, and the naming conventions of the main languages.
- **Readable code.** How to write code that a reader understands fast: control flow, function size, expressions and the cost of cleverness.
- **Comments and documentation in code.** How and when to write comments and documentation comments, and how documentation inside the code base stays true.
- **Style and formatting.** Layout, coding standards, style guides and the automatic formatters of the tier 1 languages.
- **Code smells.** The surface signs that code needs refactoring. The 24 smells of *Refactoring* 2nd edition first, then the two smells of the 1st edition that the 2nd edition dropped, then the other named smells.
- **Code-level anti-patterns.** Named bad habits in code that are not in the Fowler smell catalogue. Design and architecture anti-patterns are in software-design/design-anti-patterns.
- **Refactoring fundamentals.** What refactoring is, when and how to do it, and the tools that automate it. The catalogue of refactorings follows in six sub-areas, in the chapter order of *Refactoring* 2nd edition.
- **A first set of refactorings.** The eleven refactorings that *Refactoring* 2nd edition calls the most useful set (chapter 6).
- **Encapsulation refactorings.** The refactorings of chapter 7 that hide data and decisions behind interfaces.
- **Moving features.** The refactorings of chapter 8 that move functions, fields and statements to a better place.
- **Organizing data and simplifying conditional logic.** The refactorings of chapters 9 and 10 that organize data and simplify conditional logic, with Replace Magic Literal and Replace Control Flag with Break from the online catalogue.
- **Refactoring APIs.** The refactorings of chapter 11 that change the interfaces of functions and classes, with the error-handling refactorings of the online catalogue.
- **Dealing with inheritance.** The refactorings of chapter 12 that move features up and down a class hierarchy or replace inheritance with delegation.
- **Large-scale refactoring and codemods.** Changes too big for one commit or one team: large-scale changes, automated code rewriting and migrations across a code base.
- **Code review.** How teams review code before it merges: the goals, the process, the reviewer and author roles, and the formal inspection methods.
- **Technical debt.** The metaphor of technical debt, its kinds and how teams find, track and pay it down.
- **Defensive coding.** Coding that protects a program from bad input and from its own bugs, and the choice between failing fast and keeping on.
- **Legacy code.** How to change code that has no tests: Feathers's method, characterization tests and seams.
- **Dependency-breaking techniques.** The 24 techniques of chapter 25 of *Working Effectively with Legacy Code* that get a class or function into a test harness.
- **Pair and mob programming.** Working on one piece of code with two or more people at the same time, and the shared ownership that goes with it.

## What it does not hold

- Design at the level of modules and systems. It is in [Software design](/software-design/).
- Documentation tools and project documentation. They are in [Tools and workflow](/tools-and-workflow/) and [Software engineering process](/software-engineering-process/).
- Linters as analysis tools. They are in [Program analysis and verification](/program-analysis-and-verification/).
- Tests. They are in [Testing](/testing/).
