---
tag: A1QFCK
kind: concept
slug: logic-programming
name: Logic programming
aliases: [Prolog style, relational programming, Horn clause programming, Datalog, rule-based programming]
area: paradigms/logic
level: core
summary: >-
  Logic programming writes a program as facts and rules in logic, and runs it by asking queries that an engine answers by unification and search, or by deriving facts until nothing changes.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1972
  people: [Alain Colmerauer, Philippe Roussel, Robert Kowalski]
  note: Colmerauer and Roussel built Prolog in Marseille, and Kowalski gave Horn clauses a procedural reading.
see_also: [declarative-programming, functional-programming, pattern-matching, hindley-milner, recursion, laziness]
same_as:
  wikidata: Q275603
  wikipedia: Logic_programming
sources:
- url: https://www.doc.ic.ac.uk/~rak/papers/the%20early%20years.pdf
  title: The Early Years of Logic Programming, Robert Kowalski, Communications of the ACM 31(1), 1988
  locator: Introduction; Marseille
- url: https://www.doc.ic.ac.uk/~rak/papers/algorithm%20=%20logic%20+%20control.pdf
  title: Algorithm = Logic + Control, Robert Kowalski, Communications of the ACM 22(7), 1979
  locator: abstract; top-down and bottom-up interpretations
- url: https://souffle-lang.github.io/simple
  title: A Simple Example, Soufflé documentation
  locator: transitive closure
- url: https://www.sqlite.org/lang_with.html
  title: The WITH Clause, SQLite documentation
  locator: 3. Recursive Common Table Expressions
- url: http://minikanren.org/
  title: miniKanren
  locator: introduction
- url: http://alain.colmerauer.free.fr/alcol/ArchivesPublications/PrologHistory/19november92.pdf
  title: The birth of Prolog, Alain Colmerauer and Philippe Roussel, 1992
  locator: introduction
---

## Idea

Logic programming writes a program as a set of facts and rules in a logical form, and runs it by asking questions. An engine searches for values that make a question true, using the facts and the rules. Robert Kowalski, one of the founders of the field, put the main point like this: "A consequence of using logic to represent knowledge is that such knowledge can be understood declaratively. A consequence of using deduction to derive consequences in a computational manner is that the same knowledge can also be understood procedurally."[^1] The example below is a small engine in Python. It has three facts about parents, two rules that define `ancestor`, and a search that answers questions about them:

```python run id=idea
parent = [("zeus", "ares"), ("ares", "harmonia"), ("harmonia", "semele")]

# Each clause is (head, body). Names that start with a capital letter are variables.
clauses = [(("parent", a, b), []) for a, b in parent] + [
    (("ancestor", "X", "Y"), [("parent", "X", "Y")]),
    (("ancestor", "X", "Y"), [("parent", "X", "Z"), ("ancestor", "Z", "Y")]),
]


def walk(t, s):
    while t[0].isupper() and t in s:
        t = s[t]
    return t


def unify(a, b, s):
    if len(a) != len(b):
        return None
    for x, y in zip(a, b):
        x, y = walk(x, s), walk(y, s)
        if x == y:
            continue
        if x[0].isupper():
            s = {**s, x: y}
        elif y[0].isupper():
            s = {**s, y: x}
        else:
            return None
    return s


def solve(goals, s, depth=0):
    if not goals:
        yield s
        return
    for head, body in clauses:
        rename = lambda term: tuple(f"{t}{depth}" if t[0].isupper() else t for t in term)
        s2 = unify(goals[0], rename(head), s)
        if s2 is not None:
            yield from solve([rename(g) for g in body] + goals[1:], s2, depth + 1)


print([walk("Who", s) for s in solve([("ancestor", "zeus", "Who")], {})])
print([walk("Who", s) for s in solve([("ancestor", "Who", "semele")], {})])
print(any(solve([("ancestor", "semele", "zeus")], {})))
```

```output for=idea
['ares', 'harmonia', 'semele']
['harmonia', 'zeus', 'ares']
False
```

In Prolog, the same program is five lines: `parent(zeus, ares).` and two more facts, then `ancestor(X, Y) :- parent(X, Y).` and `ancestor(X, Y) :- parent(X, Z), ancestor(Z, Y).` The rules do not say which argument is the input. The first question gives the ancestor and asks for the descendants. The second question gives the descendant and asks for the ancestors. The third question gives both and asks for a yes or a no. One definition answers all three.

