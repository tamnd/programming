---
tag: DFH6NR
kind: concept
slug: concatenative-programming
name: Concatenative programming
aliases: [stack-based programming, stack languages, Forth style, postfix programming, point-free stack programming]
area: paradigms/concatenative
level: core
summary: >-
  In concatenative programming, every word is a function from a stack to a stack, so writing two programs one after the other composes them, and no word names its arguments.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1970
  people: [Charles H. Moore, Manfred von Thun]
  note: Moore used Forth in 1970 to control radio telescopes, and von Thun's Joy gave the model its algebraic foundations in the 1990s.
see_also: [functional-programming, higher-order-functions, array-oriented-programming, imperative-programming, declarative-programming, recursion]
same_as:
  wikidata: Q4230857
  wikipedia: Concatenative_programming_language
sources:
- url: https://hypercubed.github.io/joy/html/j00rat.html
  title: Manfred von Thun, Rationale for Joy, a functional language
  locator: Introduction
- url: https://hypercubed.github.io/joy/html/j02maf.html
  title: Manfred von Thun, Mathematical foundations of Joy
  locator: Introduction; program concatenation and function composition
- url: https://factorcode.org/
  title: Factor programming language
  locator: Why Factor
---

## Idea

A concatenative program is a sequence of words, and writing two programs one after the other composes them. Each word is a function from a stack of values to a new stack: `2` pushes the number 2, `+` pops two numbers and pushes their sum, and `dup` copies the top item. The program `2 3 + 4 *` pushes 2 and 3, adds them, pushes 4 and multiplies, and leaves 20. No word names its arguments, because every word takes its inputs from the stack and leaves its results there. Forth, PostScript, Joy, Factor and the bytecode of the JVM and of CPython work in this way.

```python run id=idea
import operator

WORDS = {}  # word name -> function that changes the stack
DEFS = {"square": "dup *", "inc": "1 +"}  # words defined by other words


def binary(f):
    def word(s):
        b, a = s.pop(), s.pop()
        s.append(f(a, b))
    return word


WORDS.update({"+": binary(operator.add), "*": binary(operator.mul),
              "-": binary(operator.sub)})
WORDS["dup"] = lambda s: s.append(s[-1])
WORDS["swap"] = lambda s: s.extend([s.pop(), s.pop()])
WORDS["i"] = lambda s: run(s.pop(), s)  # call the quotation on top


def dip(s):  # run the quotation under the top item
    q, x = s.pop(), s.pop()
    run(q, s)
    s.append(x)


WORDS["dip"] = dip


def parse(tokens):
    out = []
    while tokens:
        t = tokens.pop(0)
        if t == "[":
            out.append(parse(tokens))  # a quotation is a program as data
        elif t == "]":
            return out
        else:
            out.append(int(t) if t.lstrip("-").isdigit() else t)
    return out


def run(prog, stack):
    if isinstance(prog, str):
        prog = parse(prog.split())
    for w in prog:
        if isinstance(w, list) or isinstance(w, int):
            stack.append(w)
        elif w in DEFS:
            run(DEFS[w], stack)
        else:
            WORDS[w](stack)
    return stack


print(run("2 3 + 4 *", []))
print(run("3 square inc", []))
print(run("1 2 [ 10 * ] dip", []))
print(run("5 [ square ] i", []))

p, q = "2 3 +", "4 * inc"
print(run(p + " " + q, []) == run(q, run(p, [])))
```

```output for=idea
[20]
[10]
[10, 2]
[25]
True
```

The interpreter is a loop over the words. A number pushes itself, a defined word such as `square` runs its definition, and a quotation in brackets pushes itself as data until a combinator such as `i` or `dip` runs it. The last line shows the central property: running `p` and then `q` on the result gives the same stack as running the text of `p` joined to the text of `q`.

## How it works

**Concatenation is composition.** In most languages a program applies functions to arguments, as in `f(g(x), y)`. In a concatenative language, every word denotes a function from stacks to stacks, and putting words side by side composes their functions. Manfred von Thun, who designed Joy, wrote: "Whereas all other functional programming languages are based on the application of functions to arguments, Joy is based on the composition of functions. All such functions take a stack as argument and produce a stack as value."[^1] Because composition is associative, a program can be cut at any point between two words, and each part is a valid program. This makes refactoring mechanical: any sequence of words that appears twice can become a new word.

**The stack carries the data.** The stack replaces named variables. A word states its stack effect, the items it takes from the top and the items it leaves, and Forth programmers write it in a comment before each definition, while Factor requires it in the syntax of each definition. Shuffle words such as `dup`, `drop`, `swap`, `over` and `rot` rearrange the top items, so that the next word finds its inputs in the right order. Short definitions keep the shuffling small, and Forth programmers aim for words of one or two lines.

**Quotations and combinators.** A quotation is a program written as data, such as `[ 10 * ]`. It stays on the stack until a combinator runs it. `i` runs the quotation on top, `dip` runs it under the item below, and `map`, `filter` and `ifte` give loops and conditionals. Von Thun summed up the design: "where other functional languages use abstraction and application, Joy uses quotation and combinators".[^1] Quotations play the role of the lambdas of other languages, so higher-order programming needs no variables. See [higher-order functions](/higher-order-functions/).

**A simple semantics.** The meaning of a program follows from the meaning of its words and one rule. The paper on the foundations of Joy states it: "The concatenation of two programs denotes the composition of the functions denoted by the two programs."[^2] In algebraic terms, the meaning of programs is a homomorphism from the monoid of programs under concatenation to the monoid of functions under composition. Equations such as `swap swap = id` and `dup drop = id` hold for every stack, and a programmer or a compiler can rewrite programs with them.

