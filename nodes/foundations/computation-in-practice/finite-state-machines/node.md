---
tag: ZN6BE8
kind: concept
slug: finite-state-machines
name: Finite-state machines
aliases: [finite-state machine, state machine, finite automaton, DFA, NFA, deterministic finite automaton, transition table, Mealy machine, Moore machine, statechart]
area: foundations/computation-in-practice
level: core
summary: >-
  A finite-state machine is always in one of a fixed set of states and moves between them on each input symbol. Regular expressions, lexers and network protocols are such machines.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [Stephen Kleene, George Mealy, Edward Moore, Michael Rabin, Dana Scott]
  note: Kleene showed in 1956 that finite automata recognize the languages of regular expressions, and Rabin and Scott defined nondeterministic automata in 1959.
see_also: [state-pattern, find-and-replace-with-a-regular-expression, iterators, recursion, imperative-programming, tree-walking-interpreter]
same_as:
  wikidata: Q176452
  wikipedia: Finite-state_machine
sources:
- url: https://swtch.com/~rsc/regexp/regexp1.html
  title: Russ Cox, Regular Expression Matching Can Be Simple And Fast, 2007
  locator: Finite Automata
- url: https://www.rfc-editor.org/rfc/rfc9293.txt
  title: RFC 9293, Transmission Control Protocol (TCP), 2022
  locator: 3.3.2 State Machine Overview
- url: https://doc.rust-lang.org/book/ch18-03-oo-design-patterns.html
  title: Implementing an Object-Oriented Design Pattern, The Rust Programming Language
  locator: Introduction and Encoding States and Behavior as Types
- url: https://pkg.go.dev/regexp
  title: Package regexp, Go standard library
  locator: Overview
---

## Idea

A finite-state machine is a model of computation with a fixed, finite set of states. The machine is always in one of them. It reads its input one symbol at a time, and a transition table gives the next state for each pair of current state and symbol. Some states are final: the machine accepts the input if it stops in one of them. The machine has no other memory, so it can recognize patterns such as numbers, keywords and well-formed dates, but it cannot count without limit or match nested brackets. Regular expressions, lexers, network protocols, user interfaces and game logic are finite-state machines in practice.

```python run id=idea
# a machine that accepts decimal numbers such as 42, -3.5 and 007
TABLE = {
    ("start", "sign"): "sign",
    ("start", "digit"): "int",
    ("sign", "digit"): "int",
    ("int", "digit"): "int",
    ("int", "dot"): "dot",
    ("dot", "digit"): "frac",
    ("frac", "digit"): "frac",
}
ACCEPT = {"int", "frac"}


def kind(ch):
    if ch.isdigit():
        return "digit"
    return {"+": "sign", "-": "sign", ".": "dot"}.get(ch, "other")


def run(text, trace=False):
    state = "start"
    for ch in text:
        state = TABLE.get((state, kind(ch)), "dead")  # no entry: reject
        if trace:
            print(f"  {ch!r} -> {state}")
        if state == "dead":
            return False
    return state in ACCEPT


print("-3.5")
run("-3.5", trace=True)
for text in ["42", "-3.5", "1.", "+", "007", ".5", "4.2.1"]:
    print(f"{text:6} {run(text)}")
```

```output for=idea
-3.5
  '-' -> sign
  '3' -> int
  '.' -> dot
  '5' -> frac
42     True
-3.5   True
1.     False
+      False
007    True
.5     False
4.2.1  False
```

The machine has five states and a dead state. `kind` reduces each character to one of four classes, and `TABLE` gives the next state for each pair of state and class; a pair with no entry goes to the dead state, from which no input escapes. The trace shows the machine reading `-3.5`, and it stops in `frac`, a final state. `1.` stops in `dot` and `+` in `sign`, which are not final, so the machine rejects them, and `.5` dies at once because a number must start with a sign or a digit.

## How it works

**States, input and transitions.** Russ Cox describes the model in an article on regular expressions: "A finite automaton is always in one of its states".[^1] A machine is defined by five things: the set of states, the alphabet of input symbols, the transition function, the start state and the set of final states. The state is the only memory of the machine, so each state must stand for everything that matters about the input read so far. In the example, `int` means "a sign or nothing, then at least one digit, and no dot yet".