## How it works

**Facts, rules and queries.** A fact states that a relation holds, such as `parent(zeus, ares)`. A rule states that a relation holds if other relations hold: `ancestor(X, Y) :- parent(X, Z), ancestor(Z, Y)` reads "X is an ancestor of Y if X is a parent of some Z and Z is an ancestor of Y". Facts and rules of this form are Horn clauses: each has at most one conclusion. A query is a list of goals with variables, and an answer is a set of values for the variables that makes all the goals true.

**Unification.** The engine matches a goal against the head of a clause by unification. Unification finds a substitution that makes two terms equal, binding variables on both sides. In the example, `unify` and `walk` do this for flat terms. A full engine also unifies nested terms such as lists and records, which is how Prolog programs build and take apart data.

**Top-down search with backtracking.** Prolog proves a query from the goal down. It takes the first goal, tries the clauses in the order of the program, and for each clause whose head unifies, replaces the goal with the body of the clause. When a goal has no more clauses to try, the engine goes back to the last choice and tries the next clause. This is depth-first search, and in the example the Python generators give the backtracking for free: `yield from` resumes the search for the next answer. Each use of a clause gets fresh variables, which is the job of `rename`.

**Logic plus control.** Kowalski wrote that "an algorithm can be regarded as consisting of a logic component, which specifies the knowledge to be used in solving problems, and a control component, which determines the problem-solving strategies by means of which that knowledge is used."[^2] In Prolog, the logic is the clauses and the control is the fixed search order: clauses from top to bottom, goals from left to right. The same paper shows that one set of Horn clauses can run top-down, from the goal, or bottom-up, from the facts.

**Bottom-up evaluation.** Datalog is a subset of Prolog without nested terms, and its engines usually run bottom-up. The engine applies every rule to the known facts, adds the new facts, and repeats until no rule gives a new fact. The result is the least fixpoint of the rules. The Soufflé documentation shows the standard example, transitive closure over a graph: `path(x, y) :- edge(x, y).` and `path(x, y) :- path(x, z), edge(z, y).`[^3] Bottom-up evaluation always terminates in Datalog, because the set of possible facts is finite, and the order of the clauses does not change the answer. The examples in the other languages on this page use this method.

**Negation as failure.** Horn clauses cannot state that something is false. Prolog adds `\+ Goal`, which succeeds when the search for `Goal` fails. This is negation as failure: the engine assumes that what it cannot prove is false, which is the closed-world assumption of databases. It is correct only when the goal has no unbound variables at the time of the call.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | generators for backtracking; recursive CTEs in `sqlite3` |
| JavaScript | absent | a fixpoint loop over a `Set`; Prolog engines in libraries |
| TypeScript | absent | as JavaScript |
| Java | absent | a fixpoint loop over a `TreeSet`; rule engines in libraries |
| Kotlin | absent | as Java |
| C# | absent | a fixpoint loop with a LINQ join |
| Go | absent | a fixpoint loop over a map used as a set |
| Rust | absent | a fixpoint loop over a `BTreeSet`; Datalog in crates |
| C | absent | a relation as a matrix of Booleans |
| C++ | absent | a fixpoint loop over a `std::set` |
| Bash | absent | no relations or rules |
| PowerShell | absent | no relations or rules |
| SQL | idiom | `WITH RECURSIVE` computes the fixpoint of a rule |
| Regex | n/a | backtracking over a pattern, but no relations |
| HTML | n/a | no rules |
| CSS | n/a | no rules that derive facts |

None of the core languages has a logic engine. Logic programming lives in its own languages: Prolog and its descendants, Datalog engines such as Soufflé, and answer set programming systems. Programs in other languages use it in two ways. They embed an engine, such as a Prolog interpreter or a rule engine in a library, or they write the search or the fixpoint by hand, as the examples on this page do.

SQL is the closest of the core languages. The SQLite documentation says: "Recursive common table expressions provide the ability to do hierarchical or recursive queries of trees and graphs, a capability that is not otherwise available in the SQL language."[^4] A recursive common table expression has a base case and a recursive case, like the two clauses of `ancestor`, and with `UNION` the engine stops when no new row appears. This is bottom-up Datalog evaluation of one rule. The Python file of this page shows it.

