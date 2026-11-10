---
tag: NEW-syntactic-macros
kind: concept
slug: syntactic-macros
name: Syntactic macros
aliases: [macro, macros, preprocessor macro, hygienic macro]
area: metaprogramming/macros
level: core
summary: >-
  A rule that the compiler applies to program text before it gives the text a meaning. The C preprocessor works on tokens; Lisp, Scheme and Rust macros work on syntax.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1963
  people: [Timothy P. Hart]
  note: An MIT AI memo proposed a MACRO instruction expander in the define of LISP, to replace special forms that take unevaluated or indefinite arguments.
see_also: [parametric-polymorphism, higher-order-functions, closures, constant-evaluation, annotations, c, cpp, rust]
same_as:
  wikidata: Q629579
  wikipedia: Macro_(computer_science)
sources:
  - url: http://hdl.handle.net/1721.1/6111
    title: Timothy P. Hart, MACRO Definitions for LISP, MIT AI Memo AIM-057, October 1963
    locator: abstract
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: N3220, working draft of ISO/IEC 9899:2024 (C23)
    locator: 5.1.1.2 Translation phases, phase 4; 6.10.5 Macro replacement; 6.10.5.4 Rescanning and further replacement, paragraphs 1 and 2; 6.10.5.5 Scope of macro definitions, paragraph 1
  - url: https://gcc.gnu.org/onlinedocs/cpp/Operator-Precedence-Problems.html
    title: The C Preprocessor (GCC), 3.10.2 Operator Precedence Problems
    locator: the ceil_div example and the parenthesized definition
  - url: https://gcc.gnu.org/onlinedocs/cpp/Swallowing-the-Semicolon.html
    title: The C Preprocessor (GCC), 3.10.3 Swallowing the Semicolon
    locator: the SKIP_SPACES example and the do ... while (0) definition
  - url: https://gcc.gnu.org/onlinedocs/cpp/Duplication-of-Side-Effects.html
    title: The C Preprocessor (GCC), 3.10.4 Duplication of Side Effects
    locator: the min example, "an unsafe macro", and the GNU statement expression
  - url: https://gcc.gnu.org/onlinedocs/cpp/Self-Referential-Macros.html
    title: The C Preprocessor (GCC), 3.10.5 Self-Referential Macros
    locator: the foo example and the EPERM example
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf
    title: Rationale for International Standard, Programming Languages, C, Revision 5.10, April 2003
    locator: 6.10.3 Macro replacement, pages 99 and 101; 6.10.3.2 The # operator and 6.10.3.3 The ## operator, pages 102 and 103
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/m_defmac.htm
    title: Common Lisp HyperSpec, Macro DEFMACRO
    locator: Description
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/02_df.htm
    title: Common Lisp HyperSpec, 2.4.6 Backquote
    locator: the first two paragraphs and the comma at-sign paragraph
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/f_gensym.htm
    title: Common Lisp HyperSpec, Function GENSYM
    locator: Description
  - paper: Eugene Kohlbecker, Daniel P. Friedman, Matthias Felleisen and Bruce Duba, Hygienic macro expansion, Proceedings of the 1986 ACM Conference on LISP and Functional Programming, pages 151 to 161
    url: https://doi.org/10.1145/319838.319859
    title: Hygienic macro expansion
    locator: bibliographic record (title, venue, year, pages)
  - url: https://standards.scheme.org/official/r7rs.pdf
    title: Revised7 Report on the Algorithmic Language Scheme
    locator: 4.3 Macros, page 22; Introduction (the paragraph on the history of Scheme); References [9], [21] and [22]
  - url: https://doc.rust-lang.org/reference/macros-by-example.html
    title: The Rust Reference, Macros by example
    locator: "[macro.decl.intro]; [macro.decl.meta.specifier]; [macro.decl.meta.transcription]; [macro.decl.hygiene.intro]; [macro.decl.hygiene.crate]"
  - url: https://doc.rust-lang.org/reference/procedural-macros.html
    title: The Rust Reference, Procedural macros
    locator: "[macro.proc.intro]; [macro.proc.def]; [macro.proc.result]; [macro.proc.proc_macro-crate.token-stream]; [macro.proc.hygiene]"
  - url: https://doc.rust-lang.org/book/ch20-05-macros.html
    title: The Rust Programming Language, 20.5 Macros
    locator: The Difference Between Macros and Functions
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    title: C++ Core Guidelines
    locator: "ES.30: Don't use macros for program text manipulation; ES.31: Don't use macros for constants or \"functions\"; ES.32: Use ALL_CAPS for all macro names"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/n4950.pdf
    title: N4950, Working Draft, Standard for Programming Language C++ (the C++23 draft)
    locator: 17.8.2.2 [support.srcloc.cons], paragraph 2 and example 1
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/preprocessor-directives
    title: C# preprocessor directives
    locator: the introduction and Conditional compilation
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/roslyn-sdk/source-generators-overview
    title: Source Generators overview
    locator: the first paragraphs
  - url: https://kotlinlang.org/docs/ksp-overview.html
    title: Kotlin Symbol Processing API
    locator: the introduction and Limitations
  - url: https://go.dev/blog/generate
    title: Rob Pike, Generating code, The Go Blog, 22 December 2014
    locator: the paragraphs on the go generate command and on checking in the generated file
  - url: https://docs.python.org/3/reference/compound_stmts.html
    title: The Python Language Reference, 8.7 Function definitions
    locator: the paragraph on decorator expressions
  - url: https://docs.python.org/3/library/ast.html
    title: Python documentation, ast, Abstract Syntax Trees
    locator: ast.NodeTransformer; ast.unparse; ast.fix_missing_locations
