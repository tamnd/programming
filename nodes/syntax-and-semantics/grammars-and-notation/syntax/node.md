---
tag: JV3AKW
kind: concept
slug: syntax
name: Syntax
aliases: [syntax, concrete syntax, abstract syntax, abstract syntax tree, AST, parse tree, syntax error, syntactic sugar, desugaring, grammar, surface syntax, automatic semicolon insertion, most vexing parse]
area: syntax-and-semantics/grammars-and-notation
level: core
summary: >-
  Syntax is the set of rules for the form of a program: which characters make tokens and which sequences of tokens make valid programs. What a valid program does is a question of semantics.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [John Backus, Peter Naur]
  note: The ALGOL 60 report described its syntax with metalinguistic formulae, a notation that Backus had proposed in 1959 and that is now called Backus-Naur form.
see_also: [tokens-and-lexing, context-free-grammars, program-semantics, statements-and-expressions, syntactic-macros]
same_as:
  wikidata: Q1152399
  wikipedia: Syntax_(programming_languages)
sources:
- url: https://en.wikipedia.org/wiki/Syntax_(programming_languages)
  title: Syntax (programming languages), Wikipedia
  locator: Introduction; Levels of syntax; Syntax versus semantics
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-2.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 2
  locator: 2.3 The Syntactic Grammar
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Notation; Semicolons
- url: https://docs.python.org/3/reference/introduction.html
  title: Introduction, The Python Language Reference
  locator: 1.2 Notation
- url: https://docs.python.org/3/library/ast.html
  title: ast, Abstract syntax trees, Python documentation
  locator: Introduction
- url: https://pkg.go.dev/go/parser
  title: Package parser, Go standard library
  locator: Overview
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
  title: Lexical grammar, MDN Web Docs
  locator: Automatic semicolon insertion
- url: https://en.cppreference.com/w/cpp/language/direct_initialization.html
  title: Direct-initialization, C++ reference, cppreference.com
  locator: Notes
- url: https://en.wikipedia.org/wiki/Syntactic_sugar
  title: Syntactic sugar, Wikipedia
  locator: Introduction; Origins
- url: https://en.cppreference.com/w/c/language/operator_member_access.html
  title: Member access operators, C reference, cppreference.com
  locator: Subscript
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.14.2 The enhanced for statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/write-linq-queries
  title: Write C# LINQ queries to query data, Microsoft Learn
  locator: Introduction
- url: https://doc.rust-lang.org/reference/macros-by-example.html
  title: Macros By Example, The Rust Reference
  locator: macro.decl.intro
- url: https://www.masswerk.at/algol60/report.htm
  title: Revised Report on the Algorithmic Language Algol 60, J. W. Backus and others, edited by Peter Naur
  locator: 1. Structure of the language; footnote on metalinguistic formulae
---

## Idea

A program is text, and a language must say which texts are programs. Syntax is the set of rules for this. It says how characters make words such as names, numbers and symbols, and how the words make expressions, statements and declarations. A text that breaks these rules has a syntax error, and the compiler or the interpreter rejects it before the program runs. A text that obeys them can still be wrong, but that is a question of meaning, which is semantics.

```python run id=idea
import ast

# Syntax is the form of a program. The parser turns the text into a tree.
tree = ast.parse("total = price * (1 + rate)")
print(ast.dump(tree.body[0].value))

# Two texts with the same tree have the same syntax in the abstract.
a = ast.dump(ast.parse("x=1+2*3"))
b = ast.dump(ast.parse("x = 1 + (2 * 3)"))
print("same tree:", a == b)

# A syntax error is found before the program runs.
try:
    compile("total = price * (1 + rate", "<text>", "exec")
except SyntaxError as e:
    print("SyntaxError:", e.msg, "at line", e.lineno)

# Correct syntax can still fail when it runs: that is a matter of semantics.
code = compile("1 / 0", "<text>", "eval")
try:
    eval(code)
except ZeroDivisionError as e:
    print("ZeroDivisionError:", e)
```

```output for=idea
BinOp(left=Name(id='price', ctx=Load()), op=Mult(), right=BinOp(left=Constant(value=1), op=Add(), right=Name(id='rate', ctx=Load())))
same tree: True
SyntaxError: '(' was never closed at line 1
ZeroDivisionError: division by zero
```

The parser turns `price * (1 + rate)` into a tree: a multiplication whose right operand is an addition. The parentheses are not in the tree, because the shape of the tree already gives the grouping. `x=1+2*3` and `x = 1 + (2 * 3)` give the same tree, so the spaces and the extra parentheses do not change the program. The text with a missing `)` is not a program, and `compile` rejects it before anything runs. `1 / 0` has valid syntax, and it fails only when it runs.

