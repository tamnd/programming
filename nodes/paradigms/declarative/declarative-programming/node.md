---
tag: 0HBVK7
kind: concept
slug: declarative-programming
name: Declarative programming
aliases: [declarative style, what not how, logic and control, nonprocedural language, declarative UI, declarative configuration]
area: paradigms/declarative
level: core
summary: >-
  Declarative programming describes the result a program must produce, such as a query, a pattern or a wanted state, and leaves the steps to an engine that decides how to compute it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1970
  people: [Edgar F. Codd, Robert Kowalski]
  note: Codd's relational model made a query a statement about data, and Kowalski separated the logic of an algorithm from its control.
see_also: [functional-programming, imperative-programming, annotations, pattern-matching, laziness, higher-order-functions]
same_as:
  wikidata: Q531152
  wikipedia: Declarative_programming
sources:
- url: https://www.sqlite.org/queryplanner.html
  title: Query Planning, SQLite documentation
  locator: 1 Overview
- url: https://www.doc.ic.ac.uk/~rak/papers/algorithm%20=%20logic%20+%20control.pdf
  title: Algorithm = Logic + Control, Robert Kowalski, Communications of the ACM 22(7), 1979
  locator: abstract
- url: https://www.seas.upenn.edu/~zives/03f/cis550/codd.pdf
  title: A Relational Model of Data for Large Shared Data Banks, E. F. Codd, Communications of the ACM 13(6), 1970
  locator: 1.1 Introduction
- url: https://developer.hashicorp.com/terraform/language
  title: Terraform Language Documentation, HashiCorp
  locator: About the Terraform Language
- url: https://react.dev/learn/reacting-to-input-with-state
  title: Reacting to Input with State, React documentation
  locator: How declarative UI compares to imperative
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/introduction-to-linq-queries
  title: Introduction to LINQ Queries in C#, Microsoft Learn
  locator: introduction
---

## Idea

Declarative programming describes the result that a program must produce, not the steps to produce it. The programmer writes a description: a query, a pattern, a set of rules or the wanted state of a system. An engine then decides how to compute it. The SQLite documentation states the idea for SQL: "When programming in SQL you tell the system what you want to compute, not how to compute it."[^1] The same split appears in regular expressions, HTML and CSS, build files, configuration languages and user interface libraries:

```python run id=idea
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE emp (name TEXT, dept TEXT, salary INTEGER)")
db.executemany(
    "INSERT INTO emp VALUES (?, ?, ?)",
    [("ana", "eng", 120), ("bo", "eng", 100), ("cy", "ops", 90), ("di", "ops", 95), ("ed", "art", 70)],
)

query = """
SELECT dept, COUNT(*), MAX(salary)
FROM emp
WHERE salary >= 80
GROUP BY dept
ORDER BY dept
"""
for row in db.execute(query):
    print(row)
```

```output for=idea
('eng', 2, 120)
('ops', 2, 95)
```

The query says which rows to keep, how to group them and what to report. It does not say whether to scan the table or use an index, how to group, or in which order to visit the rows. The database can choose a different plan when the data grows or an index is added, and the query stays the same.

## How it works

**Logic and control.** Robert Kowalski divided an algorithm into two parts: "a logic component, which specifies the knowledge to be used in solving problems, and a control component, which determines the problem-solving strategies by means of which that knowledge is used."[^2] He noted that "The logic component determines the meaning of the algorithm whereas the control component only affects its efficiency."[^2] A declarative program is the logic component. The engine supplies the control. Kowalski argued that programs "would be more often correct and more easily improved and modified if their logic and control aspects were identified and separated in the program text."[^2]

**An engine does the work.** Each declarative language has an engine that turns a description into steps. A SQL database has a query planner. The SQLite documentation explains that for one statement "there might be hundreds or thousands or even millions of different algorithms of performing the operation", and the planner "tries to pick the fastest and most efficient algorithm for each SQL statement."[^1] A regular expression engine compiles a pattern into an automaton or a backtracking matcher. A browser turns HTML and CSS into a layout. A build tool turns a graph of dependencies into an order of commands. A logic language such as Prolog searches for values that make the rules true.

**Independence from the representation.** Because the description does not say how to find the data, the representation can change under it. Edgar Codd built the relational model on this point: "Future users of large data banks must be protected from having to know how the data is organized in the machine (the internal representation)."[^3] An index can be added, a table can be split, or a new version of the engine can choose a better plan, and the queries need no change.

**Order does not matter, or matters less.** In an imperative program, the order of the statements is part of the meaning. In a declarative description, it often is not. The Terraform documentation says that its language "is declarative, describing an intended goal rather than the steps to reach that goal. The ordering of blocks and the files they are organized into are generally not significant".[^4] The engine finds the order from the relationships between the parts, so it can also run independent parts in parallel.