---

## Idea

A syntactic macro is a rule that rewrites program text before the compiler gives the text a meaning. The programmer writes a call to the macro. The compiler, or a preprocessor in front of it, replaces the call with the expansion that the rule produces, and then compiles the expansion as if the programmer had written it.

A function receives values when the program runs. A macro receives program text when the program is compiled. This one difference explains what macros can do and what goes wrong with them. A macro can take an argument without evaluating it, evaluate it twice, turn it into a string, or build an identifier from it. It can expand to a declaration, a statement or a whole type. A function can do none of these things. But a macro also has none of the protections of a function: its arguments have no types, its body can be evaluated more than once, and its names can clash with the names around the call.

Two families exist. The C preprocessor works on tokens and knows nothing of the grammar of C. Lisp, Scheme and Rust macros work on syntax: they receive a parsed form, a pattern match or a token tree with structure, and they return a form of the same kind. The second family can respect the scopes of the language. That property is called hygiene.

## How it works

### Token macros: the C preprocessor

The C standard describes translation in phases. Phase 4 executes the preprocessing directives and expands the macro invocations. The compiler proper sees the result.[^2] A macro definition "lasts (independent of block structure) until a corresponding #undef directive is encountered", and "macro definitions have no significance after translation phase 4".[^2] So a macro has no scope, no namespace and no type.

A function-like macro substitutes the token sequences of the arguments for its parameters. Then the preprocessor rescans the result for more macro names to replace. If it finds the name of the macro that is being replaced, it does not replace it again.[^2] This rule stops infinite expansion. GCC shows the effect: with `#define foo (4 + foo)`, each `foo` becomes `(4 + foo)` and stops there.[^6]

Two operators work on the text of an argument. `#` turns an argument into a string literal, and `##` joins two tokens into one.[^2] The C89 Committee introduced `#` for stringizing and "invented" `##` for token pasting, to replace older practices that the standard did not want to allow.[^7] C99 added macros with a variable number of arguments, written with `...` and `__VA_ARGS__`, because C89 had no way to write a macro that looks like a call to `printf`.[^7] The C example `text` uses both operators: `SHOW` prints the text of an expression next to its value, and `GETTER` builds the functions `get_x` and `get_y`.

The Rationale states one of the design principles of macro replacement: "Allow macros to be used wherever functions can be."[^7] Macros look like function calls. Most of the pitfalls below come from the fact that they do not behave like them.

### Syntax macros: Lisp

A Lisp program is a list. So a Lisp macro is a function from a list to a list that the compiler calls. In Common Lisp, `defmacro` defines a name "as a macro by associating a macro function with that name in the global environment". The expansion function receives a form and an environment and returns a form, which replaces the call.[^8] The body of the expansion function is a sequence of ordinary forms, and the value of the last one is the expansion.[^8]

Backquote makes the result readable. A backquoted form is a template. A comma marks an expression that is evaluated and inserted, and `,@` marks a list that is spliced into place.[^9]

