---
tag: NBM1BP
kind: concept
slug: comment
name: Comment
aliases: [comment, comments, code comment, source code comment, line comment, block comment, doc comment, documentation comment, docstring, remark, commenting out]
area: syntax-and-semantics/lexical-structure
level: core
summary: >-
  A comment is text in source code that the compiler or interpreter ignores. It holds notes for human readers, and tools can read some comments as documentation or as directives.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [Bjarne Stroustrup]
  note: The 1956 FORTRAN manual for the IBM 704 let a programmer punch C in column 1 of a card, so that the compiler ignored the card and the listing showed it as a comment. C++ took the line comment // from BCPL, and C added it in C99.
see_also: [tokens-and-lexing, annotations, syntactic-macros, runtime-reflection]
same_as:
  wikidata: Q1141067
  wikipedia: Comment_(computer_programming)
sources:
- url: https://docs.python.org/3/reference/lexical_analysis.html
  title: Lexical analysis, The Python Language Reference
  locator: 2.1.3 Comments; 2.1.4 Encoding declarations
- url: https://doc.rust-lang.org/reference/comments.html
  title: Comments, The Rust Reference
  locator: Lexer; Doc comments
- url: https://en.cppreference.com/w/c/comment.html
  title: Comments, C reference, cppreference.com
  locator: Explanation; Notes
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 3
  locator: 3.3 Unicode Escapes; 3.7 Comments
- url: https://en.cppreference.com/w/cpp/comment.html
  title: Comments, C++ reference, cppreference.com
  locator: C-style; Notes
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/xmldoc/
  title: Documentation comments, C# language reference
  locator: Introduction
- url: https://go.dev/doc/comment
  title: Go Doc Comments
  locator: Introduction; Directives
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar
  title: Lexical grammar, MDN Web Docs
  locator: Comments
- url: https://peps.python.org/pep-0257/
  title: PEP 257, Docstring Conventions
  locator: What is a Docstring?
- url: https://archive.computerhistory.org/resources/text/Fortran/102649787.05.01.acc.pdf
  title: The FORTRAN Automatic Coding System for the IBM 704 EDPM, Programmer's Reference Manual, IBM, 1956
  locator: Chapter 1, Keypunching the Program
- url: https://en.wikipedia.org/wiki/Comment_(computer_programming)
  title: Comment (computer programming), Wikipedia
  locator: Fortran
- url: https://www.stroustrup.com/hopl2.pdf
  title: A History of C++, 1979 to 1991, Bjarne Stroustrup
  locator: Section 3, From C with Classes to C++; 3.3.1 Minor Changes
---

## Idea

A comment is a part of the source text that is for people and not for the machine. The lexer finds each comment and drops it, so the program means the same with or without it. To find a comment, the lexer must know where strings start and end, because a comment marker inside a string literal is plain text.

```python run id=idea
import io
import re
import tokenize

src = '''x = 1  # set x
url = "http://example.com/#top"  # the # in the string is not a comment
y = x + 1
'''

# A naive rule cuts at the first #, which breaks the string.
naive = [line.split("#")[0].rstrip() for line in src.splitlines()]
print("naive:", naive[1])

# A lexer knows where strings start and end, so it finds only the real comments.
for tok in tokenize.generate_tokens(io.StringIO(src).readline):
    if tok.type == tokenize.COMMENT:
        print(f"line {tok.start[0]} col {tok.start[1]:>2}: {tok.string}")

# After the lexer drops comments, the program is the same token stream.
def code_tokens(s):
    return [t.string for t in tokenize.generate_tokens(io.StringIO(s).readline)
            if t.type not in (tokenize.COMMENT, tokenize.NL, tokenize.NEWLINE, tokenize.ENDMARKER)]

print(code_tokens("y = x + 1  # add one\n") == code_tokens("y = x + 1\n"))
```

```output for=idea
naive: url = "http://example.com/
line 1 col  7: # set x
line 2 col 33: # the # in the string is not a comment
True
```