**Deterministic and nondeterministic machines.** In a deterministic machine (DFA), "in any state, each possible input letter leads to at most one new state."[^1] A nondeterministic machine (NFA) can have several choices, and it accepts if any sequence of choices ends in a final state. An NFA is often smaller and easier to build from a pattern, and the subset construction turns any NFA into an equal DFA whose states are sets of NFA states, at the risk of many more states. Every regular expression has an equal finite automaton, and every finite automaton has an equal regular expression. The example files check this: each compares its machine with the pattern `[+-]?[0-9]+(\.[0-9]+)?` on every input.

**Two ways to write a machine in code.** A table puts the transitions in data: an array indexed by state and symbol class, as in the idea block and the C++ file. A step is one lookup, and tools can generate, check and draw the table. Code puts the transitions in a `switch` or `match` on the state, as in the other files. This is easier to read and lets each transition run actions, but its structure is less uniform. Languages with enums and exhaustive matching, such as Rust, Java, Kotlin, C# and TypeScript, check that every state has a case, which catches the common bug of a new state that the code forgets.

**Machines that act.** A recognizer only accepts or rejects. Most machines in programs also produce output or run actions on each transition: a lexer emits a token, a protocol sends a message, a user interface shows a different screen. The TCP standard defines a connection this way: "A connection progresses through a series of states during its lifetime."[^2] Its states include LISTEN, SYN-SENT, ESTABLISHED and TIME-WAIT, and each event, a segment that arrives or a call from the user, moves the connection to a new state. Writing such logic as an explicit machine makes illegal transitions visible and keeps the state in one variable instead of spread over many flags.

**States in the type system.** A language with expressive types can make each state a separate type, so that the compiler rejects operations that are illegal in the current state. The Rust book shows this with a blog post that moves from draft to review to published: "we’ll encode the states into different types. Consequently, Rust’s type-checking system will prevent attempts to use draft posts where only published posts are allowed by issuing a compiler error."[^3] The object-oriented state pattern does the opposite: "The states are represented by a set of state objects, and the value’s behavior changes based on its state."[^3] See [the state pattern](/state-pattern/).

**What finite machines cannot do.** A machine with n states cannot tell apart more than n situations. Recognizing balanced brackets of any depth needs a count without limit, so no finite-state machine can do it; a pushdown automaton, which adds a stack, can, and so can a recursive parser. A Turing machine adds a tape without limit and can compute anything that a program can. Real programs live between these models: a machine that has a few counters or a small stack in addition to its state is common, and it is still easy to reason about if the extra memory is small.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `Enum` and `match` with guards; `re` |
| JavaScript | idiom | constants and `switch`; regular expressions |
| TypeScript | idiom | unions of literal types; exhaustive `switch` |
| Java | idiom | `enum` and exhaustive `switch` expressions; `java.util.regex` |
| Kotlin | idiom | `enum class` or `sealed class` and `when` |
| C# | idiom | `enum` and `switch` expressions on tuples; `Regex` |
| Go | idiom | `iota` constants and `switch`; `regexp` |
| Rust | idiom | `enum` and exhaustive `match`; states as types |
| C | idiom | `enum` and `switch`; generated tables from lex or re2c |
| C++ | idiom | `enum class` and `constexpr` tables; `std::regex` |
| Bash | idiom | a state variable and `case` in a loop |
| PowerShell | idiom | `enum` and `switch` |
| SQL | idiom | transition tables; recursive queries; `MATCH_RECOGNIZE` |
| Regex | native | every pattern describes a finite automaton |
| HTML | n/a | no states or transitions |
| CSS | n/a | no machine; `:hover` and `:checked` react to state |

No core language has a construct for state machines, and none needs one: an enumeration for the states, a variable for the current state and a `switch` or a table for the transitions are enough. Every example file builds the same machine for decimal numbers and runs it on the same seven inputs, and the files that print a last line check that the language's regular expression engine agrees. The difference between the languages is how much the compiler checks. Rust, Java and C# reject or warn about a `match` or `switch` that leaves out a state, while JavaScript and Python accept a missing case and send the input to the default.

