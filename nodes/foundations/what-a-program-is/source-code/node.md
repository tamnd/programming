---
tag: 3CE4MV
kind: concept
slug: source-code
name: Source code
aliases: [source code, source, source text, source file, code, program text, translation unit, compilation unit, open source]
area: foundations/what-a-program-is
level: core
summary: >-
  Source code is the text of a program in the form that people write, read and change. Tools turn it into tokens, a tree and then code that a machine can run.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1957
  people: [Kathleen Booth, David Wheeler, John Backus, Marc Rochkind, Richard Stallman]
  note: Assembly languages from 1947 and FORTRAN in 1957 made the text that people write different from the code that the machine runs. SCCS kept the history of source files from 1972, and the Open Source Definition of 1998 made access to source code a licence term.
see_also: [computer-program, compilers-and-interpreters, tokens-and-lexing, abstract-syntax-tree, bytecode, syntactic-macros, utf-8, git]
same_as:
  wikidata: Q128751
  wikipedia: Source_code
sources:
- url: https://opensource.org/osd
  title: The Open Source Definition, Open Source Initiative
  locator: 2. Source Code
- url: https://docs.python.org/3/reference/lexical_analysis.html
  title: Lexical analysis, The Python Language Reference
  locator: Encoding declarations
- url: https://docs.python.org/3/library/tokenize.html
  title: tokenize, Tokenizer for Python source, Python documentation
  locator: Introduction
- url: https://docs.python.org/3/library/inspect.html
  title: inspect, Inspect live objects, Python documentation
  locator: getsource
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/toString
  title: Function.prototype.toString(), MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/StackTraceElement.html
  title: Class StackTraceElement, Java SE 25 API documentation
  locator: getLineNumber()
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/attributes/caller-information
  title: Determine caller information using attributes interpreted by the C# compiler, Microsoft Learn
  locator: Argument expressions
- url: https://pkg.go.dev/go/parser
  title: Package parser, Go standard library documentation
  locator: Overview
- url: https://doc.rust-lang.org/std/macro.stringify.html
  title: Macro stringify, Rust standard library documentation
  locator: Description
- url: https://en.cppreference.com/w/c/preprocessor/replace.html
  title: Replacing text macros, C reference, cppreference.com
  locator: Predefined macros
- url: https://en.cppreference.com/w/cpp/utility/source_location.html
  title: std::source_location, C++ reference, cppreference.com
  locator: Description
---

## Idea

Source code is the text of a program: the files that a programmer writes, reads, reviews and changes. A machine cannot run this text directly. A compiler or an interpreter reads it in steps: it cuts the characters into tokens, it builds a tree that shows the structure, and it turns the tree into machine code, bytecode or actions. Each step drops something that matters only to people, such as comments, spaces and the choice of names, so the text is the only form that keeps everything that the authors meant to say.

```python run id=idea
import ast
import io
import tokenize

source = "total = price * 3  # three items\nprint(total)\n"
print("characters:", len(source), "lines:", source.count("\n"))

# 1. The scanner cuts the text into tokens.
for tok in tokenize.generate_tokens(io.StringIO(source).readline):
    if tok.start[0] == 1 and tok.type != tokenize.NEWLINE:
        print(f"  {tokenize.tok_name[tok.type]:7} {tok.string!r}")

# 2. The parser builds a tree. The comment and the spaces are gone.
tree = ast.parse(source)
print(ast.dump(tree.body[0]))
print("back to text:", ast.unparse(tree.body[0]))

# 3. The compiler makes code that the machine can run.
code = compile(tree, "<example>", "exec")
exec(code, {"price": 4})
```

```output for=idea
characters: 46 lines: 2
  NAME    'total'
  OP      '='
  NAME    'price'
  OP      '*'
  NUMBER  '3'
  COMMENT '# three items'
Assign(targets=[Name(id='total', ctx=Store())], value=BinOp(left=Name(id='price', ctx=Load()), op=Mult(), right=Constant(value=3)))
back to text: total = price * 3
12
```

