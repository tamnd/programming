---
tag: NEW-program-semantics
kind: concept
slug: program-semantics
name: Formal semantics
aliases: [formal semantics, semantics of programming languages, operational semantics, small-step semantics, big-step semantics, structural operational semantics, denotational semantics, axiomatic semantics, Hoare logic, weakest precondition, abstract machine]
area: syntax-and-semantics/formal-semantics
level: core
summary: >-
  A formal semantics gives programs an exact meaning in mathematics: as steps of a machine, as functions, or as rules for proofs. Tools use it to prove facts about programs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1969
  people: [Robert W. Floyd, Tony Hoare, Dana Scott, Christopher Strachey, Gordon Plotkin]
  note: Floyd and Hoare gave the axiomatic method in 1967 and 1969, Scott and Strachey the denotational method in 1971, and Plotkin structural operational semantics in 1981.
see_also: [undefined-behavior, order-of-evaluation, context-free-grammars, tree-walking-interpreter, abstract-syntax-tree, assertions, statements-and-expressions]
same_as:
  wikidata: Q1437428
  wikipedia: Semantics_(computer_science)
sources:
- url: https://webassembly.github.io/spec/core/exec/conventions.html
  title: Conventions, Execution, WebAssembly Specification
  locator: Formal notation
- url: https://homepages.inf.ed.ac.uk/gdp/publications/sos_jlap.pdf
  title: Gordon D. Plotkin, A Structural Approach to Operational Semantics, reprinted in the Journal of Logic and Algebraic Programming, 2004
  locator: 1.1 Introduction
- url: https://docs.python.org/3/reference/introduction.html
  title: Introduction, The Python Language Reference
  locator: 1. Introduction
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.7
  title: Chapter 15, Expressions, The Java Language Specification, Java SE 25 Edition
  locator: 15.7 Evaluation Order
- url: https://en.cppreference.com/w/cpp/language/constant_expression.html
  title: Constant expressions, cppreference.com
  locator: Core constant expressions
- url: https://webassembly.github.io/spec/core/intro/introduction.html
  title: Introduction, WebAssembly Specification
  locator: Design goals
- url: https://compcert.org/
  title: The CompCert project
  locator: Home page
---

## Idea

A formal semantics states the meaning of every program of a language in mathematics, so that the meaning does not depend on a reader's guess or on one compiler. A grammar says which programs are well formed; the semantics says what each one does. There are three main styles. Operational semantics describes the steps of an abstract machine that runs the program. Denotational semantics maps each program to a mathematical object, such as a function from states to states. Axiomatic semantics gives rules for proving facts about a program, such as Hoare triples. Each style suits a different use, and for a well-defined language all three agree.

```python run id=idea
# A tiny language:  x := e | s ; s | while e do s,  with e = n | x | e op e
PROG = ("seq", ("set", "y", 1),
        ("while", (">", "x", 1),
         ("seq", ("set", "y", ("*", "y", "x")), ("set", "x", ("-", "x", 1)))))
OPS = {"+": lambda a, b: a + b, "-": lambda a, b: a - b,
       "*": lambda a, b: a * b, ">": lambda a, b: int(a > b)}


def ev(e, env):  # the value of an expression in an environment
    if isinstance(e, int):
        return e
    if isinstance(e, str):
        return env[e]
    return OPS[e[0]](ev(e[1], env), ev(e[2], env))


def big(s, env):  # big-step: a statement maps a state to its final state
    if s[0] == "set":
        return {**env, s[1]: ev(s[2], env)}
    if s[0] == "seq":
        return big(s[2], big(s[1], env))
    while ev(s[1], env):
        env = big(s[2], env)
    return env


def step(s, env):  # small-step: one rule rewrites a configuration once
    if s[0] == "set":
        return None, {**env, s[1]: ev(s[2], env)}
    if s[0] == "seq":
        rest, env = step(s[1], env)
        return (s[2] if rest is None else ("seq", rest, s[2])), env
    if ev(s[1], env):  # while e do b  ->  b ; while e do b
        return ("seq", s[2], s), env
    return None, env


def den(s):  # denotational: the meaning of a statement is a function
    if s[0] == "set":
        return lambda env: {**env, s[1]: ev(s[2], env)}
    if s[0] == "seq":
        f, g = den(s[1]), den(s[2])
        return lambda env: g(f(env))
    body = den(s[2])

    def loop(env):  # the least fixed point, unrolled on demand
        return loop(body(env)) if ev(s[1], env) else env
    return loop


start = {"x": 5}
print("big-step:     ", big(PROG, start))
config, steps = (PROG, start), 0
while config[0] is not None:
    config, steps = step(*config), steps + 1
print("small-step:   ", config[1], f"after {steps} steps")
print("denotational: ", den(PROG)(start))
```