A macro of this kind is not hygienic. If the template binds a variable called `tmp`, it hides a `tmp` that the caller passes in. The Common Lisp answer is `gensym`, which "creates and returns a fresh, uninterned symbol".[^10] A macro that binds its temporaries to names from `gensym` cannot capture the names of the caller.

### Hygiene: Scheme

Kohlbecker, Friedman, Felleisen and Duba published "Hygienic macro expansion" at the 1986 ACM Conference on LISP and Functional Programming.[^11] The Scheme report says that Scheme "became the first programming language to support hygienic macros". R7RS defines two mechanisms that prevent unintended conflicts:[^12]

- If a macro inserts a binding for an identifier, the identifier is in effect renamed throughout its scope.
- If a macro inserts a free reference to an identifier, the reference refers to the binding that was visible where the macro was defined, whatever local bindings surround the use.

The report concludes that macros defined with its pattern language "are 'hygienic' and 'referentially transparent' and thus preserve Scheme's lexical scoping". It cites Kohlbecker's thesis, the 1986 paper, and Clinger and Rees, "Macros that work" (1991), for these properties.[^12] The first rule stops a macro from capturing the names of the caller. The second rule stops the caller from capturing the names of the macro. The C example below breaks the second rule.

```c wrong id=free-name
#include <stdio.h>

static int verbose = 1;

#define LOG(msg) do { if (verbose) printf("log: %s\n", msg); } while (0)

int main(void) {
    int verbose = 0;
    LOG("start");
    printf("verbose = %d\n", verbose);
}
```

```output for=free-name
verbose = 0
```

The author of `LOG` meant the global `verbose`. The expansion of `LOG` appears inside `main`, so the name `verbose` refers to the local variable, and the log line never prints. A hygienic system would look up `verbose` where `LOG` was defined.

### Rust: macros by example and procedural macros

Rust has two kinds of macros. A `macro_rules!` macro is a list of rules. Each rule has a matcher and a transcriber, and the first rule whose matcher matches the call is used.[^13] A matcher binds metavariables to fragments of a given kind: `expr`, `ident`, `ty`, `block`, `pat` and others.[^13] A metavariable is replaced by the syntax element that it matched, and it "can be transcribed more than once or not at all".[^13] Because an `expr` fragment stays one syntax element, `square!(1 + 2)` with the body `$x * $x` computes 9. The precedence trap of C does not exist. But double evaluation still exists, as the Rust example `double-evaluation` shows.

Macros by example have mixed-site hygiene: "loop labels, block labels, and local variables are looked up at the macro definition site while other symbols are looked up at the macro invocation site".[^13] So the Rust `swap!` macro can declare `tmp` without hiding the caller's `tmp`, and a macro cannot read a local variable of its caller unless the caller passes the variable in. Functions, types and other macros are looked up at the call. A macro that names an item of its own crate writes `$crate::` so that the path works in other crates.[^13]

A procedural macro is a Rust function that runs at compile time. It consumes and produces Rust syntax, as a stream of tokens. It comes in three kinds: function-like (`custom!(...)`), derive (`#[derive(CustomDerive)]`) and attribute (`#[CustomAttribute]`).[^14] A procedural macro must be defined in a crate of type `proc-macro`, and that crate cannot use it.[^14] Procedural macros are unhygienic: they behave as if the output token stream was written inline at the call.[^14] They run with the same access to files and input as the compiler, so they have the same security concerns as build scripts.[^14] The `#[derive(Debug, PartialEq)]` of the Rust example `derive` calls derive macros of the standard library.

### When a function or a generic is better

The Rust book gives the tradeoff. A macro can take a variable number of arguments, and it runs before the compiler interprets the code, so it can, for example, implement a trait. But macro definitions "are generally more difficult to read, understand, and maintain than function definitions", and a macro must be defined or imported before its use in a file.[^15] The C++ Core Guidelines are stricter. Rule ES.31 says "Don't use macros for constants or 'functions'", because macros "don't obey the usual scope and type rules" and "don't obey the usual rules for argument passing". It replaces `#define PI 3.14` with a `constexpr` variable and a `SQUARE` macro with a function template.[^16]

Use a function when the job is a computation on values. A function evaluates each argument once, checks its types and has its own scope. Use a generic function, such as a C++ template or a Rust generic, when the computation must work for many types. See [[parametric-polymorphism]]. Use a [[higher-order-functions|higher-order function]] or a closure when the job is to delay or repeat a piece of code. A closure captures its variables by the rules of the language, so it has no capture problems. See [[closures]]. Use [[constant-evaluation]] when the job is to compute a value at compile time.