## How it works

**Syntax and semantics.** "The syntax of a language describes the form of a valid program, but does not provide any information about the meaning of the program or the results of executing that program"[^1]. Semantics gives the meaning. "Not all syntactically correct programs are semantically correct"[^1]: a program can have valid syntax and still use a name that does not exist, mix types or divide by zero. See [program semantics](/program-semantics/).

**Three levels.** Computer language syntax "is generally distinguished into three levels"[^1]:

- Words, "the lexical level, determining how characters form tokens". See [tokens and lexing](/tokens-and-lexing/).
- Phrases, "the grammar level, narrowly speaking, determining how tokens form phrases".
- Context, "determining what objects or variables names' refer to, if types are valid, etc."

Java describes the first two levels with two grammars. Its syntactic grammar "has as its terminal symbols the tokens defined by the lexical grammar", and it describes "how sequences of tokens can form syntactically correct programs"[^2]. The third level is often called static semantics, and the compiler checks it after the parse. In the Java example, `int x = 1 }` is a syntax error, and `int x = "one";` is a type error that the compiler finds after a successful parse.

**Grammars and notation.** A language reference gives its syntax as a grammar: a set of rules that each say how a phrase is made from smaller parts. Go writes: "The syntax is specified using a variant of Extended Backus-Naur Form (EBNF)"[^3]. Python uses "a grammar notation that is a mixture of EBNF and PEG"[^4]. See [context-free grammars](/context-free-grammars/).

**Concrete and abstract syntax.** The parser gives "the parse tree, or "concrete syntax tree", which is determined by the grammar, but is generally far too detailed for practical use, and the abstract syntax tree (AST), which simplifies this into a usable form"[^1]. The AST drops the parentheses, the semicolons and the layout, and keeps the structure. Tools read and change programs through the AST. Python's `ast` module "helps Python applications to process trees of the Python abstract syntax grammar"[^5], and Go's `go/parser` "produces an ast.File representing the complete abstract syntax tree of the file"[^6].

**Rules that are not obvious.** Some rules of syntax surprise programmers:

- JavaScript inserts a semicolon at a line break in some places. "return a + b" on two lines "is transformed by ASI into return; a + b;"[^7], so the function returns `undefined`.
- Go also inserts semicolons. A semicolon goes "immediately after a line's final token" if that token is a name, a literal, one of some keywords, or one of `++`, `--`, `)`, `]` and `}`[^3]. So the `{` of a function must be on the same line as the `func`.
- In C++, "In case of ambiguity between a variable declaration using the direct-initialization syntax (1) (with round parentheses) and a function declaration, the compiler always chooses function declaration"[^8]. So `Timer t1();` declares a function. This rule "has been called the most vexing parse"[^8].
- In C, the parse of `T * x;` depends on whether `T` names a type. If it does, the line declares a pointer, and if not, it multiplies two values.

**Syntactic sugar.** "Syntactic sugar is syntax within a programming language that is designed to make things easier to read or to express"[^9]. The compiler translates the sugar into a longer form, and the two forms have the same meaning:

- In C, "The subscript operator E1[E2] is exactly identical to *((E1)+(E2))"[^10], so `1[a]` is valid.
- In Java, "The meaning of the enhanced for statement is given by translation into a basic for statement"[^11] that uses an `Iterator`.
- In C#, "The C# compiler translates query syntax into method calls", and "Query syntax and method syntax are semantically identical"[^12].
- A Python decorator is a short form of a call and an assignment, and `a[i]` calls `__getitem__`.