The source is one string of 46 characters on two lines. The scanner in `tokenize` cuts the first line into names, operators, a number and a comment. The parser in `ast` builds a tree with an assignment, a multiplication and a constant, and the comment and the extra spaces are not in the tree, so the text that `ast.unparse` makes from the tree has lost them. `compile` turns the tree into a code object, and `exec` runs it with a value for `price`.

## How it works

**Text in files.** Source code is plain text, stored in files with a name and an extension, such as `main.go` or `area.py`. The language defines how the bytes of a file become characters. Most modern languages read UTF-8: in Python, "If no encoding declaration is found, the default encoding is UTF-8."[^2] The language also defines which file is the unit of compilation: a C translation unit is one source file after the preprocessor has inserted its headers, a Java compilation unit is one `.java` file, and a Go package is all the files of one directory.

**From text to tokens.** The first step of a compiler is lexical analysis. It reads the characters and groups them into tokens: names, keywords, numbers, strings, operators and punctuation. Spaces and comments separate tokens, and most languages then drop them. Python's `tokenize` module is "a lexical scanner for Python source code" that also "returns comments as tokens", which tools such as formatters and colorizers need.[^3] See [tokens and lexing](/tokens-and-lexing/).

**From tokens to a tree.** The parser checks the tokens against the grammar of the language and builds an [abstract syntax tree](/abstract-syntax-tree/). Different texts can give the same tree: `x=(a)+1` and `x = a + 1` parse to the same assignment. Go's `go/parser` "implements a parser for Go source files" and keeps the position of each node, so tools can report an error at a file, a line and a column.[^8] Formatters such as `gofmt`, `rustfmt` and Black parse the source and print the tree again in one standard layout, which ends arguments about style.

**From the tree to running code.** A compiler turns the tree into machine code or [bytecode](/bytecode/), and an interpreter runs it. See [compilers and interpreters](/compilers-and-interpreters/). The compiler keeps a map from the output back to the source, as debug information, line tables or source maps, so that a debugger and a stack trace can show the source line of the running code. Java's `StackTraceElement.getLineNumber` "Returns the line number of the source line containing the execution point represented by this stack trace element."[^6]

**Source text inside the program.** Some languages keep the source, or parts of it, for the running program. In CPython, `inspect.getsource` returns "the text of the source code for an object" by reading the file again.[^4] In JavaScript, a function's `toString` "returns a string containing the source text segment which was used to define the function".[^5] Compiled languages can put pieces of the source into the program when it compiles: C has `__LINE__`, which "expands to the source file line number", and the `#` operator, which turns a macro argument into a string literal.[^10] C++ has `std::source_location`, which "represents certain information about the source code, such as file names, line numbers, and function names".[^11] C# fills parameters with the caller's line and argument text,[^7] and Rust's `stringify!` "Stringifies its arguments".[^9] Assertion and logging libraries use these to print the failed expression and its line.

**Source code as the preferred form.** Source code is more than the input to a compiler. It is the form in which people understand and change a program, so licences define it that way. The Open Source Definition says: "The source code must be the preferred form in which a programmer would modify the program."[^1] It also excludes "Deliberately obfuscated source code" and "Intermediate forms such as the output of a preprocessor or translator". Programs from a generator, a minifier or a transpiler are text, but they are not the source in this sense.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `.py` files in UTF-8; `tokenize`, `ast`, `inspect.getsource` |
| JavaScript | native | scripts and modules; `Function.prototype.toString`; `new Function` |
| TypeScript | native | `.ts` files that the compiler turns into JavaScript |
| Java | native | compilation units in `.java` files; `javax.tools`; stack traces |
| Kotlin | native | `.kt` files; the compiler writes JVM class files |
| C# | native | `.cs` files; caller information attributes; Roslyn |
| Go | native | packages of `.go` files in UTF-8; `go/parser`, `go/format` |
| Rust | native | crates of `.rs` files; `stringify!`, `line!`, `column!` |
| C | native | translation units; the preprocessor, `__LINE__`, `#` |
| C++ | native | translation units and modules; `std::source_location` |
| Bash | native | script files that the shell reads line by line |
| PowerShell | native | `.ps1` files; the parser API in `System.Management.Automation.Language` |
| SQL | native | statements as text that the database parses and plans |
| Regex | native | a pattern is source text that the engine compiles |
| HTML | native | documents as text that the browser parses into a tree |
| CSS | native | style sheets as text that the browser parses into rules |