Regular expressions are the one native case. A pattern is a compact way to write a finite automaton, and engines in the RE2 family, such as Go's `regexp` and Rust's `regex`, compile it to one. Their documentation makes a promise that follows from the model: the Go package "is guaranteed to run in time linear in the size of the input."[^4] Backtracking engines, as in Python, JavaScript, Java and .NET, accept features that are not regular, such as backreferences, and lose that promise.

## Variations and related ideas

- **Mealy and Moore machines.** Machines with output: a Mealy machine produces output on each transition, a Moore machine in each state. Most machines in programs are Mealy machines.
- **Statecharts.** David Harel's statecharts add nested states, parallel regions and history to state machines, and UML state machines and libraries such as XState follow them.
- **Lexers.** A lexer is a finite-state machine that splits text into tokens. Tools such as lex, flex and re2c generate it from regular expressions.
- **Pushdown automata and parsers.** A finite-state machine with a stack recognizes the context-free languages of nested structure, which is the model behind most parsers. See [recursion](/recursion/).
- **Coroutines and async functions.** Compilers turn generators and `async` functions into state machines, with one state for each point where the function can pause. See [iterators](/iterators/).
- **Workflow and protocol machines.** Order processing, document approval and network protocols are long-running machines whose state lives in a database or a connection.
- **Regular expressions.** A notation for the languages that finite automata recognize. See [find and replace with a regular expression](/find-and-replace-with-a-regular-expression/).

## Pitfalls

- **Accepting in the wrong states.** A machine accepts only if it stops in a final state. Checking only that it did not die accepts partial input, as the wrong block of the Python file shows with `1.`, `+` and the empty string.
- **Missing transitions.** A state with no case for some input falls into a default that may be wrong. Use exhaustive matching, or make the dead state explicit, as the fails block of the Rust file shows.
- **State in flags.** Several booleans such as `connected`, `closing` and `error` allow combinations that make no sense. One variable with one value for each real state removes the illegal combinations.
- **Too many states.** Combining independent concerns in one machine multiplies the states. Split the machine into parallel machines, or use the nested states of statecharts.
- **Regular expressions that are not regular.** Backreferences and lookaround in backtracking engines can take exponential time on some inputs, which attackers use for denial of service. Prefer engines that compile to automata for untrusted input.
- **Trying to match nesting.** No finite machine and no true regular expression can check balanced brackets or HTML of any depth. Use a parser.

## Cost

A deterministic machine reads each symbol once and does one lookup or one branch for it, so it runs in time linear in the length of the input and uses constant memory for the state. A transition table costs one entry for each pair of state and symbol class, which is why lexers reduce characters to a few classes first. Turning an NFA into a DFA can, in the worst case, create a number of states exponential in the size of the NFA, so engines such as RE2 build DFA states lazily, only for the inputs they meet, or simulate the NFA directly in time proportional to the product of the input length and the size of the pattern.[^1] Backtracking engines run fast on common patterns but can take exponential time on bad ones.

## History

Warren McCulloch and Walter Pitts modeled neurons as units with finite states in 1943. Stephen Kleene, in a RAND report of 1951 published in 1956, showed that the events such networks can recognize are exactly those described by his regular expressions. George Mealy in 1955 and Edward Moore in 1956 defined the machines with output that carry their names, and in 1959 Michael Rabin and Dana Scott defined nondeterministic finite automata and proved that they recognize the same languages as deterministic ones. Ken Thompson built regular expression search into the QED editor in the 1960s by compiling patterns to machine code that simulated an NFA, and the tools grep, sed, awk and lex brought the method to Unix in the 1970s.[^1]

David Harel introduced statecharts in 1987, and UML adopted them for its state machines in the 1990s. The TCP specification of 1981 described connections as a state machine, and RFC 9293 of 2022 keeps that description.[^2] Russ Cox's articles from 2007 and the RE2 library revived matching by automata, and Go's `regexp` and Rust's `regex` follow that design.[^4]

## References