A macro is the right tool when the job needs the program text itself:

- the text of an argument, as in `assert` or a test macro that prints the failed expression;
- an identifier or a declaration made from parts, as in `GETTER`;
- the location of the call, unless the language has a library feature for it. In C++, a call to `std::source_location::current()` in a default argument corresponds to the location of the caller;[^17]
- syntax that a function cannot accept, such as a variable number of arguments of different kinds, or a block of code that must not run before the macro decides;
- code that the type system cannot express, such as an `impl` of a trait for many types, or the derive macros of Rust.

## In languages

| Language | Verdict | Form |
|---|---|---|
| C | native | the preprocessor: object-like and function-like macros, `#`, `##`, `__VA_ARGS__` |
| C++ | native | the preprocessor of C; templates and `constexpr` replace most uses |
| Rust | native | `macro_rules!` with mixed-site hygiene; procedural macros in a separate crate |
| Python | absent | no macros; the `ast` module can rewrite source text at run time |
| Java | absent | no macros; annotation processors generate source files |
| C# | absent | directives for conditional compilation only; source generators add files |
| Go | absent | no macros; `go generate` runs tools before the build |
| JavaScript | absent | no macros; build tools transform the source |
| TypeScript | absent | as JavaScript |
| Kotlin | absent | no macros; compiler plugins and symbol processors generate code |
| PowerShell | absent | no macros |
| Bash | absent | no macros; shell functions |
| SQL | absent | no macros in the standard |
| Regex | n/a | a pattern has no definitions that expand |
| HTML/CSS | n/a | no definitions that expand |

**C and C++** share the preprocessor. C++ adds scope to almost everything else, but a macro still ignores namespaces. The C++ example `no-scope` defines a macro `max` and then calls `std::numeric_limits<int>::max()`. The preprocessor sees `max` followed by `(` and expands it, and the compilation fails.

**Rust** is the only tier 1 language with syntactic macros that respect scope. Common parts of the standard library, such as `println!` and `vec!`, are macros. `println!` takes one argument or more, which a Rust function cannot do.[^15]

**C#** has preprocessor directives, but "unlike C and C++ directives, you can't use these directives to create macros". A symbol can be defined, but it has no value.[^18] Source generators fill part of the gap. They let a program add source files to a compilation at compile time.[^19] **Kotlin** has the Kotlin Symbol Processing API, a framework that generates source code, most often from annotations. It cannot see expressions or the bodies of functions.[^20] **Java** has annotation processors in the same role. See [[annotations]].

**Go** has no macros. The `go generate` command scans source files for special comments and runs the commands in them. It is not part of `go build` and must be run explicitly before it. The generated file must be checked into the source repository to be available to clients.[^21]

**Python** has no macros, but the `ast` module exposes the parser. `ast.NodeTransformer` rewrites a tree, `ast.fix_missing_locations` repairs the positions, and `compile` turns the tree into code.[^22] The Python example `ast-transform` does at run time what a macro does at compile time. Decorators look like macros but are not. A decorator expression is evaluated when the function is defined, and its result is called with the function object, not with the syntax of the function.[^23]

## Variations and related ideas

- **Object-like macros.** `#define PI 3.14` gives a name to a sequence of tokens. ES.31 replaces it with `constexpr double pi = 3.14;`, which has a type and a scope.[^16]
- **Conditional compilation.** `#if`, `#ifdef` and `#endif` select text before compilation. ES.30 does not ban macros for this use.[^16] C#, which has no macros, keeps these directives.[^18]
- **Templates and generics.** A C++ template also generates code at compile time, but from typed declarations and with the scoping rules of the language. See [[parametric-polymorphism]].
- **Compile-time evaluation.** `constexpr` functions in C++ and `const fn` in Rust compute values at compile time with the rules of ordinary functions. See [[constant-evaluation]].
- **Code generators.** Annotation processors, C# source generators, Kotlin symbol processors and `go generate` write source files. They see declarations, not the expressions inside functions, and they cannot change the code that the programmer wrote.[^19][^20][^21] See [[annotations]].
- **Run-time code transformation.** Python can parse, change and compile source text while the program runs.[^22] This gives the power of a macro without its compile-time checks.