Every language in the table has source code, because every one is written as text. The difference is what the running program keeps. Python and JavaScript keep enough to give the text of a function back. Java and C# keep line tables for stack traces, and their compilers are libraries that a program can call. C, C++ and Rust keep nothing by default, but macros and built-ins copy pieces of the source into the program when it compiles.

The example files show the stages and the tools. Python prints a function's own source, its first line and the tree that loses the comment. JavaScript gets the text of a function and compiles new text with `new Function`. Java writes a source file, compiles it with the compiler API and reads the class file. C# prints the text and line of each argument at the call. Go formats messy source with `go/format` and lists each name with its position. Rust, C and C++ make assertion macros that print the expression and the line.

## Variations and related ideas

- **Generated source code.** Parser generators, protocol buffer compilers and code generators write source files from a description. The description is the real source, and the generated files are usually marked so that people do not edit them.
- **Transpilers and minifiers.** TypeScript, Babel and minifiers turn one source text into another. A source map relates positions in the output to positions in the input.
- **Literate programming.** Donald Knuth's literate programming, which he described in 1984, kept a program and its explanation in one document, and tools extracted the code and the typeset text from it.
- **Visual and structured languages.** Scratch, spreadsheets and some industrial languages store a program as blocks or a tree, not as free text, and the editor prevents syntax errors.
- **Notebooks.** A Jupyter notebook stores code cells with their output in one JSON file, so the source and the results of a run travel together.
- **Syntactic macros.** [Syntactic macros](/syntactic-macros/) take source as tokens or trees and give new source that the compiler then reads.

## Pitfalls

- **Encoding problems.** A file saved in one encoding and read in another changes its strings and can break the compile. Save source as UTF-8 without a byte order mark unless the language needs something else. See [UTF-8](/utf-8/).
- **Invisible characters.** Tabs mixed with spaces break the indentation of Python and Makefiles, and bidirectional control characters and look-alike letters can make code look different from what the compiler reads, as in the Trojan Source attacks of 2021.
- **Running text as code.** `eval`, `new Function` and string building for SQL or shell commands turn data into source code, which lets an attacker inject code. See [code injection](/code-injection/).
- **Line numbers that do not match.** Generated code, macros and minified bundles report lines in the output, not in the source that a person wrote, unless a source map or `#line` directive maps them back.
- **Editing generated files.** A change to a generated file is lost the next time the generator runs. Change the input of the generator instead.
- **Relying on the text of a function.** `Function.prototype.toString` and `inspect.getsource` fail for built-in functions and for code without a file, and minifiers change the text.

## Cost

The size of source code drives the time to compile it, the time to read it and the number of defects in it. Compile time grows with the amount of text after macro expansion and header inclusion, which is why large C++ projects spend much of their build time parsing the same headers again, and why C++ modules and precompiled headers exist. Tokenizing and parsing are fast, often linear in the length of the text; type checking, optimization and code generation take most of the time of a compile. For people, the cost is reading: code is read many more times than it is written, so clear names, a standard layout and short functions reduce the cost of each change.

## History

The first stored-program computers ran numbers in memory that programmers wrote by hand. Kathleen Booth wrote an assembly notation for the ARC computer in 1947, and David Wheeler's initial orders for the EDSAC, in 1949, read instructions written as letters and decimal numbers from paper tape and turned them into binary. From then on, the text that people wrote was different from the code that the machine ran. John Backus's team delivered the FORTRAN compiler in 1957, and the source of a program became formulas and statements on punched cards, one line per card.

Source moved from cards to files in time-sharing systems in the 1960s and 1970s. Marc Rochkind wrote SCCS at Bell Labs in 1972 to keep the history of each source file, the first in a line of version control systems that leads to [Git](/git/). Richard Stallman started the GNU project in 1983 because vendors had stopped giving source code to users, and the Open Source Definition of 1998 made access to the preferred form of the source a term of open source licences.

## References