The naive rule cuts each line at the first `#`, so it breaks the URL in the string. The tokenizer reports only the two real comments, with their line and column. The last line shows that a comment does not change the program: the code tokens of the two lines are the same.

## How it works

**Line and block comments.** A line comment starts with a marker and ends at the end of the line: `#` in Python, Bash and many scripting languages, `//` in the C family, `--` in SQL. A block comment has a start marker and an end marker, such as `/* */`, and can cover part of a line or many lines. Python has only the line form. In Python, "A comment starts with a hash character (#) that is not part of a string literal, and ends at the end of the physical line", and "Comments are ignored by the syntax"[^1].

**The lexer removes them.** Comments are not tokens. The lexer reads them and drops them, or turns them into whitespace, so the parser never sees them. The Rust reference says that "Non-doc comments are interpreted as a form of whitespace"[^2]. In C, "All comments are removed from the program at translation phase 3 by replacing each comment with a single whitespace character"[^3], so `int/**/x` is `int x` and not `intx`. Because comments go before the preprocessor runs, "a macro cannot be used to form a comment"[^3].

**Order of the lexical steps.** A language can change the source text before it looks for comments, and this can surprise a reader. In C, a backslash at the end of a line joins the next line to it in phase 2, so a `//` comment that ends with a backslash also hides the next line[^3]. In Java, the compiler translates Unicode escapes such as `\u000a` before it finds comments, so an escape for a line break ends a line comment, and the text after it is code[^4].

**Nesting.** In C, C++, Java and JavaScript, block comments do not nest: the first `*/` ends the comment[^4][^3][^5]. The Java specification states that "Comments do not nest"[^4]. So a programmer cannot comment out a block of code that already holds a block comment. Rust is different: "Nested block comments are supported"[^2].

**Comments that tools read.** Some comments have meaning for a tool, but not for the compiler:

- A doc comment documents the declaration after it. Javadoc reads `/** */` in Java, and in C# the compiler "finds all comment fields with XML tags in the source code and creates an XML documentation file from those comments"[^6]. In Go, a doc comment is a comment that appears "immediately before top-level package, const, func, type, and var declarations with no intervening newlines"[^7].
- In Rust, `///` and `//!` doc comments are attributes: `/// Foo` "turns into `#[doc=" Foo"]`"[^2].
- A directive is a comment that tells a tool what to do. In Go, "Directive comments such as //go:generate are not considered part of a doc comment and are omitted from rendered documentation"[^7]. In Python, a comment in the first or second line can declare the encoding of the file[^1]. In JavaScript, a hashbang comment `#!` is valid only at the start of a script or module[^8].

**Docstrings are not comments.** A Python docstring "is a string literal that occurs as the first statement in a module, function, class, or method definition", and it "becomes the `__doc__` special attribute of that object"[^9]. It stays in the program at run time, and a comment does not.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `#` line comments; docstrings are string literals |
| JavaScript | native | `//`, `/* */`; `#!` at the start of a script |
| TypeScript | native | as JavaScript; `/// <reference>` directives |
| Java | native | `//`, `/* */`; Javadoc `/** */` |
| Kotlin | native | `//`, `/* */` that nest; KDoc `/** */` |
| C# | native | `//`, `/* */`; XML doc comments `///` |
| Go | native | `//`, `/* */`; doc comments; `//go:` directives |
| Rust | native | `//`, `/* */` that nest; `///` and `//!` doc comments |
| C | native | `/* */`; C99 `//` |
| C++ | native | `//`, `/* */` |
| Bash | native | `#` at the start of a word |
| PowerShell | native | `#`, `<# #>`; comment-based help |
| SQL | native | `--`, `/* */` |
| Regex | native | `(?#...)` and verbose mode in many engines; not in JavaScript |
| HTML | native | `<!-- -->` |
| CSS | native | `/* */` only |

Every language in the table has comments, so the verdict is native. The languages differ in three things: whether block comments nest, which comments tools read as documentation, and how the other lexical steps interact with comments.