**Describe states, not transitions.** User interface code used to change the screen step by step: enable this button, hide that message. React describes the other approach: "Instead of manipulating individual pieces of the UI directly, you describe the different states that your component can be in, and switch between them in response to the user input."[^5] A function maps each state to a description of the screen, and the library computes the changes. The number of states is usually much smaller than the number of transitions between them, so there is less code to get wrong.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `sqlite3` with SQL, `re`, comprehensions, decorators and data classes |
| JavaScript | native | regex literals, array methods, declarative UI libraries |
| TypeScript | native | as JavaScript, plus type declarations |
| Java | native | streams and collectors, `java.util.regex`, annotations |
| Kotlin | idiom | collection functions, regex, type-safe builders |
| C# | native | LINQ query syntax, attributes, `Regex` |
| Go | idiom | `text/template`, `regexp`, struct tags |
| Rust | idiom | `match` patterns, derive attributes; regex in a crate |
| C | idiom | POSIX `<regex.h>`, `printf` format strings |
| C++ | idiom | `<regex>`, range algorithms with projections |
| Bash | idiom | awk patterns and actions, `find`, `make` |
| PowerShell | idiom | `Where-Object`, `Group-Object`, Desired State Configuration |
| SQL | native | queries planned by the engine |
| Regex | native | patterns compiled into matchers |
| HTML | native | document structure; the browser lays it out |
| CSS | native | rules for selectors; the cascade decides |

No general-purpose language in the table is declarative at its base. Python, Java, C#, Go, Rust and C are imperative, and they reach declarative style through embedded languages and libraries. The common pattern is a small language for one domain, inside a string or a builder: SQL in a string passed to a driver, a regular expression, a template, a format string. C# went further and put a query language into the syntax. Microsoft's guide says that with LINQ "you write declarative code that describes what you want to retrieve."[^6] The compiler turns the query into method calls, and a provider can turn the same query into SQL.

SQL, regular expressions, HTML and CSS are declarative by design. They are not general-purpose languages, and their engines are complex: a query planner, a regex compiler, a layout engine. The narrow domain is what lets the engine choose a good strategy.

Annotations and attributes are another declarative form in Java, C#, Python and Rust. A field marked as required, a method marked as a route for a URL, or a type marked to derive `Debug` states a fact, and a framework or the compiler writes the code. See [annotations](/annotations/).

## Variations and related ideas

- **Functional programming.** Pure functions describe values, not steps, so functional programs are often called declarative. See [functional programming](/functional-programming/).
- **Logic programming.** A program is a set of facts and rules, and the engine searches for answers to queries. Prolog and Datalog are the main examples.
- **Constraint programming.** The program states constraints on variables, and a solver finds values that satisfy them. SAT and SMT solvers, and constraint solvers in scheduling, work this way.
- **Configuration as code.** Terraform, Kubernetes manifests and Nix describe the wanted state of a system, and a tool computes the changes to reach it.
- **Build systems.** Make, Bazel and similar tools take a graph of targets and dependencies and decide what to rebuild and in which order.
- **Dataflow and reactive programming.** A spreadsheet is declarative: each cell states how to compute its value, and the engine updates cells when their inputs change.
- **Embedded DSLs.** A library can give a declarative interface inside an imperative language, such as LINQ, query builders and UI builders.

## Pitfalls

- **Performance depends on the engine.** The same query can take a millisecond or an hour, depending on the plan and the indexes. Read the plan, for example with `EXPLAIN`, when a query is slow.
- **The abstraction leaks.** Regular expressions with nested repetition can backtrack for exponential time on some inputs. CSS rules can conflict in ways that need knowledge of the cascade and of specificity.
- **Greedy and default rules.** A description can match more than the author meant, such as a greedy `.*` in a regular expression or an unqualified CSS selector.
- **Injection.** Building SQL, shell commands or HTML from untrusted text by string concatenation lets the text change the description. Use parameters and escaping that the engine provides.
- **Hard to debug.** There are no steps to trace. When the result is wrong, reduce the description or ask the engine to explain its plan.
- **Hidden order.** Some declarative systems still depend on order in places, such as the order of rules in CSS when specificity is equal, or the order of clauses in Prolog for termination.

## Cost

The cost of a declarative program is the cost of the plan that the engine chooses, plus the cost of choosing it. A good engine can beat hand-written code, because it knows the indexes, the sizes of the data and the hardware, and can change its plan when they change. A poor plan can be much slower than a simple loop. Engines also cost start-up time: a query must be parsed and planned, and a regular expression must be compiled, so programs cache prepared statements and compiled patterns. Engines with guarantees make the cost predictable: Go's `regexp` and the RE2 library run in time linear in the input because they do not support backreferences.

## History

Declarative ideas came from logic and from data. In 1970, Edgar Codd proposed the relational model, in which a query is a statement about relations and not a program that walks a file.[^3] SEQUEL, later SQL, came out of IBM's System R project in the 1970s. Alain Colmerauer and Philippe Roussel designed Prolog in 1972, and Kowalski described logic programming and the split of logic and control in 1979.[^2] Ken Thompson built regular expression search into text editors in the 1960s, and Unix tools such as grep, sed, awk and make spread declarative small languages among programmers. HTML in 1991 and CSS in 1996 made declarative description the basis of the web. From the 2010s on, declarative user interfaces such as React and SwiftUI, and declarative infrastructure such as Terraform and Kubernetes, moved the idea into application and operations code.

## References