```output for=idea
big-step:      {'x': 1, 'y': 120}
small-step:    {'x': 1, 'y': 120} after 14 steps
denotational:  {'x': 1, 'y': 120}
```

The program defines a small language of assignments, sequences and `while` loops, and gives it three semantics. `big` relates a statement and a state directly to the final state. `step` rewrites a configuration, a statement and a state, one rule at a time, and needs 14 steps for the factorial of 5. `den` turns each statement into a Python function, built only from the functions of its parts. All three give `y = 120`, which is what agreement between semantics means.

## How it works

**Operational semantics.** An operational semantics gives rules that say how a program runs. Small-step rules rewrite a configuration, a program and its state, into the next configuration, as `step` does. The WebAssembly specification works this way: "A configuration is a syntactic description of a program state. Each rule specifies one step of execution."[^1] Big-step rules, also called natural semantics, relate a program and a start state directly to a final value or state, as `big` does. Gordon Plotkin set out the structural style, with one rule for each form of syntax, and its aim: "It is the purpose of these notes to develop a simple and direct method for specifying the semantics of programming languages."[^2] A tree-walking [interpreter](/tree-walking-interpreter/) is close to a big-step semantics written as code.

**Denotational semantics.** A denotational semantics maps each phrase of a program to a mathematical object, its denotation, and the denotation of a compound phrase comes only from the denotations of its parts. This property, compositionality, lets a reader reason about one part at a time. Statements denote functions from states to states, and a sequence denotes the composition of two functions. Loops and recursion need a fixed point: the meaning of `while` is the least function that satisfies its unrolling equation, and domain theory, from Dana Scott, shows that such a function exists.

**Axiomatic semantics.** An axiomatic semantics defines a language by the facts that can be proved about its programs. A Hoare triple `{P} S {Q}` states that if `P` is true before `S` runs and `S` ends, then `Q` is true after it. Each construct has a rule. The assignment rule says that `{Q[x := e]} x = e {Q}`, where `Q[x := e]` is `Q` with `e` in place of `x`. The loop rule uses an invariant, a condition that each pass of the body keeps true. Edsger Dijkstra's weakest precondition computes the most general `P` for a given `S` and `Q`, as the Python example does. These rules are the basis of [assertions](/assertions/), contracts and program verifiers.

**English specifications.** Most language standards are written in English, not mathematics, and they leave gaps. The Python reference says so: "While I am trying to be as precise as possible, I chose to use English rather than formal specifications for everything except syntax and lexical analysis. This should make the document more understandable to the average reader, but will leave room for ambiguities."[^3] Other standards use precise English to close the gaps. The Java specification states that "the operands of operators appear to be evaluated in a specific evaluation order, namely, from left to right",[^4] where C leaves the order unspecified. See [order of evaluation](/order-of-evaluation/).

**Undefined behavior and the abstract machine.** The C and C++ standards define a program by its effect on an abstract machine, and they mark some operations, such as signed overflow, as [undefined behavior](/undefined-behavior/), for which the standard sets no rules. A compiler may assume that undefined behavior never happens. During constant evaluation the compiler must detect it instead, and cppreference lists `std::numeric_limits<int>::max() + 1` as an error for this reason.[^5]

**What a formal semantics gives.** A formal definition makes a standard exact, so that independent implementations agree. WebAssembly was designed to be "Well-defined: fully and precisely defines valid programs and their behavior in a way that is easy to reason about informally and formally."[^6] A semantics also states what a correct compiler must preserve. CompCert, a C compiler, comes with "a mathematical, machine-checked proof that the generated executable code behaves exactly as prescribed by the semantics of the source program".[^7] Type systems use the semantics to state soundness: a well-typed program does not get stuck in the operational rules.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | the reference in English; CPython as the reference implementation |
| JavaScript | absent | step-by-step algorithms in English; research semantics |
| TypeScript | absent | defined by the compiler; JavaScript at run time |
| Java | absent | the JLS in English; a formal memory model |
| Kotlin | absent | the Kotlin specification in English |
| C# | absent | ECMA-334 in English |
| Go | absent | a short specification in English; a memory model |
| Rust | absent | the Rust Reference in English; MiniRust and Miri |
| C | absent | an abstract machine in English; CompCert and K semantics |
| C++ | absent | an abstract machine in English; constant evaluation |
| Bash | absent | POSIX in English; the Smoosh research semantics |
| PowerShell | absent | a specification in English |
| SQL | absent | the standard in English; research semantics |
| Regex | absent | exact in theory; engines specified in English |
| HTML | n/a | parsing algorithms, but no execution |
| CSS | n/a | the cascade and layout, specified in English |