The same ideas appear inside tools that programmers use every day. A type checker that infers types solves equations between type terms by unification, as in [Hindley-Milner type inference](/hindley-milner/). The Rust compiler finds trait implementations by a search over impls that works like a logic program. Static analysis tools often express their rules in Datalog, and so do some access control and policy systems.

## Variations and related ideas

- **Prolog.** The first and most used logic programming language, with top-down search, backtracking, nested terms and the cut (`!`), which removes choice points to control the search.
- **Datalog.** A restricted logic language with no nested terms, evaluated bottom-up. It is used for program analysis, queries over graphs and deductive databases.
- **Constraint logic programming.** Variables can range over integers, reals or finite domains, and the engine keeps constraints such as `X + Y #= 10` and solves them. Many Prolog systems include it.
- **Answer set programming.** A program defines stable models, and a solver finds them. It is used for planning, scheduling and configuration problems.
- **miniKanren.** The miniKanren site describes it as "a family of Domain Specific Languages for logic programming",[^5] small enough to embed in a host language as a library.
- **Functional logic languages.** Curry and Mercury join functions and relations with types and modes, which say which arguments are inputs.
- **Pattern matching.** Matching is one-way unification: the pattern has variables, but the value does not. See [pattern matching](/pattern-matching/).
- **Declarative programming.** Logic programming is one branch of [declarative programming](/declarative-programming/), next to [functional programming](/functional-programming/) and query languages.

## Pitfalls

- **Left recursion.** A rule that calls itself as its first goal, such as `ancestor(X, Y) :- ancestor(X, Z), parent(Z, Y)`, makes a top-down engine loop forever. The rule is logically correct, but the control is wrong. The wrong block in the Python file shows the result. Datalog engines and tabling in some Prolog systems do not have this problem.
- **Order matters in Prolog.** The logic does not depend on the order of clauses and goals, but termination and speed do. A goal that binds a variable must come before a goal that tests it.
- **Negation as failure with unbound variables.** `\+ member(X, [1, 2])` fails when `X` is unbound, because some `X` is a member. Call negation only after the goal is ground.
- **The cut.** `!` makes programs faster but removes answers. A cut in the wrong place makes a relation work in one direction only, or makes it give wrong answers.
- **Exponential search.** A depth-first search can try the same subgoal many times. Memoization (tabling) or a bottom-up engine prevents the repeated work.
- **Missing occurs check.** For speed, most Prolog systems unify `X` with `f(X)` and make a cyclic term. The result is not a logical answer. Use `unify_with_occurs_check` where this matters.
- **Arithmetic is not relational.** `X is Y + 1` in Prolog needs `Y` to be bound. Constraint libraries give relations over numbers that run in all directions.

## Cost

A top-down search costs time in proportion to the number of nodes in its search tree, which can be exponential in the size of the problem. Each step unifies terms and records bindings so that it can undo them on backtracking. Well-built Prolog systems compile clauses to instructions for an abstract machine, the Warren Abstract Machine, and index clauses on their first argument, so a simple deterministic predicate runs almost as fast as a function in a compiled language. Naive bottom-up evaluation, as in the examples on this page, repeats old joins in each round. Semi-naive evaluation joins only the new facts of the last round, and engines such as Soufflé compile the rules to parallel C++ with indexes. A Datalog program without negation and nested terms always terminates, and its cost is polynomial in the number of facts.

## History

Logic programming grew from work on automatic theorem proving. In 1965, J. Alan Robinson published the resolution principle, with unification as its central step. In 1971 and 1972, Alain Colmerauer and his group in Marseille built a language for processing natural language, and Kowalski in Edinburgh gave it a procedural reading of Horn clauses. Colmerauer and Philippe Roussel wrote that they had "a preliminary version of Prolog at the end of 1971 and a more definitive version at the end of 1972".[^6] Kowalski recalls that the name "was suggested by Roussel's wife, Jacqueline, as an abbreviation for programmation en logique."[^1] David H. D. Warren's compiler for DEC-10 Prolog in 1977 made Prolog fast enough for real programs, and his abstract machine of 1983 became the base of most later systems. Kowalski's paper "Algorithm = Logic + Control" appeared in 1979.[^2] Japan's Fifth Generation Computer Systems project used logic programming as its base from 1982 to 1992. Prolog became an ISO standard in 1995. Datalog came from deductive database research in the 1980s and returned in the 2010s as a language for program analysis and for queries over graphs.

## References