**Stack machines underneath.** Many virtual machines are stack machines: the JVM, the .NET runtime, CPython and WebAssembly run bytecode that pushes and pops values. A compiler for one of them translates the nested expressions of the source into a concatenative program, so `(a + b) * c` becomes `a b + c *`. Concatenative languages expose this model to the programmer, and a simple implementation needs only a loop, a stack and a dictionary of words.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | closures over a list or tuple stack; CPython bytecode is a stack machine |
| JavaScript | absent | arrays of functions; method chains |
| TypeScript | absent | as JavaScript |
| Java | absent | `andThen` on functional interfaces; JVM bytecode |
| Kotlin | absent | function composition; JVM bytecode |
| C# | absent | delegates over `ImmutableStack<T>`; CIL bytecode |
| Go | absent | a map of words and a loop |
| Rust | absent | an enum of operations and `match`; iterator chains |
| C | absent | a global stack and a loop over tokens |
| C++ | absent | lambdas composed with a fold expression |
| Bash | idiom | pipelines; the `dc` calculator |
| PowerShell | idiom | the object pipeline |
| SQL | n/a | no stack and no words |
| Regex | n/a | patterns, not programs of words |
| HTML | n/a | no programs to compose |
| CSS | n/a | no programs to compose |

None of the core languages of this site is concatenative. They all apply functions to named arguments, so the concatenative model appears in them as an interpreter: a stack, a dictionary of words and a loop, as every example file builds. Each such interpreter is short, which shows how little machinery the model needs.

Several common features have a concatenative shape without a stack. Unix pipelines join programs by writing them in sequence, and each program takes the output of the one before it. Method chains such as `items.filter(f).map(g)` in JavaScript, Java streams, LINQ and Rust iterators read left to right, and the pipe operators `|>` of F#, Elixir and OCaml turn nested calls into a sequence. These are compositions of functions of one value, while a concatenative word can take and leave any number of values.

## Variations and related ideas

- **Forth.** A small, fast language for machine control and embedded systems, with words that compile to threaded code. Users extend the compiler itself with new words.
- **PostScript.** A stack language for page description, which printers run to draw text and graphics. PDF grew from its imaging model.
- **Joy.** A purely functional concatenative language by Manfred von Thun, with quotations and combinators in place of variables.
- **Factor.** Its home page calls it "a concatenative, stack-based programming language with high-level features including dynamic types, extensible syntax, macros, and garbage collection".[^3] It has an optimizing compiler and a large library.
- **Typed stack languages.** Cat and Kitten give each word a type that states the stack it takes and the stack it leaves, and infer these types, so that an unbalanced stack is a compile-time error.
- **Array stack languages.** Uiua combines the stack of concatenative languages with the primitives of APL. See [array-oriented programming](/array-oriented-programming/).
- **Point-free style.** Functional programmers write `sum . map square` in Haskell, which composes functions without naming the argument. Concatenative languages make this the only style. See [functional programming](/functional-programming/).
- **Stack machines.** The JVM, CIL, CPython bytecode and WebAssembly are concatenative instruction sets, and RPN calculators from Hewlett-Packard use the same order of operands.

## Pitfalls

- **Operand order.** `2 3 * 10 -` computes 6 minus 10, because `-` takes the item under the top as its left operand. The wrong block in the Python file shows this mistake.
- **Stack effects that do not balance.** A word that leaves one item too many, or takes one too few, breaks every word that runs after it, often far from the cause. State the stack effect of each word and test words alone. Typed stack languages find these errors before the program runs.
- **Stack juggling.** A word that needs four values and a long series of `rot`, `over` and `swap` is hard to read. Split the word into smaller ones, or use the named locals that Forth and Factor give.
- **Hidden arity.** A reader cannot see how many items a word takes without its definition or its stack comment, so a sequence of unfamiliar words is hard to follow.
- **Underflow.** Popping an empty stack is an error at run time in a dynamic implementation and undefined behavior in some small Forth systems. Check the depth at the boundaries of a program.

## Cost

A word costs one dispatch in an interpreter, often a jump through a table of addresses in Forth's threaded code. Pushes and pops are memory accesses near the top of the stack, which stays in the cache. An optimizing compiler, such as Factor's or a JIT for the JVM, keeps the top items of the stack in registers and removes most of the pushes and pops, so the code runs like code from an applicative language. A definition costs one call, and inlining short words removes that. An interpreter for the model fits in a few kilobytes, which is why Forth systems run on microcontrollers.

## History

Charles H. Moore developed Forth in the late 1960s and used it in 1970 to control the telescopes of the National Radio Astronomy Observatory. Forth spread to embedded systems, boot firmware and space hardware, and ANSI standardized it in 1994. Hewlett-Packard calculators used Reverse Polish notation from 1968. John Warnock and Charles Geschke released PostScript at Adobe in 1984, and it made a stack language run in millions of printers.

Manfred von Thun designed Joy in the 1990s and gave the model its algebraic foundations: programs form a monoid under concatenation, and their meaning is a homomorphism to functions under composition.[^2] Slava Pestov started Factor in 2003, and the community around Joy, Factor and Cat adopted the name concatenative programming. Stack bytecode came with Pascal's p-code in the 1970s and the JVM in 1995, and WebAssembly made a stack machine a standard target for the web in 2017.

## References