No core language of this site has an official, complete formal semantics, so every general-purpose language is absent. The languages that do have one are mostly outside the core set: Standard ML was defined formally in 1990, and WebAssembly has a formal specification that the official reference interpreter follows. For the core languages, formal semantics come from research, such as KJS for JavaScript, K-Java, the semantics of C in CompCert and the K framework, and RustBelt for parts of Rust, and they often find bugs in the English standards.

The example files show the three styles and the gaps they close. The Rust file is a big-step interpreter, the C# file builds the denotation of an expression as a function, and the Python and Go files apply Hoare logic. The JavaScript file follows an algorithm of the ECMAScript specification step by step. The Java and C files contrast a language that defines the order of evaluation and overflow with one that leaves them open, and the C++ fails block shows the compiler rejecting undefined behavior during constant evaluation.

## Variations and related ideas

- **Reduction semantics.** A small-step style that splits each configuration into an evaluation context and a redex, the part that the next rule rewrites. PLT Redex runs such definitions.
- **Abstract machines.** The SECD and CEK machines define the steps of functional programs with explicit stacks and environments, close to a real implementation.
- **Executable semantics.** The K framework and PLT Redex turn a formal definition into an interpreter, so the semantics can run test suites.
- **Separation logic.** An extension of Hoare logic for programs with pointers, in which a condition owns a part of the heap. Verifiers such as VeriFast, Viper and Iris use it.
- **Game and trace semantics.** Denotational models that describe a program by its interactions with its context, used for concurrency and higher-order code.
- **Memory models.** Formal rules for what values a read can see in a concurrent program, as in the memory models of Java, C, C++ and Go.
- **Type soundness.** The theorem that well-typed programs do not get stuck, proved by progress and preservation over an operational semantics.

## Pitfalls

- **Taking one compiler as the definition.** A program that works on one compiler can depend on behavior that the standard leaves open. Read the standard, and test with more than one implementation.
- **Undefined behavior that appears to work.** In C and C++ a program with undefined behavior can give the expected output today and a different output with the next compiler or a higher optimization level. Use sanitizers, and Miri for unsafe Rust.
- **Confusing unspecified with implementation-defined.** An implementation-defined result is documented and stable; an unspecified one, such as the order of evaluation of arguments in C, can change from call to call.
- **Partial correctness.** A Hoare triple says nothing about programs that do not end, so a proof of `{P} S {Q}` does not prove that `S` stops. Prove termination with a measure that decreases on each pass of each loop.
- **Semantics of the model, not the machine.** A formal model often leaves out overflow, memory limits or concurrency. A proof holds only for what the model covers.

## Cost

A formal semantics costs nothing at run time; its cost is the effort to write it and to keep it true to the language. Small languages, such as the cores used in research, have short definitions, but the full semantics of a language such as C or JavaScript runs to thousands of rules. Executable semantics run programs much more slowly than a real implementation, so they serve for tests and for deciding disputes, not for production. Proofs about real programs with Hoare logic or separation logic take much more effort than writing the program, and verified projects such as CompCert and seL4 took years of work.

## History

Robert W. Floyd gave the first method for proving programs with assertions on flowcharts in 1967, and Tony Hoare turned it into a logic of triples for programs in 1969. Christopher Strachey and Dana Scott developed denotational semantics from 1969, and Scott's domain theory gave meaning to recursion and loops. Peter Landin described the SECD machine for functional languages in 1964, and the Vienna Definition Language at IBM gave PL/I an operational semantics in the late 1960s. Edsger Dijkstra introduced weakest preconditions in 1975.

Gordon Plotkin wrote his notes on structural operational semantics in 1981,[^2] and Gilles Kahn proposed natural semantics in 1987. Robin Milner, Mads Tofte and Robert Harper published the formal definition of Standard ML in 1990. Xavier Leroy's CompCert, from 2005, proved a C compiler correct against a formal semantics.[^7] WebAssembly, from 2017, was the first widely used language whose standard includes a formal semantics.[^6]

## References