## Pitfalls

### Missing parentheses

A C macro sees tokens, not expressions. With `#define SQUARE(x) x * x`, the call `SQUARE(1 + 2)` becomes `1 + 2 * 1 + 2`, which is 5. The C example `parentheses` shows it. The rule is to put parentheses around each use of a parameter and around the whole body. GCC shows the same trap with a `ceil_div` macro and gives the fixed form `(((x) + (y) - 1) / (y))`.[^3]

### Double evaluation

A parameter that appears twice in the body is evaluated twice. GCC calls a `min` macro with this property "an unsafe macro": in `min (x + y, foo (z))`, `foo` may be called twice.[^5] Parentheses do not help. The C example `double-evaluation` shows `MAX(i++, 0)` incrementing `i` twice, and the Rust example `double-evaluation` shows the same error in a `macro_rules!` macro. GCC offers a fix with a GNU extension, a statement expression that copies each argument into a local variable. In standard C, it says, the only solution is to be careful.[^5] In Rust, the fix is the same idea in standard syntax: `{ let v = $x; v * v }`. A function has this property without effort.

### Name capture

A C macro that declares a variable can hide a variable of the caller with the same name. The C example `capture` shows a `SWAP` macro that does nothing when the caller's variable is called `tmp`. The GNU fix for `min` gives its locals names with trailing underscores "to reduce the risk of conflict", and GCC adds that "it is impossible to avoid this entirely".[^5] The reverse error is in the `free-name` example above: the caller's variable captures a name that the macro meant to take from its own scope. Hygiene solves both problems in Scheme and, for local variables, in `macro_rules!`.[^12][^13] Rust procedural macros are not hygienic, so they use absolute paths such as `::std::option::Option`.[^14]

### Swallowing the semicolon

A macro that expands to a block `{ ... }` breaks an `if` with an `else`. The semicolon after the call is an empty statement, so the `else` has no `if`.[^4]

```c fails expect=compile id=semicolon
#include <stdio.h>

#define SKIP_SPACES(p) { while (*(p) == ' ') ++(p); }

int main(void) {
    const char *s = "  word";
    if (*s != '\0')
        SKIP_SPACES(s);
    else
        s = "empty";
    printf("%s\n", s);
}
```

The fix wraps the body in `do { ... } while (0)`. The expansion is then one statement that needs the semicolon, and the loop runs once.[^4] The `SWAP` and `LOG` macros on this page use this form.

### Macros that clash with other names

A macro name is replaced everywhere after its definition, in every scope and in every namespace.[^2] The C++ example `no-scope` shows a macro `max` that breaks a call to `std::numeric_limits<int>::max()`. The C++ Core Guidelines give macros names in `ALL_CAPS` to distinguish them from other names (rule ES.32).[^16]

### A macro that hides a bug

ES.30 shows `#define Case break; case`. One lower-case `c` in place of `C` turns it into a control-flow bug, because the reader sees something different from what the compiler sees.[^16]

## Cost

- **At run time.** None in principle. The expansion is ordinary code, and it costs what the same code costs when written by hand. A macro that evaluates an argument twice does the work twice.
- **At compile time.** Every call is expanded and then compiled. A procedural macro is a program that the compiler builds and runs before it can compile the crate that uses it.[^14]
- **For people and tools.** The reader sees the call, and the compiler sees the expansion. Error messages can point into the expansion. ES.30 says that macros "complicate tool building", and it names auto completers, static analyzers and debuggers.[^16] The Rust book says that macro definitions are harder to read and maintain than function definitions.[^15]

## History

- **1963.** Timothy P. Hart proposed "a MACRO instruction expander in define" for LISP 1.5, to replace the special forms that take an indefinite number of arguments or keep their arguments from being evaluated.[^1]
- **1986.** Kohlbecker, Friedman, Felleisen and Duba published "Hygienic macro expansion".[^11]
- **1989.** The C89 Committee introduced the `#` operator and invented the `##` operator, to replace older stringizing and token-pasting practices.[^7]
- **1991.** Clinger and Rees published "Macros that work".[^12]
- **1999.** C99 added macros with a variable number of arguments through `...` and `__VA_ARGS__`.[^7]
- **2014.** Go 1.4 included `go generate`, a command that runs code generators before the build.[^21]

## References