The example files show these differences. Python compares a docstring with a comment and shows that the compiled code keeps only the docstring. JavaScript shows that block comments do not nest, that a block comment with a line break ends a `return` statement, and that `JSON.parse` does not accept comments. Java shows a Unicode escape that ends a line comment. C# reads an attribute by reflection, because reflection cannot see comments. Go reads doc comments and a directive with `go/parser`. Rust shows nested comments and doc attributes. C shows a line splice, the one space that replaces a comment, and the `//*` sequence that C89 and C99 read differently. C++ shows a raw string, `#if 0` and a macro that cannot make a comment.

## Variations and related ideas

- **Commenting out code.** A programmer often turns code into a comment to stop it from running. Line comments are safer for this than block comments, because block comments do not nest in most languages. In C and C++, `#if 0` and `#endif` remove a block that holds comments[^5].
- **Doc comments and doc generators.** Javadoc, the C# XML documentation, Go doc comments, rustdoc and Doxygen turn comments into reference documentation. See [annotations](/annotations/) for metadata that the compiler keeps.
- **Docstrings.** Python, Lisp dialects and Julia keep documentation as a string value at run time, so a program can read it, for example with `help`.
- **Pragmas and directives.** `//go:` lines, `# type: ignore` for Python type checkers, `// eslint-disable` and `#!` lines are comments to the language but instructions to a tool. See [syntactic macros](/syntactic-macros/) for other text that tools change before compilation.
- **Comments in data formats.** JSON has no comments. YAML and TOML use `#`, and XML and HTML use `<!-- -->`.

## Pitfalls

- **Comments that do not match the code.** The compiler does not check comments, so a comment can stay after the code changes. A wrong comment is worse than no comment. Explain why the code does something, and let the code show what it does.
- **Commented-out block comments.** `/* ... */` around code that holds `*/` ends early, and the rest is code again. Use line comments, `#if 0` in C, or delete the code and use version control.
- **A backslash at the end of a C line comment.** It joins the next line to the comment, so the next statement is lost. Clang and GCC warn with `-Wcomment`.
- **Unicode escapes in Java comments.** `\u000a` ends a line comment, and the escapes for `*` and `/` end a block comment, because the compiler translates them first.
- **Comment markers in strings and regular expressions.** A tool that strips comments with a simple pattern breaks strings such as `"http://"`. Use the tokenizer of the language, as the idea example does.
- **Block comments and automatic semicolon insertion.** In JavaScript, a block comment that contains a line break acts as a line break for automatic semicolon insertion[^8], so `return /* ... */` across lines returns `undefined`.
- **Comments in shipped code.** Comments in JavaScript, CSS and HTML go to the browser unless a minifier removes them. Do not put secrets or internal notes in them.

## Cost

A compiled program has no cost from comments at run time, because the compiler removes them. The lexer still reads each comment, so very large comments make the build a little slower. In an interpreted language, the interpreter also drops comments when it reads the source. Docstrings are different, because they are values: Python keeps them in memory, and the option `-OO` removes them. In code that goes over the network, such as JavaScript for a web page, comments add bytes until a minifier removes them.

## History

The 1956 FORTRAN manual for the IBM 704 says that "Punching the character C in column 1 will cause the card to be ignored by FORTRAN. Such cards may therefore be used to carry comments which will appear when the deck is listed"[^10]. Fixed-form Fortran kept this rule, and Fortran 77 also allowed an asterisk in column 1[^11].

The two comment forms of the C family have different sources. C had only `/* */`. Bjarne Stroustrup wrote that, of the languages he considered, BCPL "gave // comments" to C++ as released in 1985, and that "Since both styles of comments are allowed people can simply use the style they like best"[^12]. C added `//` in C99[^3]. Later languages such as Java, C#, JavaScript, Go and Rust kept both forms. Java, C# and Go added doc comments, Python added docstrings, and Rust made doc comments attributes[^9][^4][^6][^7][^2].

## References