**Syntax that the program extends.** Some languages let a program add syntax. In Rust, "macro_rules allows users to define syntax extension in a declarative way"[^13]. Lisp macros, Scheme `syntax-rules` and C preprocessor macros do similar things at different levels. See [syntactic macros](/syntactic-macros/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | indentation is syntax; PEG grammar; `ast` module in the standard library |
| JavaScript | native | C-like syntax; automatic semicolon insertion; `new Function` parses at run time |
| TypeScript | native | the syntax of JavaScript with type annotations; the compiler API gives the AST |
| Java | native | lexical and syntactic grammars in the JLS; `javax.tools` and the compiler tree API |
| Kotlin | native | optional semicolons; trailing lambdas; the grammar is in the specification |
| C# | native | C-like syntax; query syntax; the Roslyn syntax API as a library |
| Go | native | EBNF in the spec; semicolons inserted at line ends; `go/parser` and `go/ast` |
| Rust | native | expression-oriented syntax; `macro_rules`; `syn` as a library for parsing |
| C | native | declarator syntax; the parse depends on typedef names; `a[i]` is `*(a + i)` |
| C++ | native | the most vexing parse; templates; `>>` closes two argument lists |
| Bash | native | words, quoting and expansions; `bash -n` checks syntax without a run |
| PowerShell | native | the parser API gives the AST of a script |
| SQL | native | the syntax of the standard and of each database; keywords are not case sensitive |
| Regex | native | a small syntax of its own: atoms, quantifiers, groups and alternation |
| HTML | native | elements, attributes and text; the parser recovers from most errors |
| CSS | native | rules, selectors and declarations; a parser skips a declaration that it cannot read |

Every language has a syntax, so the verdict is native for all 16 languages. They differ in the style of the syntax, in how strict the parser is, and in whether the standard library gives the program access to its own syntax tree.

The example files show these differences. Python prints an AST, prints the standard form of a text with `ast.unparse`, shows a decorator and `a[i]` as sugar, and reports the position of a syntax error. JavaScript shows a syntax error from `new Function`, a `return` that automatic semicolon insertion ends early, a `{` that starts a block, a tagged template and class syntax. Java compiles strings with the compiler API to separate a syntax error from a type error, and shows the enhanced `for` as sugar. C# compares query syntax with method syntax and shows properties, interpolation and patterns. Go parses an expression with `go/parser`, walks the tree and shows the error when a `{` is on its own line. Rust defines a macro, calls a method as a function and writes a `for` loop without the sugar. C shows `1[a]`, the declarator syntax, a typedef that changes a parse and the longest token rule in `i+++j`. C++ shows the most vexing parse, and the compiler warns about it on purpose, and `>>`, a range `for` and a raw string literal.

## Variations and related ideas

- **Free form and layout.** Most languages ignore line breaks and spaces between tokens. Python and Haskell use indentation as a part of the syntax, and Go and JavaScript use line breaks to insert semicolons.
- **Prefix syntax.** Lisp writes every form as a list, such as `(+ 1 2)`. The syntax is small and uniform, so macros can treat code as data.
- **Error recovery.** A compiler can stop at the first syntax error or try to continue and report more errors. An HTML parser recovers from almost every error, because browsers must show pages that have them.
- **Syntax for tools.** Editors highlight syntax, formatters such as `gofmt` and `black` print a standard form from the tree, and linters read the AST.
- **Language workbenches and parser generators.** Tools such as yacc, ANTLR and tree-sitter make a parser from a grammar. GCC, Clang, the Go compiler and the Rust compiler use parsers that are written by hand.

## Pitfalls

- **A line break that ends a statement.** In JavaScript, a `return` followed by a line break returns `undefined`. Put the value on the same line as `return`.
- **The most vexing parse.** `T x();` in C++ declares a function. Use `T x;` or `T x{};`.
- **Sugar that hides a cost.** A short syntax can hide a call, an allocation or a copy: a property in C#, an overloaded operator, or a `for` loop over an iterator.
- **Valid syntax with a different meaning.** `if (x = 0)` in C, `i+++j` and a missing comma between two string literals in Python, which joins them, are all valid syntax that does not do what the programmer wanted.
- **An error far from its cause.** A missing `)` or `}` is often reported at the end of the file or at the next declaration. Read the lines before the reported position.

## Cost

Parsing is fast. A compiler reads source text in time that grows about linearly with its size, and the type checking, optimization and code generation after it take most of the time of a build. An interpreter that reads the source each time it starts pays for the parse at each start, so Python stores compiled bytecode in `__pycache__` and JavaScript engines cache compiled code.

## History

The ALGOL 60 report gave its syntax with "metalinguistic formulae", and it named the source of the notation: "J. W. Backus, The syntax and semantics of the proposed international algebraic language of the Zuerich ACM-GRAMM conference. ICIP Paris, June 1959"[^14]. This notation is now called Backus-Naur form, and most language references still use a form of it[^3][^4]. "The term syntactic sugar was coined by Peter J. Landin in 1964 to describe the surface syntax of a simple ALGOL-like programming language which was defined semantically in terms of the applicative expressions of lambda calculus"[^9].

Later languages added tools to read their own syntax. Python, Go and Java give a parser or a syntax tree in their standard distributions[^5][^6], and Rust macros work on the syntax of the program at compile time[^13].

## References
