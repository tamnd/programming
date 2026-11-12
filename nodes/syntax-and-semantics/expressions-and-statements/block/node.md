---
tag: 7RQNNB
kind: concept
slug: block
name: Block
aliases: [block, code block, compound statement, block statement, suite, block scope, block structure, block expression, labeled block, begin end, curly braces, off-side rule]
area: syntax-and-semantics/expressions-and-statements
level: core
summary: >-
  A block is a group of statements that the language treats as one statement. In most languages a block is also a scope, so the names declared in it are local to it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [Peter Naur, John Backus, John McCarthy, Alan J. Perlis, Heinz Rutishauser, Adriaan van Wijngaarden]
  note: The ALGOL 60 report defined a block as declarations followed by statements between begin and end, and made each declaration valid only in its block. ALGOL 58 had compound statements, which grouped statements but did not have their own declarations.
see_also: [lexical-scope, statements-and-expressions, declaration, structured-programming, resource-acquisition-is-initialization]
same_as:
  wikidata: Q1327579
  wikipedia: Block_(programming)
sources:
- url: https://en.wikipedia.org/wiki/Block_(programming)
  title: Block (programming), Wikipedia
  locator: Introduction; History; Syntax
- url: https://en.cppreference.com/w/c/language/statements.html
  title: Statements, C reference, cppreference.com
  locator: Compound statements
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.2 Blocks
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8. Compound statements
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-6.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 6
  locator: 6.3 Scope of a Declaration; 6.4 Shadowing and Obscuring
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Blocks
- url: https://docs.python.org/3/reference/executionmodel.html
  title: Execution model, The Python Language Reference
  locator: 4.1 Structure of a program
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/block
  title: Block statement, MDN Web Docs
  locator: Description
- url: https://doc.rust-lang.org/reference/expressions/block-expr.html
  title: Block expressions, The Rust Reference
  locator: expr.block.intro; expr.block.value-trailing-expr
- url: https://en.cppreference.com/w/cpp/language/raii.html
  title: RAII, C++ reference, cppreference.com
  locator: Introduction
- url: https://doc.rust-lang.org/reference/destructors.html
  title: Destructors, The Rust Reference
  locator: destructors.scope.intro
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/using
  title: using statement, C# language reference
  locator: using declaration
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60, J. W. Backus and others, edited by Peter Naur
  locator: Description of the reference language; 4.1.3 Semantics
---

## Idea

An `if` or a loop controls one statement, but the work often needs many statements. A block groups many statements into one, so a control structure can control all of them. Most languages mark a block with braces, some with `begin` and `end`, and Python with indentation. In most languages a block is also a scope: a name declared in the block is not visible outside it, and in some languages an object made in the block is destroyed at its end.

```python run id=idea
# Python groups statements into a block by indentation.
def classify(n):
    if n < 0:
        kind = "negative"   # these two lines are one block
        n = -n
    else:
        kind = "not negative"
    return kind, n


print(classify(-4), classify(3))

# A block in Python is not a scope: a name made in the if block is visible after it.
if True:
    made_inside = 42
print("made_inside =", made_inside)

# The loop variable also stays after the loop.
for i in range(3):
    pass
print("i after the loop =", i)

# A function body is a scope, so a name made there is not visible outside.
def f():
    local = 1
    return local

f()
print("local" in globals())
```

```output for=idea
('negative', 4) ('not negative', 3)
made_inside = 42
i after the loop = 2
False
```

The two indented lines after `if n < 0:` are one block, so both run when the condition is true. Python is different from most languages in one way: an `if` block or a loop body is not a scope. `made_inside` and the loop variable `i` stay visible after their blocks. Only a module, a function body and a class body make a new scope, so `local` is not visible outside `f`.

## How it works

**A group of statements.** "A block of source code is a grouping of statements that execute in sequential order, top to bottom", and "Syntactically, a block acts much like a single statement"[^1]. The grammar of most languages says that an `if`, a `while` or a `for` controls one statement, and a block is one statement. C calls it a compound statement: "A compound statement, or block, is a brace-enclosed sequence of statements and declarations"[^2].

**How the block is marked.**

- Braces: C, C++, Java, C#, JavaScript, Go and Rust. The Java grammar is `Block: { [ BlockStatements ] }`[^3].
- Keywords: `begin` and `end` in ALGOL and Pascal, `do` and `end` in Ruby and Lua.
- Indentation: "Python and Haskell use indentation to demarcate a block instead of delimiters as most languages do"[^1]. Python calls the block a suite: "A suite is a group of statements controlled by a clause", and it can be on the same line as the header or on indented lines after it[^4].

**A block is a scope.** In most languages a name that a block declares is local to the block. In Java, the scope of a local variable "is the rest of the block, starting with the declaration's own initializer"[^5]. Go has explicit blocks and implicit ones: "Each "if", "for", and "switch" statement is considered to be in its own implicit block"[^6], so a variable declared in the header of an `if` is local to the `if`. See [lexical scope](/lexical-scope/).

**Where blocks are not scopes.** In Python, "A block is a piece of Python program text that is executed as a unit. The following are blocks: a module, a function body, and a class definition"[^7]. An indented suite is not in this list, so it is not a scope. In JavaScript, `let`, `const` and `class` are scoped to the block, but `var` and function declarations in non-strict mode "do not have block scope"[^8].

**Hiding a name.** An inner block can usually declare a name that the outer block has, and the inner name hides the outer one. C, C++, Go and Rust allow this. Java and C# do not allow it for local variables: "It is a compile-time error if the name of a local variable v is used to declare a new variable within the scope of v"[^5].

**A block as an expression.** In Rust, "A block expression, or block, is a control flow expression and anonymous namespace scope for items and variable declarations", and "When a block contains a final operand, the block has the type and value of that final operand"[^9]. So `let area = { let w = 3; let h = 4; w * h };` gives `12`. A semicolon after the last expression makes the value `()`. Java has a similar form: an arm of a `switch` expression can be a block that ends with `yield`.

**The end of a block.** In C++, C# and Rust, the end of a block is also the end of the life of its local objects. C++ uses this for RAII, which "binds the life cycle of a resource that must be acquired before use ... to the lifetime of an object"[^10]. In Rust, "When control flow leaves a drop scope all variables associated to that scope are dropped in reverse order of declaration"[^11]. In C#, with a `using` declaration, "a local variable is disposed at the end of the scope in which it's declared"[^12].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | indented suites; a suite is not a scope |
| JavaScript | native | `{ }`; `let` and `const` are block scoped, `var` is not; labeled blocks |
| TypeScript | native | as JavaScript |
| Java | native | `{ }`; labeled blocks; initializer blocks; a local cannot hide a local |
| Kotlin | native | `{ }`; a lambda block gives the value of its last expression |
| C# | native | `{ }`; `using` declarations; `checked` and `unchecked` blocks |
| Go | native | `{ }`; implicit blocks for `if`, `for`, `switch` and each case |
| Rust | native | block expressions with a value; labeled blocks; drop at the end of a block |
| C | native | compound statements; block scope; automatic storage |
| C++ | native | compound statements; destructors at the end of a block (RAII) |
| Bash | native | `{ ...; }` groups commands; `( ... )` runs them in a subshell |
| PowerShell | native | script blocks `{ }`, which are also values |
| SQL | native | `BEGIN ... END` in procedural SQL |
| Regex | n/a | groups group parts of a pattern, but a pattern has no statements |
| HTML | n/a | no statements; a block element is a layout term |
| CSS | idiom | a rule has a declaration block in `{ }` |

Every general-purpose language has blocks, so the verdict is native. The languages differ in how they mark a block, in whether a block is a scope, in whether a block has a value, and in what happens to the local objects at the end of the block.

The example files show these differences. Python shows that the names from a loop body stay after the loop, that a comprehension has its own scope, and that `with` runs exit code at the end of its block. JavaScript shows `let` and `var` in a block, a labeled block, a `let` binding per loop iteration, and a `{` after `=>` that starts a block and not an object. Java shows a static initializer block, a labeled block, `try` with resources and a block in a `switch` arm. C# shows a `using` declaration and `checked` and `unchecked` blocks. Go shows a name in an inner block, the implicit block of an `if` and `defer`, which runs at the end of the function and not at the end of the block. Rust shows a block with a value, a drop at the end of a block and a labeled block with `break`. C shows a name that hides an outer name, an `if` without braces and a `static` local. C++ shows destructors at the end of a block, an `if` with an init-statement and a lock that the block holds.

## Variations and related ideas

- **Block structure.** A language where blocks nest and each block can declare names is block-structured. ALGOL 60 started this[^13]. See [structured programming](/structured-programming/).
- **Implicit blocks.** The header of an `if`, a `for` or a `switch`, and each case, can be a block of its own. Go defines this in its spec[^6], and C++ allows an init-statement in `if` and `switch`.
- **Labeled blocks.** Java, JavaScript and Rust let a program leave a labeled block early with `break`. In Rust the `break` can carry the value of the block.
- **Blocks as values.** A lambda or a closure is a block that the program can store and call later. Smalltalk and Ruby call them blocks, PowerShell has script blocks, and Kotlin passes a lambda block as the last argument.
- **Statement expressions.** GNU C has a non-standard `({ ... })` form that makes a block into an expression. C++ uses a lambda that is called at once to get the same effect.

## Pitfalls

- **A missing pair of braces.** Without braces, an `if` controls only the next statement. A second line with the same indent looks like a part of the `if`, but it runs each time. Many style guides require braces for each `if` and loop.
- **A name that leaks from a block.** In Python, and with `var` in JavaScript, a name made in a block is still visible after it, and code that reads it later can get a stale value.
- **A name that hides another.** In C, C++ and Go, an inner declaration with the same name hides the outer one, and an assignment changes the inner name only. In Go, `x, err := f()` in an inner block is a common case.
- **A pointer to a dead object.** In C and C++, a pointer to a local object of a block is not valid after the block ends. Use a `static` object, an object on the heap or a value that is copied out.
- **`defer` is not a block exit.** In Go, `defer` in a loop runs when the function returns, not at the end of each iteration, so a loop that opens files can keep them all open.
- **An object literal that is a block.** In JavaScript, `() => { value: 1 }` is a function with a block and a label, and it returns `undefined`. Write `() => ({ value: 1 })`.

## Cost

A block itself costs nothing at run time. The compiler uses it to find the scope of each name and to reuse stack slots for names whose blocks do not overlap. The code at the end of a block can cost time: a destructor in C++, a drop in Rust, a `Dispose` call in C# or the exit method of a `with` in Python. This cost is usually the release of a resource that the program must release anyway.

## History

"The block concept was developed in the 1950s during the development of the first autocodes", and "Algol 58 introduced the notion of the "compound statement", which was related solely to control flow"[^1]. The ALGOL 60 report added the block: "A sequence of declarations followed by a sequence of statements and enclosed between begin and end constitutes a block. Every declaration appears in a block in this way and is valid only for that block"[^13]. It also said that "Every block automatically introduces a new level of nomenclature"[^13].

Later languages kept the block and changed its marks. C used braces[^2]. Python used indentation[^4]. C++ joined the end of a block to the release of resources[^10], and Rust made the block an expression with a value[^9].

## References
